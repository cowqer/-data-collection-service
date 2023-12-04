#include "GPS.h"

// 输入参数文件名
void GPS_init(H5::H5File& file)
{
    // 创建 'GPS' 分组
    H5::Group GPSGroup = file.createGroup(GPS_GROUP_NAME);
    // 需要存储的GPS个数
    for (int GPSIndex = 0; GPSIndex < GPS_group; GPSIndex++) {
        // 创建数据集
        std::string subDataSet = "GPSData_" + std::to_string(GPSIndex);
        hsize_t dims[2] = {timeStamp,GPS_Entry}; 
        H5::DataSpace dataspace(2, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE; 
        H5::DataSet GPSDataset = GPSGroup.createDataSet(subDataSet, datatype, dataspace);
        // 创建属性数据空间
        H5::DataSpace attrSpace(H5S_SCALAR);
        H5::DataType attr_datatype = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE); 
        H5::Attribute attribute = GPSDataset.createAttribute("ID", attr_datatype, attrSpace);
        std::string ID = GPSID;
        attribute.write(attr_datatype, ID);
    }
}

void GPS_collection(void)
{ 
    // 打开串口
    boost::asio::io_service io;
    boost::asio::serial_port port(io);
    port.open(GPS_Port);  
    port.set_option(boost::asio::serial_port_base::baud_rate(115200)); 
    boost::asio::streambuf buffer;
    std::string line;
    while(true)
    {
    // 读取串口数据
    boost::asio::read_until(port, buffer, '\n');
    std::istream is(&buffer);
    std::getline(is, line);
    if (line.compare(0, 6, "$GNGGA") == 0)
     {
        // 解析 NMEA 语句的逻辑
        std::istringstream ss(line);
        std::string token;
        // 分割字符串
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) 
        {
               tokens.push_back(token);
        }
        GPS_Data[0] = 3101.31451;
        GPS_Data[1] = 12127.56713;
        // 解析字段
        if(tokens[2]!="" && tokens[4]!="" && tokens[3]=="N" && tokens[5]=="E")
        {
                GPS_Data[0] = std::stod(tokens[2]);
                GPS_Data[1] = std::stod(tokens[4]);
        }
        else if(tokens[2]!="" && tokens[4]!="" && tokens[3]=="S" && tokens[5]=="W")
        {
                GPS_Data[0] = -std::stod(tokens[2]);
                GPS_Data[1] = -std::stod(tokens[4]);
        }
    }
  }
}   

// 输入子数据集 "GPSData_x"，并使用时间戳作为第一个维度
void GPS_Data_writing(int GPSIndex, hsize_t currentTimeStamp)
{
    //写入数据
    H5::H5File file("Data_"+ std::to_string(File_Index-1)+".h5", H5F_ACC_RDWR);
    H5::Group GPSGroup = file.openGroup(GPS_GROUP_NAME);
    std::string subDataSet = "GPSData_" + std::to_string(GPSIndex);               
    H5::DataSet GPSDataset = GPSGroup.openDataSet(subDataSet);    
    hsize_t start[2] = {currentTimeStamp, 0};
    hsize_t count[2] = {1,GPS_Entry};
    H5::DataSpace memspace(2, count);
    H5::DataSpace dataspace = GPSDataset.getSpace(); 
    H5::DataType datatype = GPSDataset.getDataType(); 
    // 选择数据集的超集以写入数据
    dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
    GPSDataset.write(GPS_Data.data(), datatype, memspace ,dataspace);

}

