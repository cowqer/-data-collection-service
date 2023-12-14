#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <x264.h>
#include "H5Cpp.h"

//根文件下的相对路径
const H5std_string CAMERA_GROUP_NAME("/Camera");

//系统数据
extern int System_Rates;
extern int Writing_Index;
extern hsize_t timeStamp;
extern hsize_t currentTimestamp;
extern int Camera_Port;
extern std::string CameraID;
extern std::atomic<bool> thread3Ready;

//初始化Camera数据
const int Camera_group = 1; 
extern int image_Width; 
extern int image_Height;
extern int image_Rates;
extern int image_Compress_Flag;
const int channels = 3;
extern std::vector<cv::Mat> CameraData;
extern std::vector<uint8_t> imageData;

//Camera初始化
void Camera_init(H5::H5File& file);
    
//Camera数据采集
void Camera_collection(void);

//Camera数据写入
void Camera_Data_writing(int CameraIndex, hsize_t currentTimeStamp,int Writing_Index);

//Camera数据压缩
void compressData(std::vector<cv::Mat>& frames, std::vector<uchar>& output);

#endif

