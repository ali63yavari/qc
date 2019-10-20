//
// Created by Arash on 10/18/2019.
//

#ifndef BROKER_ZMQPROXY_H
#define BROKER_ZMQPROXY_H

#include <string>
#include <utility>
#include <functional>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <iterator>

#include "QcBrokerBinaryChannel.h"

class ZmqProxy {
public:
    ZmqProxy(std::string  backend_ip,
            uint16_t backend_port,
            std::string  frontend_ip,
            uint16_t frontend_port,
            std::string  repository_path);

    ~ZmqProxy();
    void Start();
    void Stop();

//    bool Publish(const std::string& topic, const uint8_t* data, uint32_t data_len)
//    {
//        std::unique_lock lock(mutex_);
//        //TODO: Generate new broker channel
//        auto channel = new QcBrokerChannel();
//        if (broker_channels_.insert(std::make_pair(topic, channel)).second == true)
//        {
//
//        }
//    }
//
//    void Subscribe(const std::string& topic, const std::function<void(const uint8_t*, uint16_t)>& callback)
//    {
//
//    }

private:
    bool terminate_zmq_thread_;
    std::string backend_ip_;
    std::string frontend_ip_;
    std::string repository_path_;
    uint16_t backend_port_;
    uint16_t frontend_port_;
    std::unordered_map<std::string, QcBrokerBinaryChannel*> broker_channels_;
    mutable std::shared_mutex mutex_;
    std::thread th_zmq_proxy_;
    void* frontend_socket_;
    void* backend_socket_;
    void* zmq_context_;

    void InitZmq();
    QcBrokerBinaryChannel* InitNewChannel(const std::string& topic, void* backend_socket);
};


#endif //BROKER_ZMQPROXY_H
