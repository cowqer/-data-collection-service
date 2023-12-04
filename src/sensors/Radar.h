#ifndef _RADAR_H_
#define _RADAR_H_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string RADAR_GROUP_NAME("/Radar");

//系统数据
extern int File_Index;
extern hsize_t timeStamp; 
extern std::string Radar_Port; 
extern std::string RadarID;

//初始化Radar数据
const int Radar_group = 1; 
const int Radar_DataWidth = 420; 
const int Radar_Entry= 6; 

//Radar初始化
void Radar_init(H5::H5File& file);

//Radar采集
void Radar_collection(void);

//Radar数据写入
void Radar_Data_writing(int RadarIndex, hsize_t currentTimeStamp);

#endif

