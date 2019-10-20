//
// Created by Arash on 10/18/2019.
//

#include "ZmqProxy.h"

#include <cppzmq.h>
#include <sstream>


ZmqProxy::ZmqProxy(std::string backend_ip,
        uint16_t backend_port,
        std::string frontend_ip,
        uint16_t frontend_port,
        std::string repository_path) :
        backend_ip_(std::move(backend_ip)),
        frontend_ip_(std::move(frontend_ip)),
        backend_port_(backend_port),
        frontend_port_(frontend_port),
        repository_path_(std::move(repository_path)),
        zmq_context_(nullptr),
        frontend_socket_(nullptr),
        backend_socket_(nullptr),
        terminate_zmq_thread_(false)
{
}

ZmqProxy::~ZmqProxy() {
}

void ZmqProxy::Start() {
    //init zmq proxy
    InitZmq();
}

void ZmqProxy::Stop() {

}

void ZmqProxy::InitZmq() {
    zmq_context_ = zmq_ctx_new ();
    frontend_socket_ = zmq_socket (zmq_context_, ZMQ_SUB);
    std::ostringstream stringUrlStream;
    stringUrlStream << "tcp://"<<frontend_ip_<<":"<<frontend_port_;
    std::string stringUrl = stringUrlStream.str();
    zmq_bind (frontend_socket_, stringUrl.c_str());

    backend_socket_ = zmq_socket (zmq_context_, ZMQ_XPUB);
    stringUrlStream.clear();
    stringUrlStream << "tcp://"<<backend_ip_<<":"<<backend_port_;
    stringUrl = stringUrlStream.str();
    zmq_bind (backend_socket_, stringUrl.c_str());

    //  Subscribe to every single topic from publisher
    zmq_setsockopt(frontend_socket_, ZMQ_SUBSCRIBE, "", 0);

    //main proxy thread
    th_zmq_proxy_ = std::thread([this](){
        zmq_pollitem_t items [] = {
                { frontend_socket_, 0, ZMQ_POLLIN, 0 },
                { backend_socket_,  0, ZMQ_POLLIN, 0 }
        };

        while(!terminate_zmq_thread_)
        {
            if (zmq_poll (items, 2, 1000 * ZMQ_POLL_MSEC) == -1)
            {
                continue;
            }

            //  When we get a new subscription, we pull data from the cache:
            if (items[1].revents & ZMQ_POLLIN)
            {
                char* c_topic = zstr_recv(frontend_socket_);
                if (!strlen(c_topic))
                {
                    delete[] c_topic;
                    continue;
                }

                std::string topic = std::string(c_topic + 1);
                if (topic.empty())
                {
                    delete[] c_topic;
                    continue;
                }

                //  Event is one byte 0=unsub or 1=sub, followed by topic
                uint8_t event = c_topic[0];
                auto elm = broker_channels_.find(topic);
                if (elm == broker_channels_.end())
                {
                    //add new topic
                    broker_channels_.insert( make_pair(topic, InitNewChannel(topic, backend_socket_)));
                }

                auto channel = broker_channels_[topic];

                if (event == 0)
                {
                    channel->DecreaseSubscriber();
                }
                else
                {
                    channel->IncreaseSubscriber();
                }


                delete[] c_topic;
            }

            //  Any new topic data we cache and then forward
            if (items [0].revents & ZMQ_POLLIN)
            {
                char* c_topic = zstr_recv(frontend_socket_);
                std::string topic = std::string(c_topic);
                char* raw_data = (zstr_recv (frontend_socket_));
                if (topic.empty())
                {
                    delete [] raw_data;
                    delete [] c_topic;
                    continue;
                }

                auto elm = broker_channels_.find(topic);
                if (elm == broker_channels_.end())
                {
                    //add new topic
                    broker_channels_.insert( make_pair(topic, InitNewChannel(topic, backend_socket_)));
                }

                auto channel = broker_channels_[topic];
                channel->EnqueueMsg(raw_data, strlen(raw_data));

                delete [] raw_data;
                delete [] c_topic;
            }
        }
    });
}

QcBrokerBinaryChannel *ZmqProxy::InitNewChannel(const std::string &topic, void *backend_socket) {
    auto channel = new QcBrokerBinaryChannel(topic);
    return channel;
}

//void ZmqProxy::PassMsgToBackend(QcBrokerBinaryMsg* msg) {
//    zstr_sendm(backend_socket_, msg->topic.c_str());
//    zstr_send(backend_socket_, std::string(msg->raw_data, msg->raw_data_len).c_str());
//
//    delete msg;
//}
