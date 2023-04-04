#pragma once
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>

class Backstage_task
{
public:
    static Backstage_task* get() noexcept
    {
        return _backstage_task;
    }

    void set_run_sleep(int argv_sleep_seconds);
    bool set_run_func(std::function<void()> argv_run_func);
    bool set_local_path(std::string argv_local_path);

    void start();
    void stop();

private:
    Backstage_task() { _run.store(true); _run_sleep_seconds = 60; };
    Backstage_task(const Backstage_task& argv) = delete;
    Backstage_task(Backstage_task&& argv) = delete;
    Backstage_task& operator = (const Backstage_task& argv) = delete;
    Backstage_task& operator = (Backstage_task&& argv) = delete;

    ~Backstage_task() { /*_run.store(false); _cond.notify_all(); _run_task->join(); _stop_task->join();*/ };

private:
    static Backstage_task* _backstage_task;

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    int _run_sleep_seconds;
    std::atomic<bool> _run;
    //std::thread* _run_task;
    std::unique_ptr<std::thread> _run_task;
    //std::thread _stop_task;
    std::unique_ptr<std::thread> _stop_task;

    std::function<void()> _run_task_func;

    std::string _local_path;
};




