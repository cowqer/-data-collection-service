#include "main.h"

int main() {
    //读取Json文件
    Json_init();
    // 启动多个线程
    std::thread threads[numThreads];
    threads[0] = std::thread(File_init);
    threads[1] = std::thread(Data_writing);
    threads[2] = std::thread(run_mqtt_client,"fusion-dc.net");
    threads[3] = std::thread(Camera_collection);
    threads[4] = std::thread(Radar_collection);
    threads[5] = std::thread(Lidar_collection);
    threads[6] = std::thread(GPS_collection);
    threads[7] = std::thread(IMU_collection);
    //等待采集初始化
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //第一个文件初始化
    //初始H5根组
     H5::H5File file = H5_init();
    // 初始化Sensor
    Camera_init(file);
    Radar_init(file);
    Lidar_init(file);
    GPS_init(file);
    IMU_init(file);
    //初始化时间戳
    TimeStamp_init(file);
    std::cout << "Data_0文件初始化成功" << std::endl;
    while(true){
          currentTimestamp = 0;
          // 通知File_init线程可以执行
          thread0Ready.store(true, std::memory_order::memory_order_release);
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          //根据时间戳进行采集工作
          while (currentTimestamp < timeStamp) {
                 if (std::chrono::system_clock::now() >= targetTime) {
                     targetTime += std::chrono::milliseconds(100);
                     // 通知Data_writing线程可以执行
                     thread1Ready.store(true, std::memory_order::memory_order_release);
                     //等待记录写入时间
                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
                     currentTimestamp++; // 增加时间戳计数 
                     }else {
                     // 如果还没到时间，可以休眠一段时间再检查
                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
                     }      
                 }
           std::cout << "Data_"<< std::to_string(File_Index-1)<<"文件写入成功" << std::endl;      
           // 通知UpLoad线程可以执行
           thread2Ready.store(true, std::memory_order::memory_order_release);
           }
   return 0;
}

void Json_init(void)
{
    //读取JSON配置数据
    config_file >> config;
    Camera_Port=config["Camera_Port"];
    IMU_Port=config["IMU_Port"];
    GPS_Port=config["GPS_Port"];
    Lidar_Port=config["Lidar_Port"];
    timeStamp=config["timeStamp"];
    CameraID=config["CameraID"];
    IMUID=config["IMUID"];
    GPSID=config["GPSID"];
    RadarID=config["RadarID"];
    LidarID=config["LidarID"];
    VehicleID=config["VehicleID"];
    image_Width=config["image_Width"];
    image_Height=config["image_Height"];
    Lidar_line=config["Lidar_line"];
    Lidar_DataWidth=config["Lidar_DataWidth"];
    std::cout << "Json文件读取完成" << std::endl;
    //初始化准备数据
    imageData.resize(image_Height * image_Width  * channels);
    Lidar_Data.resize(Lidar_line * Lidar_DataWidth * Lidar_Entry);
    GPS_Data.resize(GPS_Entry);
    IMU_Data.resize(IMU_Entry);
}

void Data_writing(void)
{    
     while (true) {
            while (!thread1Ready.load(std::memory_order::memory_order_acquire)) {
                  // 等待 thread0Ready 为 true
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));
                  }
                  int NowTimestamp = currentTimestamp;
                   // 存储图片到指定Camera_x中
                   Camera_Data_writing(0, NowTimestamp);
                   // 存储到指定Lidar_x中
                   Lidar_Data_writing(0, NowTimestamp);
                   // 存储到指定Radar_x中
                   Radar_Data_writing(0, NowTimestamp);
                   // 存储到指定GPS中
                   GPS_Data_writing(0, NowTimestamp); 
                   // 存储到指定IMU中
                   IMU_Data_writing(0, NowTimestamp);
                   thread1Ready.store(false, std::memory_order::memory_order_release);
     } 
}

void File_init(void)
{
     while (true) {
            while(!thread0Ready.load(std::memory_order::memory_order_acquire)){
                  // 等待 thread0Ready 为 true
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));
                  }
                  File_Index++;
                  //初始H5根组
                  H5::H5File file = H5_init();
                  // 初始化Sensor
                  Camera_init(file);
                  Radar_init(file);
                  Lidar_init(file);
                  GPS_init(file);
                  IMU_init(file);
                  //初始化时间戳
                  TimeStamp_init(file);
                  thread0Ready.store(false, std::memory_order::memory_order_release);
                  std::cout << "Data_"<< std::to_string(File_Index)<<"文件初始化成功"<< std::endl;
     }
}

H5::H5File H5_init(void)
{
    //定义FILE名
    const H5std_string FILE_NAME("Data_"+ std::to_string(File_Index)+".h5");
    // 打开一个现有的HDF5文件，如果不存在则创建一个新文件
    H5::H5File file(FILE_NAME, H5F_ACC_TRUNC); 
    H5::Group rootGroup = file.openGroup("/");
    // 创建属性数据空间
    H5::DataSpace attrSpace(H5S_SCALAR);
    H5::DataType attr_datatype = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE); 
    H5::Attribute attribute_0 = rootGroup.createAttribute("ScenarioID", attr_datatype, attrSpace);
    H5::Attribute attribute_1 = rootGroup.createAttribute("VehicleID", attr_datatype, attrSpace);
    H5::Attribute attribute_2 = rootGroup.createAttribute("Sequence", H5::PredType::NATIVE_INT, attrSpace);
    H5std_string ScenarioID = boost::uuids::to_string(boost::uuids::random_generator()());
    //写入属性数据
    attribute_0.write(attr_datatype, ScenarioID);
    attribute_1.write(attr_datatype, VehicleID);
    attribute_2.write(H5::PredType::NATIVE_INT, &File_Index);
    return file;
}

// 辅助处理数据
std::chrono::system_clock::time_point currentTime;
void TimeStamp_init(H5::H5File file)
{
    // 获取当前时间+采集时间
    currentTime += std::chrono::milliseconds(100 * (timeStamp));
    //第一个文件
    if(File_Index == 0){
    // 获取当前时间
    currentTime = std::chrono::system_clock::now();
    targetTime = currentTime;
    }
    // 时间接口
    std::vector<std::chrono::system_clock::time_point> customTimestamp;
    for (int i = 0; i < timeStamp; i++) {
        customTimestamp.push_back(currentTime + std::chrono::milliseconds(100 * (i + 1))); // 将时间戳设置为当前时间 + 0.1 秒
    }
    // 创建时间数据集
    H5::Group rootGroup = file.openGroup("/");
    H5::DataSpace dataspace(H5S_SCALAR);
    hsize_t dims[] = {timeStamp};
    dataspace.setExtentSimple(1, dims);
    H5::DataType timeType = H5::PredType::NATIVE_INT64; 
    H5::DataSet timeDataset = rootGroup.createDataSet("Timestamps", timeType, dataspace);
    // 将时间戳存储为INT64数组
    std::vector<int64_t> timeData;
    for (const auto& timestamp : customTimestamp) {
         auto duration = timestamp.time_since_epoch();
         timeData.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    } 
    // 写入时间数据到数据集
    timeDataset.write(timeData.data(), timeType);
}

