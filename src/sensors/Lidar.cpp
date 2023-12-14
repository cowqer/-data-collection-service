#include "Lidar.h"

#define PI 3.14159265359
// 输入参数文件名
void Lidar_init(H5::H5File& file) 
{
    // 创建 'Lidar' 分组
    H5::Group LidarGroup = file.createGroup(LIDAR_GROUP_NAME);
    // 需要存储的Lidar个数
    for (int LidarIndex = 0; LidarIndex < Lidar_group; LidarIndex++) {
        // 创建数据集
        std::string subDataSet = "LidarData_" + std::to_string(LidarIndex);
        hsize_t dims[4] = {timeStamp , Lidar_line , Lidar_DataWidth , Lidar_Entry}; 
        H5::DataSpace dataspace(4, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE; 
        H5::DataSet LidarDataset = LidarGroup.createDataSet(subDataSet, datatype, dataspace);  
        // 创建属性数据空间
        H5::DataSpace attrSpace(H5S_SCALAR);
        H5::DataType attr_datatype = H5::StrType( H5::PredType::C_S1, H5T_VARIABLE); 
        H5::Attribute attribute = LidarDataset.createAttribute("ID", attr_datatype, attrSpace);
        H5std_string ID= LidarID;
        attribute.write(attr_datatype, ID);
    }
}

// 辅助处理数据
int y = 0;
int i = 0;
void Lidar_collection(void)
{
    // 打开串口
    boost::asio::io_service io; 
    boost::asio::serial_port port(io);
    port.open(Lidar_Port);
    port.set_option(boost::asio::serial_port_base::baud_rate(Lidar_BaudRate)); 
    while(true)
    {
     while (!thread5Ready.load(std::memory_order::memory_order_acquire)) {
            // 等待 thread5Ready 为 true
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            //Lidar接口
            uint8_t data[1];
            boost::asio::read(port, boost::asio::buffer(data, 1));
            if (data[0] == 0xA5) {
                boost::asio::read(port, boost::asio::buffer(data, 1));
                if (data[0] == 0x5A) {
                    boost::asio::read(port, boost::asio::buffer(data, 1));
                    if (data[0] == 0x3A) {
                        uint8_t packet[55];
                        boost::asio::read(port, boost::asio::buffer(packet, 55));//读取数据包
                        // 在此解析数据包并执行相应的操作
                        double start_angle = static_cast<double>((packet[2] << 8) | packet[3]) / 100.0;  // 计算起始角度
                        double end_angle = static_cast<double>((packet[52] << 8) | packet[53]) / 100.0;  // 计算结束角度
                        // 辅助处理数据
                        int j = 0;
                        // 遍历数据包中的距离和光强数据
                        for (int x = 4; x < 50; x += 3) {
                        int offset = y * Lidar_DataWidth * Lidar_Entry + i * Lidar_Entry;
                        Lidar_Data[offset+0] = fmod(start_angle + (fmod(end_angle + 360 -start_angle, 360.0)) * j / 15, 360.0); // 计算角度
                        Lidar_Data[offset+1] = packet[x] << 8 | packet[x + 1];  // 计算距离
                        Lidar_Data[offset+2] = (Lidar_Data[offset+1]) * cos(Lidar_Data[offset]*PI/180); // 计算x
                        Lidar_Data[offset+3] = (Lidar_Data[offset+1]) * sin(Lidar_Data[offset]*PI/180);  // 计算y
                        Lidar_Data[offset+4] = y;  // 计算z
                        Lidar_Data[offset+5] = packet[x + 2];  // 获取光强                       
                        i++;
                        if (i == Lidar_DataWidth) //采集一圈后停
                        {
                            i=0;
                            y++;
                            if(y == Lidar_line)
                              {
                               y = 0;
                              }
                        }
                    j++;
                   }
               }
           }
       }
       Lidar_Data_writing(0, currentTimestamp);
       thread5Ready.store(false, std::memory_order::memory_order_release);
   }
}

// 输入子数据集 "LidarData_x"，并使用时间戳作为第一个维度
void  Lidar_Data_writing(int LidarIndex, hsize_t currentTimeStamp)
{
    //写入数据
    H5::H5File file("Data_"+ std::to_string(Writing_Index)+".h5", H5F_ACC_RDWR);
    H5::Group LidarGroup = file.openGroup(LIDAR_GROUP_NAME);
    std::string subDataSet = "LidarData_" + std::to_string(LidarIndex);              
    H5::DataSet LidarDataset = LidarGroup.openDataSet(subDataSet);  
    hsize_t start[4] = {currentTimeStamp, 0, 0, 0};
    hsize_t count[4] = {1, Lidar_line , Lidar_DataWidth , Lidar_Entry};
    H5::DataSpace memspace(4, count);
    H5::DataSpace dataspace = LidarDataset.getSpace();
    H5::DataType datatype = LidarDataset.getDataType(); 
    // 选择数据集的超集以写入数据
    dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
    LidarDataset.write(Lidar_Data.data(), datatype, memspace, dataspace);
}

