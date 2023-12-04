#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include "mqtt/async_client.h"

extern std::atomic<bool> thread2Ready;

void send_hdf5_file(const std::string &filename, const std::string &topic, mqtt::async_client &client);

void run_mqtt_client(const std::string &broker_address);

#endif // MQTT_PUBLISHER_H

