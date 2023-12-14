#ifndef _GPS_H_
#define _GPS_H_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <atomic>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string GPS_GROUP_NAME("/GPS");

//系统数据
extern int Writing_Index;
extern hsize_t timeStamp; 
extern hsize_t currentTimestamp; 
extern std::string GPS_Port;
extern std::string GPSID;
extern std::atomic<bool> thread6Ready;

//初始化GPS数据
const int GPS_group = 1; 
const int GPS_Entry= 2; 
extern int GPS_BaudRate;
extern float GPS_Rates;
extern std::vector<double> GPS_Data;

//GPS初始化
void GPS_init(H5::H5File& file);
void GPS_Set(int GPS_BaudRate,float GPS_Rates);

//GPS采集
void GPS_collection(void);

//GPS数据写入
void GPS_Data_writing(int GPSIndex, hsize_t currentTimeStamp);

#endif

