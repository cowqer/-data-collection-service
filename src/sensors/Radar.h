#ifndef _RADAR_H_
#define _RADAR_H_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <atomic>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string RADAR_GROUP_NAME("/Radar");

//系统数据
extern int Writing_Index;
extern hsize_t timeStamp; 
extern hsize_t currentTimestamp; 
extern std::string Radar_Port; 
extern std::string RadarID;
extern std::atomic<bool> thread4Ready;

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

