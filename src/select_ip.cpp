#include "select_ip.h"

using std::cout;
using std::endl;
using std::regex_search;


select_ip::select_ip() : file_str_s(nullptr), ip_enum(nullptr), re_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}"), re_num("num:num:([0-9]{1,2})"), re_mode(0) {}

select_ip::select_ip(vector<string>& argv_file_str_s, map<string, int>& argv_ip_enum, int argv_mode) : file_str_s(&argv_file_str_s), ip_enum(&argv_ip_enum), re_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}"), re_num("num:num:([0-9]{1,2})"), re_mode(argv_mode)
{
    init();
}

bool select_ip::config()
{
    return (file_str_s != nullptr && ip_enum != nullptr);
}

bool select_ip::config(vector<string>& argv_file_str_s, map<string,int>& argv_ip_enum, int argv_mode)
{
    file_str_s = &argv_file_str_s;
    ip_enum = &argv_ip_enum;
    re_mode = argv_mode;
    return config();
}

void select_ip::init()
{
    if (re_mode == 1)
    {
        if (!config())
            throw std::logic_error("select_ip 未就绪");

        for (auto a = file_str_s->begin(); a != file_str_s->end(); ++a)
        {
            regex_search(*a, re_ip_return, re_ip);
            regex_search(*a, re_num_return, re_num);
            push_back(re_ip_return.str(), atoi(re_num_return.str(1).c_str()));
        }
    }
    else
    {
        if (!config())
            throw std::logic_error("select_ip 未就绪");

        for (auto a = file_str_s->begin(); a != file_str_s->end(); ++a)
        {
            regex_search(*a, re_ip_return, re_ip);
            push_back(re_ip_return.str());
        }
    }

}


void select_ip::push_back(const string& argv_ip)
{
    if (ip_enum->count(argv_ip) == 1)
    {
        (*ip_enum)[argv_ip] = (*ip_enum)[argv_ip] +1;
    }
    else
        (*ip_enum)[argv_ip] = 1;
}

void select_ip::push_back(const string& argv_ip, const int& argv_num)
{
    (*ip_enum)[argv_ip] = argv_num;
}

void select_ip::show()
{
    for (auto a = ip_enum->begin(); a != ip_enum->end(); ++a)
    {
        cout << "ip:\t" << a->first << "\tsize:\t" << a->second << endl;
    }
}








