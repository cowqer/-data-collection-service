#include "Camera.h"

// 输入参数文件名
void Camera_init(H5::H5File& file)
{
    // 创建 'Camera' 分组
    H5::Group CameraGroup = file.createGroup(CAMERA_GROUP_NAME);
    // 需要存储的Camera个数
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

void Camera_collection(void)
{    
    // 打开摄像头
    cv::VideoCapture cap(Camera_Port);  
    cv::Mat image;
    while(true)
    {
    // 从摄像头读取一帧图像
    cap >> image;
    // 将图像转换为 RGB 格式
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    //Camera接口
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
}

// 输入子数据集 "CameraData_x"，并使用时间戳作为第一个维度
void Camera_Data_writing(int CameraIndex, hsize_t currentTimeStamp)
{
    //写入数据
    H5::H5File file("Data_"+ std::to_string(File_Index-1)+".h5", H5F_ACC_RDWR);
    H5::Group CameraGroup = file.openGroup(CAMERA_GROUP_NAME);
    std::string subDataSet = "CameraData_" + std::to_string(CameraIndex); 
    H5::DataSet imageDataset =CameraGroup.openDataSet(subDataSet); 
    hsize_t start[4] = {currentTimeStamp, 0, 0, 0};
    hsize_t count[4] = {1, image_Height, image_Width, channels};
    H5::DataSpace memspace(4, count);
    H5::DataSpace dataspace = imageDataset.getSpace(); 
    H5::DataType datatype = imageDataset.getDataType(); 
    // 选择数据集的超集以写入数据
    dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
    imageDataset.write(imageData.data(), datatype, memspace, dataspace);
}

