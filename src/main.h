#include "sensors/Camera.h"
#include "sensors/Lidar.h"
#include "sensors/Radar.h"
#include "sensors/IMU.h"
#include "sensors/GPS.h"
#include "sensors/mqtt.h"
#include "mqtt/async_client.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_io.hpp> 
#include <boost/uuid/uuid_generators.hpp>
#include <atomic>

//打开JSON配置文件 
std::ifstream config_file("../config.json");
nlohmann::json config;

//系统参数
int File_Index = 0;
hsize_t timeStamp; 
hsize_t currentTimestamp = 0; 
const int numThreads = 8;
std::atomic<bool> thread0Ready(false);
std::atomic<bool> thread1Ready(false);
std::atomic<bool> thread2Ready(false);
std::chrono::system_clock::time_point targetTime;

//串口参数
std::string IMU_Port;
std::string GPS_Port;
std::string Lidar_Port;
std::string Radar_Port;
int Camera_Port;

//ID参数
std::string CameraID;
std::string IMUID;
std::string GPSID;
std::string RadarID;
std::string LidarID;
std::string VehicleID;

//Sensor参数
int image_Width;
int image_Height;
int Lidar_line;
int Lidar_DataWidth;

//准备数据
std::vector<uint8_t> imageData;
std::vector<double> Lidar_Data;
std::vector<double> GPS_Data;
std::vector<float> IMU_Data;

//读取Json文件
void Json_init(void);

//存储数据到指定的Sersor中
void Data_writing(void);

//初始化File
void File_init(void);

//初始H5根组
H5::H5File H5_init(void);

//初始化时间戳
void TimeStamp_init(H5::H5File file);

