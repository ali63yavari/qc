//
// Created by Arash on 10/18/2019.
//

#ifndef BROKER_QCBROKERFILEREPOSITORY_H
#define BROKER_QCBROKERFILEREPOSITORY_H


#include "QcBrokerRepositoryBase.h"
#include "QcBrokerBinaryMsg.h"

class QcBrokerFileRepository : public QcBrokerRepositoryBase<QcBrokerBinaryMsg>{
public:
    QcBrokerFileRepository(const std::string& topic, const std::string& repo_path)
        :QcBrokerRepositoryBase(topic, repo_path)
    {

    }

    void EnqueueMessage(const QcBrokerBinaryMsg& msg)
    {

    }

    const QcBrokerBinaryMsg& DequeueMessage()
    {

    }

    int PersistedMsgCount()
    {

    }
};


#endif //BROKER_QCBROKERFILEREPOSITORY_H
