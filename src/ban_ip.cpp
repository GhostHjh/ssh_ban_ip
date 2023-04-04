#include "ban_ip.h"
#include "my_ini.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <string>

ban_ip::ban_ip()
    : _file_line_number(0), _login_failures_size(0), _ip('\0', 16), _select_ip(_file_line_str, _ip)
{}

ban_ip::~ban_ip()
{
    _log_file_fd.close();
    _hosts_file_fd.close();
}



void ban_ip::run(const char* argv_configfile_path)
{
    if (_login_failures_size == 0)
    {
        if (!update_config(argv_configfile_path))
        {
            perror("读取到的配置文件不完整");
            exit(1);
        }
    }

    for (; std::getline(_log_file_fd, _file_line_str); )
    {
        if (_select_ip.get_failed_feature() && _select_ip.get_ip())
        {
            if (_login_blacklist_set.find(_ip) == _login_blacklist_set.end())
            {
                if (_login_failures_map[_ip] == _login_failures_size)
                {
                    _login_failures_map.erase(_ip);
                    _login_blacklist_set.emplace(_ip);
                    _hosts_file_fd << "sshd:" << _ip << "\n";
                    _hosts_file_fd.flush();
                }
                else
                    ++(_login_failures_map[_ip]);
            }
        }
    }
}


bool ban_ip::update_config(const char* argv_configfile_path)
{
    my_ini tmp_ini(argv_configfile_path);
    _logfile_name = tmp_ini.get("log_file", "file_name").to_str();
    _login_failures_size = tmp_ini.get("log_file", "error_num").to_int();
    _hostsfile_name = tmp_ini.get("hosts_file", "file_name").to_str();

    _log_file_fd.open(_logfile_name, std::ios::in);
    update_login_blacklist_set();
    _hosts_file_fd.open(_hostsfile_name, std::ios::app);

    if (_hostsfile_name.size() != 0 && _logfile_name.size() != 0 && _login_failures_size != 0 && _log_file_fd.is_open() && _hosts_file_fd.is_open())
        return true;
    return false;
}

void ban_ip::init_config_file(const char* argv_file_name)
{
    std::fstream config_file;
    config_file.open(argv_file_name, std::ios::in);

    //检测文件是否存在
    if (config_file.is_open())
    {
        std::cout << "\n\n\n" << argv_file_name << "文件已经存在, 是否要覆盖?\n\n\n";
        std::string tmp_ch;
        std::cin >> tmp_ch;

        if (tmp_ch != "y" && tmp_ch != "Y" && tmp_ch != "Yes" && tmp_ch != "YES" && tmp_ch != "yes")
            return;
        
        config_file.close();
    }

    config_file.open(argv_file_name, std::ios::out);
    if (config_file.is_open() == false)
        perror("配置文件生成失败!"), exit(1);

    //检测系统    
    const char* debian_logfile_name = "/var/log/auth.log";
    const char* centos_logfile_name = "/var/log/secure";
    
    std::ifstream tmp_log_file;
    tmp_log_file.open(debian_logfile_name, std::ios::in);
    if (tmp_log_file.is_open())
    {
        config_file << "[log_file]\n"
            << "file_name = /var/log/auth.log\n"
            << "error_num = 5\n"
            << "\n[hosts_file]\n"
            << "\nfile_name = /etc/hosts.deny\n";
        config_file.close();
        return;
    }
    
    tmp_log_file.open(centos_logfile_name, std::ios::in);
    if (tmp_log_file.is_open())
    {
        config_file << "[log_file]\n"
            << "file_name = /var/log/secure\n"
            << "error_num = 5\n"
            << "\n[hosts_file]\n"
            << "\nfile_name = /etc/hosts.deny\n";
        config_file.close();
        return;
    }

    perror("暂不支持该系统");
    exit(1);
}

void ban_ip::update_login_blacklist_set()
{
    std::ifstream tmp_file_fd(_hostsfile_name, std::ios::in);
    if (tmp_file_fd.is_open())
    {
        for (; std::getline(tmp_file_fd, _file_line_str); )
        {
            if (_select_ip.get_ip())
            {
                if (_login_blacklist_set.find(_ip) == _login_blacklist_set.end())
                    _login_blacklist_set.insert(_ip);
            }
        }
    }
}

bool ban_ip::logfile_change()
{

}






