#include "mqtt/async_client.h"
#include <iostream>
#include <fstream>

const std::string broker_address = "fusion-dc.net";
const int port = 1883;
int rece_file_count = 0;

// MQTT连接回调
class callback : public virtual mqtt::callback {
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "received" << std::endl;
        rece_file_count++;
        std::string topic = msg->get_topic();
        std::string file_id = topic.substr(topic.find_last_of("/") + 1);
        std::string filename = "received_file_" + file_id + ".h5";

        // 获取消息内容和大小
        const char* payload = static_cast<const char*>(msg->get_payload().data());
        std::size_t payload_size = msg->get_payload().size();

        // 动态分配足够的空间来保存消息内容
        std::unique_ptr<char[]> file_contents(new char[payload_size]);
        std::copy(payload, payload + payload_size, file_contents.get());

        // 将消息内容保存到文件
        std::ofstream file(filename, std::ios::binary);
        file.write(file_contents.get(), payload_size);
        file.close();

        std::cout << "Received file with ID " << file_id << " saved as " << filename << std::endl;

        // 输出接收到的文件大小
        std::cout << "Received file size: " << payload_size << " bytes" << std::endl;
    }
};


int main() {
    mqtt::async_client client(broker_address, "", port);
    mqtt::connect_options conn_opts;
    conn_opts.set_clean_session(true);

    callback cb;
    client.set_callback(cb);

    try {
        mqtt::token_ptr conntok = client.connect(conn_opts);
        conntok->wait();

        // 订阅主题
        std::string topic = "upload";
        client.subscribe(topic, 0);

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Received " << rece_file_count << " files" << std::endl;
        }
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}

