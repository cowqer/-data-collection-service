#ifndef _LIDAR_H_
#define _LIDAR_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string LIDAR_GROUP_NAME("/Lidar");

//系统数据
extern int File_Index;
extern hsize_t timeStamp; 
extern std::string Lidar_Port; 
extern std::string LidarID;

//初始化Lidar数据
const int Lidar_group = 1;
extern int Lidar_line;
extern int Lidar_DataWidth;
const int Lidar_Entry= 6; 
extern std::vector<double> Lidar_Data;

//Lidar初始化
void Lidar_init(H5::H5File& file);

//Lidar数据采集
void Lidar_collection(void);

//Lidar数据写入
void Lidar_Data_writing(int LidarIndex, hsize_t currentTimeStamp);

#endif

