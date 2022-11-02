#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>


using std::string;
using std::vector;
using std::map;
using std::regex;
using std::smatch;

class select_ip
{
public:
    select_ip();
    select_ip(vector<string>& argv_flie_str_s, map<string, int>& argv_ip_enum, int argv_mode = 0);
    
    bool config();
    bool config(vector<string>& argv_flie_str_s, map<string, int>& argv_ip_enum, int argv_mode = 0);

    void init();
    void init_record();
    void show();

private:
    void push_back(const string& argv_ip);
    void push_back(const string& argv_ip, const int& argv_num);

private:
    int re_mode;
    vector<string>* file_str_s;
    regex re_ip;
    regex re_num;
    smatch re_ip_return;
    smatch re_num_return;
    map<string, int>* ip_enum;
};