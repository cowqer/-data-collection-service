#include "Radar.h"

// 输入参数文件名
void Radar_init(H5::H5File& file) 
{
    // 创建 'Radar' 分组
    H5::Group RadarGroup = file.createGroup(RADAR_GROUP_NAME);
    /*
    // 需要存储的Radar个数
    for (int RadarIndex = 0; RadarIndex < Radar_group; RadarIndex++) {
        // 创建数据集
        std::string subDataSet = "RadarData_" + std::to_string(RadarIndex);
        hsize_t dims[3] = {timeStamp , Radar_DataWidth , Radar_Entry}; 
        H5::DataSpace dataspace(3, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE; 
        H5::DataSet RadarDataset = RadarGroup.createDataSet(subDataSet, datatype, dataspace);
        // 创建一个属性数据空间
        H5::DataSpace attrSpace(H5S_SCALAR);
        H5::DataType attr_datatype = H5::StrType(H5T_C_S1, H5T_VARIABLE);  
        H5::Attribute attribute = RadarDataset.createAttribute("ID", attr_datatype, attrSpace);
        H5std_string ID = RadarID;
        attribute.write(attr_datatype, ID);  
    }
    */
}

// 输入子数据集 "RadarData_x"，并使用时间戳作为第一个维度
void Radar_collection(void)
{
     while(true)
     {
           while (!thread4Ready.load(std::memory_order::memory_order_acquire)) {
                  // 等待 thread4Ready 为 true
                  std::this_thread::sleep_for(std::chrono::milliseconds(1));
                  }
                  Radar_Data_writing(0, currentTimestamp);
      thread4Ready.store(false, std::memory_order::memory_order_release);
     }
}

void Radar_Data_writing(int RadarIndex, hsize_t currentTimeStamp)
{
    //写入数据
    H5::H5File file("Data_"+ std::to_string(Writing_Index)+".h5", H5F_ACC_RDWR);
    H5::Group RadarGroup = file.openGroup(RADAR_GROUP_NAME);
    /*
    std::string subDataSet = "RadarData_" + std::to_string(RadarIndex);       
    H5::DataSet RadarDataset = RadarGroup.openDataSet(subDataSet); 
    hsize_t start[2] = {currentTimeStamp, 0};
    hsize_t count[2] = {1, Radar_Entry};
    H5::DataSpace memspace(2, count);
    H5::DataSpace dataspace = ImuDataset.getSpace();
    H5::DataType datatype = muDataset.getDataType(); 
    // 选择数据集的超集以写入数据
    dataspace.selectHyperslab(H5S_SELECT_SET, count, start);
    RadarDataset.write(Radar_Data.data(), datatype, memspace, dataspace);
    */
}

