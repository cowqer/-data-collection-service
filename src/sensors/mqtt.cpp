#include "mqtt.h"

int id_nums = 0;
void send_hdf5_file(const std::string &filename, const std::string &topic, mqtt::async_client &client) {
    std::ifstream file(filename, std::ios::binary);
    
    if (!file) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }
    
    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> file_contents(file_size);
    file.read(file_contents.data(), file_size);
    
    // auto src = (char*) new char[file_contents.size()];
    // std::copy(file_contents.begin(), file_contents.end(), src);
    mqtt::buffer_view<char> bv(file_contents.data(), file_contents.size());

    mqtt::message_ptr pubmsg = mqtt::make_message(topic, bv.to_string(), 1, false);
    pubmsg->set_qos(1);

    auto tok = client.publish(pubmsg);
    try {
        tok->wait(); // Wait for the message to be delivered
        
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error:" << "Data_"<<id_nums<<".h5"<<"publish failed" << std::endl;
    }

    std::cout << "HDF5 file '" << filename << "' (" << file_size << " bytes) sent to topic '" << topic << "' success" << std::endl;
}

void run_mqtt_client(const std::string &broker_address) {
    mqtt::async_client client(broker_address, "");
    // 将错误信息写入文件
    std::ofstream errorFile("error.log");
    std::streambuf *cerrBuffer = std::cerr.rdbuf();
    std::cerr.rdbuf(errorFile.rdbuf());
    try {
        mqtt::connect_options conn_opts;
        conn_opts.set_clean_session(true);
        conn_opts.set_connect_timeout(std::chrono::seconds(5));

        auto connect_token = client.connect(conn_opts);
        connect_token->wait();

        if (client.is_connected()) {
            std::cout << "Connected to the MQTT broker successfully." << std::endl;
        } 
        else {
            std::cout << "Failed to connect to the MQTT broker." << std::endl;
            return;
        }

        const std::string topic = "upload";

        while (true) {
            while(!thread2Ready.load(std::memory_order::memory_order_acquire)){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            if(image_Compress_Flag==1)
            {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
            send_hdf5_file("Data_"+ std::to_string(id_nums)+".h5", topic, client);
            id_nums++;      
            thread2Ready.store(false, std::memory_order::memory_order_release);
        }
    } catch (const mqtt::exception &exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
    }
}

