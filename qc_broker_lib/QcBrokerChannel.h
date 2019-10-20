//
// Created by Arash on 10/20/2019.
//

#ifndef BROKER_QCBROKERCHANNEL_H
#define BROKER_QCBROKERCHANNEL_H


#include <shared_mutex>
#include <queue>
#include <thread>

template <class MsgType>
class QcBrokerChannel {
public:
    QcBrokerChannel(const std::string& topic, const std::string& repo_path, void* backend_socket):
    topic_(std::move(topic)),
    repo_path_(repo_path),
    subscriber_counter_(0),
    backend_socket_(backend_socket),
    terminate_channel_(false)
    {
        th_channel_ = std::thread([this](){
            while(terminate_channel_)
            {
                PassNextMsgToBackend();
            }
        });
    }

    ~QcBrokerChannel()
    {

    }

protected:
    mutable std::shared_mutex mutex_;
    std::string topic_;
    std::string repo_path_;
    std::queue<MsgType*> msg_q_;
    void* backend_socket_;
    bool terminate_channel_;
    std::thread th_channel_;

private:
    int subscriber_counter_;

public:
    virtual void EnqueueMsg(const MsgType& msg) = 0;
    virtual void EnqueueMsg(char* raw_data, int data_len) = 0;
    virtual void PassNextMsgToBackend() = 0;

    void IncreaseSubscriber()
    {
        subscriber_counter_++;
    }

    void DecreaseSubscriber()
    {
        subscriber_counter_--;
    }

    bool HasSubscriber()
    {
        return subscriber_counter_ > 0;
    }

};


#endif //BROKER_QCBROKERCHANNEL_H
