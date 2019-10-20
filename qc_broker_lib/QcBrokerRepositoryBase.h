//
// Created by Arash on 10/18/2019.
//

#ifndef BROKER_QCBROKERREPOSITORYBASE_H
#define BROKER_QCBROKERREPOSITORYBASE_H


#include <string>
#include <filesystem>
#include "QcBrokerMsgBase.h"

namespace fs = std::experimental::filesystem;

#pragma pack(push, 1)
struct RepositoryHeaderData
{
    char topic_name[255];
    uint32_t msg_count;
    uint32_t next_writeable_pos;
    uint32_t next_readable_pos;
};
#pragma pack(pop)

template <class MsgType>
class QcBrokerRepositoryBase {
public:
    QcBrokerRepositoryBase(const std::string& topic, const std::string& repo_path):
    repository_topic_(std::move(topic)),
    repo_path_(std::move(repo_path))
    {
        fs::path path(repo_path);
        if (!fs::exists(path))
        {
            throw std::exception();
        }

        InitRepositoryHandles();
    }

    virtual void EnqueueMessage(const MsgType& msg) = 0;
    virtual const MsgType& DequeueMessage() = 0;
    virtual int PersistedMsgCount() = 0;

protected:
    std::string repository_topic_;
    std::string repo_path_;
    std::ofstream* repo_write_handle;
    std::ifstream* repo_read_handle;
    std::string repo_file_name_;

    void Enqueue(char* raw_data, int template_size)
    {
        //read header
        auto current_read_pos = repo_read_handle->tellg();
        repo_read_handle->seekg(0, repo_read_handle->beg());

        RepositoryHeaderData header;
        memset((char*)&header, 0, sizeof(header));
        repo_read_handle->read((char*)&header, sizeof(header));

        repo_read_handle->seekg(current_read_pos, repo_read_handle->beg());

        //write data to repo
        repo_write_handle->seekp()

    }

private:
    void InitRepositoryHandles()
    {
        fs::path p(repo_path_);
        std::string file_name = "repo_" + topic_ + ".dat";
        p.append(file_name);

        repo_file_name_ = p.string();

        repo_write_handle = new std::ofstream(repo_file_name_, ios::binary | ios::out);
        if (!repo_write_handle)
        {
            throw std::exception();
        }

        repo_read_handle = new std::ifstream(repo_file_name_, ios::binary | ios::in);
        if (!repo_read_handle)
        {
            throw std::exception();
        }
    }
};


#endif //BROKER_QCBROKERREPOSITORYBASE_H
