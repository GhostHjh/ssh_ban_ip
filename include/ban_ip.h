#pragma once
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string>

#include "my_ini.h"
#include "select_ip.h"

class ban_ip
{
public:
    ban_ip();
    ~ban_ip();

public:
    //运行一次
    void run(const char* argv_configfile_path = "./config.ini");
    //重新创建一个配置文件
    void init_config_file(const char* argv_file_name = "./config.ini");
    //显示当前黑名单ip
    void show()
    {
        for (auto& ip : _login_blacklist_set)
            std::cout << ip << "\n";
    }
    //查看帮助文档
    void help();

private:
    //从配置文件读取配置
    bool update_config(const char* argv_configfile_path);
    //更新黑名单
    void update_login_blacklist_set();
    //日志文件是否改变
    bool logfile_change();

private:
    //日志文件的路径
    std::string _logfile_name;
    //登入失败的次数
    int _login_failures_size;
    //hosts.deny文件的路径
    std::string _hostsfile_name;

    //用于存储失败次数的哈希表
    std::map<std::string, int> _login_failures_map;
    //存储ip黑名单
    std::set<std::string> _login_blacklist_set;

    //从日志文件读取的字符串
    std::string _file_line_str;
    //日志文件读取到了第机行
    int _file_line_number;
    //存储读取到的ip
    std::string _ip;

private:
    //日志文件句柄
    std::ifstream _log_file_fd;
    //hosts.deny文件句柄
    std::ofstream _hosts_file_fd;

private:
    //用于筛选ip的
    select_ip _select_ip;
};