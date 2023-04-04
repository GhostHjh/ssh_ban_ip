#include <iostream>
#include <unistd.h>
#include <vector>
#include "ban_ip.h"
#include "select_ip.h"
#include "Backstage_task.h"


std::string default_config_path(std::string default_str);
void start_ban_ip(std::string config_name = "./config.ini");
void stop_ban_ip();
void help();

int main(int argc, const char** argv)
{
    std::vector<std::string> argv_s;
    for (int i = 0; i < argc; ++i)
        argv_s.emplace_back(argv[i]);

    if (argc == 1)
    {
        help();
    }
    else if (argc == 2) 
    {
        if (argv_s[1] == "-run")
            ban_ip().run(default_config_path(argv[0]).c_str());
        else if (argv_s[1] == "-start")
            start_ban_ip(default_config_path(argv[0]));
        else if (argv_s[1] == "-stop")
            stop_ban_ip();
        else if (argv_s[1] == "-help")
            help();
        else if (argv_s[1] == "-init_config")
            ban_ip().init_config_file(default_config_path(argv[0]).c_str());
        else
        {
            std::cout << "未知参数!\n";
            help();
        }
    }
    else if (argc == 4)
    {
        if (argv_s[1] == "-run" && argv_s[2] == "-config")
            ban_ip().run(argv_s[3].c_str());
        else if (argv_s[3] == "-run" && argv_s[1] == "-config")
            ban_ip().run(argv_s[2].c_str());
        else if (argv_s [1] == "-start" && argv_s[2] == "-config")
            start_ban_ip(argv_s[3]);
        else if (argv_s [3] == "-start" && argv_s[1] == "-config")
            start_ban_ip(argv_s[2]);
        else if (argv_s[1] == "-init_config" && argv_s[2] == "-config")
            ban_ip().init_config_file(argv_s[3].c_str());
        else if (argv_s[3] == "-init_config" && argv_s[1] == "-config")
            ban_ip().init_config_file(argv_s[2].c_str());
        else 
        {
            std::cout << "参数有误!\n";
            help();
        }
    }
    else 
    {
        std::cout << "参数过多或过少!\n";
        help();
    }


    return 0;
}

std::string default_config_path(std::string default_str)
{
    default_str.erase(default_str.find_last_of('/'), default_str.size());
    return default_str += "/config.ini";
}

void start_ban_ip(std::string config_name)
{
    int _pid = fork();
    if (_pid == 0)
    {
        ban_ip my_ban_ip;
        Backstage_task::get()->set_run_func(std::move(std::bind(&ban_ip::run, &my_ban_ip, config_name.c_str())));
        Backstage_task::get()->set_local_path("/tmp/test_ban_ip");
        Backstage_task::get()->set_run_sleep(10 *60);
        Backstage_task::get()->start();
    }
    else if (_pid > 0)
        sleep(1);
}

void stop_ban_ip()
{
    Backstage_task::get()->set_local_path("/tmp/test_ban_ip");
    Backstage_task::get()->stop();   
}

void help()
{
    std::cout << "ban_ip [选项]\n"
        << "-help\t\t查看帮助\t\t(ban_ip -help)\n"
        << "-run\t\t运行一次\t\t(ban_ip -run)\n"
        << "-config\t\t指定配置文件\t\t(ban_ip -run -config xxx.ini 或 ban_ip -config xxx.ini -run 或 ban_ip -start -config xxx.ini 或 ban_ip -config xxx.ini -start)\n"
        << "-init_config\t生成默认配置文件\t(ban_ip -init_config 可以使用 -config xxx.ini 指定配置文件的生成路径)\n";
}


