#ifndef _IMU_H_
#define _IMU_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string IMU_GROUP_NAME("/IMU");

//系统数据
extern int File_Index;
extern hsize_t timeStamp; 
extern std::string IMU_Port;
extern std::string IMUID;

//初始化IMU数据
const int IMU_group = 1; 
const int IMU_Entry= 9; 
extern std::vector<float> IMU_Data;

//IMU初始化
void IMU_init(H5::H5File& file);

//IMU采集
void IMU_collection(void);

//IMU数据写入
void IMU_Data_writing(int IMUIndex,hsize_t currentTimeStamp);

#endif

