#include "Camera.h"

// 输入参数文件名
void Camera_init(H5::H5File& file)
{
    // 创建 'Camera' 分组
    H5::Group CameraGroup = file.createGroup(CAMERA_GROUP_NAME);
    if(image_Compress_Flag == 0)
    {
        for (int CameraIndex = 0; CameraIndex < Camera_group; CameraIndex++) {
             // 创建数据集
             std::string subDataSet = "CameraData_" + std::to_string(CameraIndex);
             hsize_t dims[4] = {timeStamp, image_Height, image_Width, channels}; 
             H5::DataSpace dataspace(4, dims);
             H5::DataType datatype = H5::PredType::NATIVE_UINT8; 
             H5::DataSet imageDataset = CameraGroup.createDataSet(subDataSet, datatype, dataspace);
             // 创建一个属性数据空间
             H5::DataSpace attrSpace(H5S_SCALAR);       
             H5::DataType attr_datatype = H5::StrType( H5::PredType::C_S1, H5T_VARIABLE); 
             H5::Attribute attribute = imageDataset.createAttribute("ID", attr_datatype, attrSpace);
             H5std_string ID = CameraID;
             attribute.write(attr_datatype, ID); 
        }
     }
}

void Camera_collection(void)
{    
    // 打开摄像头
    cv::VideoCapture cap(Camera_Port);  
    // 检查摄像头是否成功打开
    if (!cap.isOpened()) {
    std::cerr << "Error: Could not open camera." << std::endl;
    }
    cv::Mat image;
    cap.set(cv::CAP_PROP_FRAME_WIDTH, image_Width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, image_Height);
    cap.set(cv::CAP_PROP_FPS, image_Rates);// 设置摄像头的帧率，例如设置为50帧每秒
    while(true)
    {
          while (!thread3Ready.load(std::memory_order::memory_order_acquire)) {
                // 等待 thread3Ready 为 true
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }    
          // 从摄像头读取一帧图像
          cap >> image;
          //Camera接口
          if(image_Compress_Flag == 0)
          {
             // 将图像转换为 RGB 格式
             cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
             for (int y = 0; y < image_Height; y++) {
                  for (int x = 0; x < image_Width; x++) {
                       // 将 RGB 像素数据添加到像素数据向量
                       int offset = y * image_Width * channels + x * channels;
                       imageData[offset] = image.at<cv::Vec3b>( y , x )[0];
                       imageData[offset + 1] = image.at<cv::Vec3b>(y ,x )[1];
                       imageData[offset + 2] = image.at<cv::Vec3b>(y ,x )[2];
                      }
                }     

          }
          if(image_Compress_Flag == 1)
          {
             CameraData.push_back(image);
          }
          thread3Ready.store(false, std::memory_order::memory_order_release);
    }
}

//辅助数据
std::vector<cv::Mat> Data;
// 输入子数据集 "CameraData_x"，并使用时间戳作为第一个维度
void Camera_Data_writing(int CameraIndex, hsize_t currentTimeStamp,int Writing_Index)
{
    if(image_Compress_Flag == 0)
    {
       //写入数据
       H5::H5File file("Data_"+ std::to_string(Writing_Index)+".h5", H5F_ACC_RDWR);
       H5::Group CameraGroup = file.openGroup(CAMERA_GROUP_NAME);
       std::string subDataSet = "CameraData_" + std::to_string(CameraIndex); 
       H5::DataSet CameraDataset =CameraGroup.openDataSet(subDataSet); 
       hsize_t start[4] = {currentTimeStamp, 0, 0, 0};
       hsize_t count[4] = {1, image_Height, image_Width, channels};
       H5::DataSpace memspace(4, count);
       H5::DataSpace dataspace = CameraDataset.getSpace(); 
       H5::DataType datatype = CameraDataset.getDataType(); 
       // 选择数据集的超集以写入数据
       dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
       CameraDataset.write(imageData.data(), datatype, memspace, dataspace);
    }
    if(image_Compress_Flag == 1)
    {  
       if(currentTimeStamp == (timeStamp-1))
       {
          Data=CameraData;
          CameraData.clear();
          // 将所有图像数据压缩成一个一维向量，并写入数据集
          std::vector<uchar> Camera_compressedData;        
          compressData(Data, Camera_compressedData);
          // 创建数据集并写入数据
          H5::H5File file("Data_" + std::to_string(Writing_Index) + ".h5", H5F_ACC_RDWR);
          H5::Group CameraGroup = file.openGroup(CAMERA_GROUP_NAME);
          std::string subDataSet = "CameraData_" + std::to_string(CameraIndex);
          hsize_t dims[1] = {Camera_compressedData.size()};
          H5::DataSpace dataspace(1, dims);
          H5::DataType datatype = H5::PredType::NATIVE_UCHAR;
          H5::DataSet CameraDataset = CameraGroup.createDataSet(subDataSet, datatype, dataspace);
          CameraDataset.write(Camera_compressedData.data(), datatype, dataspace);
          // 创建一个属性数据空间
          H5::DataSpace attrSpace(H5S_SCALAR);
          H5::DataType attr_datatype = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
          H5::Attribute attribute = CameraDataset.createAttribute("ID", attr_datatype, attrSpace);
          H5std_string ID = CameraID;
          attribute.write(attr_datatype, ID);
       }
    }
}

void compressData(std::vector<cv::Mat>& frames, std::vector<uchar>& output) {
    // 初始化 x264 编码器
    x264_param_t param;
    x264_param_default_preset(&param, "ultrafast", "zerolatency");
    param.i_log_level = X264_LOG_NONE;  // 禁用输出信息
    param.i_width = image_Width;
    param.i_height = image_Height;
    param.i_fps_num = System_Rates;  // 帧率 fps
    param.i_fps_den = 1;
    param.i_csp = X264_CSP_I420;
    param.rc.i_qp_constant = 0;  // 设置量化参数为0，实现无损压缩
    x264_t* encoder = x264_encoder_open(&param);
    // 分配图像帧
    x264_picture_t pic;
    x264_picture_alloc(&pic, X264_CSP_I420, image_Width, image_Height);
    // 编码每一帧
    for (const auto& frameMat : frames) {
        // 将 OpenCV 的 Mat 转换为 x264 的图像帧
        uint8_t* y = pic.img.plane[0];
        uint8_t* u = pic.img.plane[1];
        uint8_t* v = pic.img.plane[2];
        for (int i = 0; i < image_Height; ++i) {
            for (int j = 0; j < image_Width; ++j) {
                y[i * image_Width + j] = frameMat.at<cv::Vec3b>(i, j)[0];  // B
                u[i / 2 * image_Width / 2 + j / 2] = frameMat.at<cv::Vec3b>(i, j)[1];  // G
                v[i / 2 * image_Width / 2 + j / 2] = frameMat.at<cv::Vec3b>(i, j)[2];  // R
            }
        }
        // 编码图像帧
        x264_nal_t* nals;
        int i_nals;
        x264_picture_t pic_out;
        int frame_size = x264_encoder_encode(encoder, &nals, &i_nals, &pic, &pic_out);
        if (frame_size > 0) {
            // 将编码后的数据存入输出向量
            for (int i = 0; i < i_nals; ++i) {
                output.insert(output.end(), nals[i].p_payload, nals[i].p_payload + nals[i].i_payload);
            }
        } 
    }
    // 清理资源
    x264_encoder_close(encoder);
    x264_picture_clean(&pic);
}

