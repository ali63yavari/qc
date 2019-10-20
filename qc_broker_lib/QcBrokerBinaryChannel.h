//
// Created by a.yavari on 10/20/2019.
//

#ifndef BROKER_QCBROKERBINARYCHANNEL_H
#define BROKER_QCBROKERBINARYCHANNEL_H


#include "QcBrokerBinaryMsg.h"
#include "QcBrokerChannel.h"
#include "QcBrokerFileRepository.h"

class QcBrokerBinaryChannel : public QcBrokerChannel<QcBrokerBinaryMsg>{
public:
    QcBrokerBinaryChannel(const std::string& topic, const std::string& repo_path, void* backend_socket) :
        QcBrokerChannel(topic, repo_path, backend_socket)
    {
        file_repository_ = new QcBrokerFileRepository(topic, repo_path);
    }

    void EnqueueMsg(const QcBrokerBinaryMsg& msg)
    {

    }

    void EnqueueMsg(char* raw_data, int data_len)
    {

    }

    void PassNextMsgToBackend()
    {

    }

private:
    QcBrokerFileRepository* file_repository_;

};


#endif //BROKER_QCBROKERBINARYCHANNEL_H
