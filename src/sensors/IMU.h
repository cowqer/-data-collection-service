#ifndef _IMU_H_
#define _IMU_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <atomic>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string IMU_GROUP_NAME("/IMU");

//系统数据
extern int Writing_Index;
extern hsize_t timeStamp; 
extern hsize_t currentTimestamp; 
extern std::string IMU_Port;
extern std::string IMUID;
extern std::atomic<bool> thread7Ready;

//初始化IMU数据
const int IMU_group = 1; 
const int IMU_Entry= 9; 
extern int IMU_BaudRate;
extern std::vector<float> IMU_Data;

//IMU初始化
void IMU_init(H5::H5File& file);

//IMU采集
void IMU_collection(void);

//IMU数据写入
void IMU_Data_writing(int IMUIndex,hsize_t currentTimeStamp);

#endif

