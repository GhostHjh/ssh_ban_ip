#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <regex>

class select_ip
{
public:
    select_ip(std::string& argv_log_line_str, std::set<std::string>& argv_login_blacklisk, std::map<std::string, int>& argv_login_failures_map, int& argv_login_failures_size);
    void get_ip(std::string& argv_tmp);
    void run();
    /*void show()
    {
        std::cout << "\n\n\n" <<  tmp_int << "\n\n\n";
    }*/

private:
    bool login_failed();
    void login_failed_ip();

private:
    static const char* _login_failed_feature_str;

private:
    std::string& _log_line_str;
    std::set<std::string>& _login_blacklisk_set;
    std::map<std::string, int>& _login_failures_map;
    int& _login_failures_size;
    std::string _ip;

    int _log_failed_find_index;
    int _log_failed_feature_str_index;

private:
    std::regex _regex_ip;
    std::smatch _regex_return;

    //int tmp_int;
};


