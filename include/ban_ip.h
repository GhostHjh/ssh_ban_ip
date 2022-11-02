#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "my_ini.h"
#include "select_ip.h"

using std::string;
using std::vector;
using std::map;
using std::regex;
using std::smatch;


class ban_ip
{
private:
    class config_t
    {
    public:
        string os;
        string log_file_name;
        int error_num;
        string hosts_file_name;
        string record_file_name;
    };

public:
    ban_ip();
    ~ban_ip();
    ban_ip(const string& config_file_name);

    bool config();
    bool config(const string& config_file_name);
    int& config_level();

    void init();

private:
    string config_file_name;
    config_t m_config;
    int m_config_level;

    vector<string> log_file_str_s;
    map<string, int> log_ip_enum;

    vector<string> hosts_file_str_s;
    map<string, int> hosts_ip_enum;

    vector<string> record_file_str_s;
    map<string, int> record_ip_enum;
};



