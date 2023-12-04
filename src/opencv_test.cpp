#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

int main() {
    // 打开摄像头
    cv::VideoCapture cap(0);

    // 检查摄像头是否成功打开
    if (!cap.isOpened()) {
        std::cerr << "Error: Couldn't open the camera." << std::endl;
        return -1;
    }

    // 读取图像
    cv::Mat frame;
    cap >> frame;

    // 这里可以对图像进行处理，或者显示图像等操作

    // 释放摄像头资源
    cap.release();

    return 0;
}

