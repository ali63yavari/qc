//
// Created by Arash on 10/18/2019.
//

#ifndef BROKER_QCBROKERBINARYMSG_H
#define BROKER_QCBROKERBINARYMSG_H


#include <string>
#include "QcBrokerMsgBase.h"

struct QcBrokerBinaryMsg : QcBrokerMsgBase{
    std::string topic;
    uint64_t time_stamp;
    uint32_t raw_data_len;
    char* raw_data;

    QcBrokerBinaryMsg():
    topic(""),
    time_stamp(0),
    raw_data_len(0),
    raw_data(nullptr)
    {
    }

    uint32_t GetSize()
    {
        return
            sizeof(uint32_t) +
            topic.length() +
            sizeof(time_stamp) +
            sizeof(raw_data_len) +
            raw_data_len;
    }

    void GetBytes(uint8_t*& data) final
    {
        data = new uint8_t[GetSize()];
        int pos = 0;

        int sz = topic.length();
        memcpy(data + pos, topic.c_str(), sz);
        pos += sz;

        sz = sizeof(time_stamp);
        memcpy(data + pos, (uint8_t*)(time_stamp), sz);
        pos += sz;

        sz = sizeof(raw_data_len);
        memcpy(data + pos, (uint8_t*)(raw_data_len), sz);
        pos += sz;

        sz = raw_data_len;
        memcpy(data + pos, raw_data, sz);
        pos += sz;
    }

    void SetBytes(const uint8_t* data, uint32_t data_len, uint32_t& start_point) final
    {
        uint32_t topic_len = 0;
        memcpy((uint8_t*)topic_len, data + start_point, sizeof(uint32_t));
        start_point += sizeof(uint32_t);

        if (topic_len > 0)
        {
            topic.append((const char*)(data + start_point), topic_len);
            start_point += topic_len;
        }

        memcpy((uint8_t*)time_stamp, data + start_point, sizeof(time_stamp));
        start_point += sizeof(time_stamp);

        memcpy((uint8_t*)raw_data_len, data + start_point, sizeof(raw_data_len));
        start_point += sizeof(raw_data_len);

        if (raw_data_len>0)
        {
            raw_data = new char[raw_data_len];
            memcpy(raw_data, data + start_point, raw_data_len);
            start_point += raw_data_len;
        }
    }

};


#endif //BROKER_QCBROKERBINARYMSG_H
