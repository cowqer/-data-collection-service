#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include "mqtt/async_client.h"


const std::string broker_address = "fusion-dc.net";
//  const std::string broker_address = "localhost";
const int port = 1883;
int id_nums = 0;
void send_hdf5_file(const std::string &filename, const std::string &topic, mqtt::async_client &client) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }
    
    // 获取 HDF5 文件的大小
    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 读取 HDF5 文件内容为字节数组
    std::vector<char> file_contents(file_size);
    file.read(file_contents.data(), file_size);

    // auto src = (char*) new char[file_contents.size()];
    // std::copy(file_contents.begin(), file_contents.end(), src);
    mqtt::buffer_view<char> bv(file_contents.data(), file_contents.size());

    // Debugging: Print the size of the file_contents vector before creating the MQTT message
    std::cout << "File contents size: " << file_contents.size() << " bytes" << std::endl;

    // 创建 MQTT 消息，将字节数组作为 payload
    mqtt::message_ptr pubmsg = mqtt::make_message(topic, bv.to_string(), 1, false);
    pubmsg->set_qos(1);

    // Publishing MQTT message
    std::cout << "Publishing message..." << std::endl;
    auto tok = client.publish(pubmsg);
    try {
        tok->wait(); // Wait for the message to be delivered
        std::cout << "Message published successfully" << std::endl;
        
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: Message publish failed - " << exc.what() << std::endl;
    }

    std::cout << "HDF5 file '" << filename << "' (" << file_size << " bytes) sent to topic '" << topic << "'" << std::endl;
}

int main() {
    mqtt::async_client client(broker_address, "");
    try {
        mqtt::connect_options conn_opts;
        conn_opts.set_clean_session(true);
        conn_opts.set_connect_timeout(std::chrono::seconds(5));

        auto connect_token = client.connect(conn_opts);
        connect_token->wait();

        if (client.is_connected()) {
            std::cerr << "Connected to the MQTT broker successfully." << std::endl;
        } else {
            std::cerr << "Failed to connect to the MQTT broker." << std::endl;
            return 1;
        }
        const std::string topic = "upload";//topic name
        
        while (true) {
            send_hdf5_file("Data.h5", topic, client);
            id_nums++;
            std::cout << "File " << id_nums << " sent, press Q to pause, E to resume" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(200));
        }
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}

