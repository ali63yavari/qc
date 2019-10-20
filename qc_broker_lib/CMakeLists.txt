cmake_minimum_required(VERSION 3.15)


project(qc_broker_lib)

set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq CONFIG REQUIRED)

add_library(qc_broker_lib SHARED
        ZmqProxy.cpp ZmqProxy.h
        QcBrokerRepositoryBase.h
        QcBrokerMsgBase.h
        QcBrokerFileRepository.cpp QcBrokerFileRepository.h
        QcBrokerBinaryMsg.h
        QcBrokerChannel.h QcBrokerBinaryChannel.cpp QcBrokerBinaryChannel.h)

target_link_libraries(qc_broker_lib PRIVATE cppzmq cppzmq-static)