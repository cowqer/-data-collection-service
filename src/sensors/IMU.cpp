#include "IMU.h"

// 输入参数文件名
void IMU_init(H5::H5File& file)
{   
    // 创建 'IMU' 分组
    H5::Group IMUGroup = file.createGroup(IMU_GROUP_NAME);
    // 需要存储的IMU个数
    for (int IMUIndex = 0; IMUIndex < IMU_group; IMUIndex++) {
        // 创建数据集
        std::string subDataSet = "IMUData_" + std::to_string(IMUIndex);
        hsize_t dims[2] = {timeStamp, IMU_Entry}; 
        H5::DataSpace dataspace(2, dims);
        H5::DataType datatype = H5::PredType::NATIVE_FLOAT; 
        H5::DataSet IMUDataset = IMUGroup.createDataSet(subDataSet, datatype, dataspace);
        // 创建属性数据空间
        H5::DataSpace attrSpace(H5S_SCALAR);
        H5::DataType attr_datatype = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE); 
        H5::Attribute attribute = IMUDataset.createAttribute("ID", attr_datatype, attrSpace);
        std::string ID = IMUID;
        attribute.write(attr_datatype, ID);
    }
}

void IMU_collection(void)
{   
    // 打开串口
    boost::asio::io_service io;
    boost::asio::serial_port port(io);
    port.open(IMU_Port); 
    port.set_option(boost::asio::serial_port_base::baud_rate(921600)); 
    while(true)
    {
    //IMU接口
    uint8_t data[1];
    boost::asio::read(port, boost::asio::buffer(data, 1));
    if (data[0] == 0x55) {
        boost::asio::read(port, boost::asio::buffer(data, 1));
        if (data[0] == 0x51){
            uint8_t packet[31];
            boost::asio::read(port, boost::asio::buffer(packet, 31));//读取数据包
            if (packet[10] == 0x52 && packet[21] == 0x53){
                // 在此解析数据包并执行相应的操作
                IMU_Data[0]=static_cast<int16_t>(packet[1] << 8 | packet[0]) / 32768.0 * 16 * 9.8; // 计算加速度
                IMU_Data[1]=static_cast<int16_t>(packet[3] << 8 | packet[2]) / 32768.0 * 16 * 9.8;
                IMU_Data[2]=static_cast<int16_t>(packet[5] << 8 | packet[4]) / 32768.0 * 16 * 9.8;
                IMU_Data[3]=static_cast<int16_t>(packet[12] << 8 | packet[11]) / 32768.0 * 2000 * acos(-1) / 180;  //计算角速度
                IMU_Data[4]=static_cast<int16_t>(packet[14] << 8 | packet[13]) / 32768.0 * 2000 * acos(-1) / 180 ;
                IMU_Data[5]=static_cast<int16_t>(packet[16] << 8 | packet[15]) / 32768.0 * 2000 * acos(-1) / 180 ;        
                IMU_Data[6]=static_cast<int16_t>(packet[23] << 8 | packet[22]) / 32768.0 * 180 ;  //计算欧拉角
                IMU_Data[7]=static_cast<int16_t>(packet[25] << 8 | packet[24]) / 32768.0 * 180 ;
                IMU_Data[8]=static_cast<int16_t>(packet[27] << 8 | packet[26]) / 32768.0 * 180 ;
               }
           }
       }
    }
}

//输入子数据集 "IMU_x"，并使用时间戳作为第一个维度
void  IMU_Data_writing(int IMUIndex,hsize_t currentTimeStamp)
{
    //写入数据
    H5::H5File file("Data_"+ std::to_string(File_Index-1)+".h5", H5F_ACC_RDWR);
    H5::Group IMUGroup = file.openGroup(IMU_GROUP_NAME);
    std::string subDataSet = "IMUData_" + std::to_string(IMUIndex);       
    H5::DataSet IMUDataset = IMUGroup.openDataSet(subDataSet); 
    hsize_t start[2] = {currentTimeStamp, 0};
    hsize_t count[2] = {1, IMU_Entry};
    H5::DataSpace memspace(2, count);
    H5::DataSpace dataspace = IMUDataset.getSpace();
    H5::DataType datatype = IMUDataset.getDataType(); 
    // 选择数据集的超集以写入数据
    dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
    IMUDataset.write(IMU_Data.data(), datatype, memspace, dataspace);
}

