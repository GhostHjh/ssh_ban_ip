#include "Backstage_task.h"

Backstage_task* Backstage_task::_backstage_task = new Backstage_task();


void Backstage_task::set_run_sleep(int argv_sleep_seconds)
{
    _run_sleep_seconds = argv_sleep_seconds;
}

bool Backstage_task::set_run_func(std::function<void()> argv_run_func)
{
    _run_task_func = argv_run_func;

    return static_cast<bool>(_run_task_func);
}

bool Backstage_task::set_local_path(std::string argv_local_path)
{
    if (argv_local_path.size() == 0)
    {
        std::cout << "\n不能给本地套接字路径设置空字符串\n";
        return false;
    }

    if (argv_local_path.size() > 108)
    {
        std::cout << "\n本地套接字路径最大108个字符\n";
        return false;
    }
    
    _local_path = argv_local_path;

    return _local_path.size() == 0 ? false : true;

}

void Backstage_task::start()
{
    if (static_cast<bool>(_run_task_func) == false)
    {
        std::cout << "\n未设置要运行的函数\n";
        return;
    }
    stop();

    _run_task = std::make_unique<std::thread>([this]()
    {
        for (; this->_run.load(); )
        {
            std::unique_lock<std::mutex> _lock(this->_mutex);
            this->_cond.wait_for(_lock, std::chrono::seconds(this->_run_sleep_seconds), [this](){ return !this->_run.load(); });

            if (this->_run.load() == false)
                return;

            _run_task_func();
        }
    });

    _stop_task = std::make_unique<std::thread>([this]()
    {
        if (_local_path.size() == 0)
        {
            _run.store(false);
            _cond.notify_all();
            std::cout << "\n未设置本地套接字路径\n";
            return;
        }
        unlink(_local_path.c_str());
        int local_socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

        sockaddr_un local_socketaddr_un;
        local_socketaddr_un.sun_family = AF_LOCAL;

        for (int i = 0; i < 108 && i < _local_path.size(); i++)
            local_socketaddr_un.sun_path[i] = _local_path[i];

        if (local_socket_fd >= 1 && bind(local_socket_fd, reinterpret_cast<sockaddr*>(&local_socketaddr_un), sizeof(local_socketaddr_un)) != -1 && listen(local_socket_fd, 1) != -1)
        {
            std::cout << "\n运行中,开始等待停止指令\n";
            int client_local_socket_fd = accept(local_socket_fd, nullptr, nullptr);
            char* buff = new char[1024]{};
            int read_size = read(client_local_socket_fd, buff, 1024);
            delete[](buff);

            if (read_size > 0)
            {
                _run.store(false);
                _cond.notify_all();
                std::cout << "\n接收到停止指令\n";
            }
            else 
                std::cout << "\n停止指令接收失败\n";

            close(client_local_socket_fd);
        }
        else 
        {
            if (local_socket_fd <= 0)
                std::cout << "error local_socket\n";
            else if (bind(local_socket_fd, reinterpret_cast<sockaddr*>(&local_socketaddr_un), sizeof(local_socketaddr_un)) == -1)
                std::cout << "error bind\n";
            else if (listen(local_socket_fd, 1) == -1)
                std::cout << "error listen\n";
            std::cout << "\n运行失败,无法等待停止指令\n";
        }

        unlink(_local_path.c_str());
        close(local_socket_fd);
    });

    _run_task->join();
    _stop_task->join();

}

void Backstage_task::stop()
{
    if (_local_path.size() == 0)
    {
        std::cout << "\n未设置本地套接字路径\n";
        return;       
    }

    int client_local_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    sockaddr_un server_local_sockaddr_un;
    server_local_sockaddr_un.sun_family = AF_LOCAL;

    for (int i = 0; i < 108 && i < _local_path.size(); ++i)
        server_local_sockaddr_un.sun_path[i] = _local_path[i];

    int connect_over = connect(client_local_socket, reinterpret_cast<sockaddr*>(&server_local_sockaddr_un), sizeof(server_local_sockaddr_un));

    if (connect_over != -1)
    {
        write(client_local_socket, "off run", 8);
        std::cout << "\n已经发送停止指令\n";
    }
    else 
    {
        std::cout << "\n没有客户端在运行\n";
    }
    close(client_local_socket);
}



