#ifndef SAFEBUFFER_H
#define SAFEBUFFER_H

#include <mutex>
#include <vector>
#include <string>
#include <condition_variable>
#include <unordered_map>

struct SafeBuffer
{
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::vector<std::string>> safe_vector;
    bool is_done = false;
};

struct LogDateTime
{
    std::string date;
    std::string time;
};

struct logInfo
{
    int count = 0;
    std::unordered_map <std::string, int> logCommentCount;
    std::unordered_map <std::string, std::vector<LogDateTime>> logCommentDateTime;
};

struct SafeLogMap
{
    std::mutex mtx;
    std::condition_variable cv;
    bool is_done = false;
    std::unordered_map<std::string, logInfo> data;
};

#endif // SAFEBUFFER_H
