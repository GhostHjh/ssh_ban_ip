#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <regex>

class select_ip
{
public:
    select_ip(std::string& argv_log_line_str, std::string& argv_ip);
    const bool& get_failed_feature();
    const bool& get_ip();

private:
    const bool& login_failed();
    const bool& login_failed_ip();

private:
    static const char* _login_failed_feature_str;

private:
    std::string& _log_line_str;
    std::string& _ip;

    int _log_failed_find_index;

private:
    std::regex _regex_ip;
    std::smatch _regex_return;
    bool _regex_ip_bool;

    //int tmp_int;
};


