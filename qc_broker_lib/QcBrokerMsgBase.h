//
// Created by Arash on 10/18/2019.
//

#ifndef BROKER_QCBROKERMSGBASE_H
#define BROKER_QCBROKERMSGBASE_H

#include <cstdint>

struct QcBrokerMsgBase{
public:
    virtual uint32_t GetSize() = 0;
    virtual void GetBytes(uint8_t*& data) = 0;
    virtual void SetBytes(const uint8_t* data, uint32_t data_len, uint32_t& start_point) = 0;
};

#endif //BROKER_QCBROKERMSGBASE_H
