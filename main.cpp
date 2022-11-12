// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <regex>

// using std::cout;
// using std::endl;
// using std::stringstream;
// using std::string;
// using std::vector;
// using std::map;
// using std::regex;
// using std::smatch;


// int main(int argc, char** argv)
// {
//     std::ifstream file_in("auth.log");
//     map<string, int> ip_size;

//     vector<string> vector_log;
//     string tmp_log_str;
//     string tmp_log_re_ip;

//     if (!file_in.is_open())
//         throw std::logic_error("文件打开失败");
    
//     regex re_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}");
//     smatch re_ip_return_str;

//     for (int i = 1, is_find = 0; std::getline(file_in, tmp_log_str); ++i)
//     {
//         is_find = tmp_log_str.find("Failed password");

//         if (is_find != -1)
//         {
//             std::regex_search(tmp_log_str, re_ip_return_str, re_ip);
//             tmp_log_re_ip = re_ip_return_str.str();
//             vector_log.push_back(tmp_log_re_ip);

//             if (ip_size.count(tmp_log_re_ip) == 1)
//             {
//                 ip_size[tmp_log_re_ip] = ip_size[tmp_log_re_ip] +1;
//             }   
//             else
//                 ip_size[tmp_log_re_ip] = 1;
//         }

//     }

//     cout << vector_log.size() << endl;

//     for (auto a = ip_size.begin(); a != ip_size.end(); ++a)
//         cout << "爆破ip的为\t" << a->first << "\t爆破的次数为\t" << a->second << endl;
    

//     file_in.close();


//     return 0;
// }

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <regex>
#include <thread>
#include "ban_ip.h"

using std::regex;
using std::smatch;
using std::ofstream;

void ban_ip_help();
void ban_ip_init();
void ban_ip_config(ban_ip& argv_ban_ip, const char* argv_file_name);
void ban_ip_config(ban_ip& argv_ban_ip, string& argv_file_name);
void ban_ip_config(ban_ip& argv_ban_ip);
void ban_ip_run(ban_ip& argv_ban_ip);
// void ban_ip_start(ban_ip& argv_ban_ip);
// void ban_ip_stop(ban_ip& argv_ban_ip);


int main(int argc, char** argv)
{
    ban_ip m_ban_ip;
    string argv_str;
    
    if (argc == 1)
        ban_ip_help();
    else if (argc == 2)
    {
        argv_str = argv[1];
        //std::cout << argv_str << "\t" << argv_str.size() << std::endl;
        if (argv_str == "-help")
            ban_ip_help();
        else if (argv_str == "-init")
            ban_ip_init();
        else if (argv_str == "-run")
        {
            ban_ip_config(m_ban_ip, "./config.ini");
            ban_ip_run(m_ban_ip);
 
        } 
        // else if (argv_str == "-start")
        // {
        //     ban_ip_config(m_ban_ip, "./config");
        //     ban_ip_start(m_ban_ip);
        // }
        // else if (argv_str == "-stop")
        //     ban_ip_stop;
        else
        {
            printf("未知参数\n");
            ban_ip_help();
        }
    }
    else if (argc == 3)
    {
        argv_str = argv[1];
        if (argv_str == "-config")
        {
            printf("在使用 -config 时, 需要配合 -run 或者 -start 来使用指定配置文件 运行/后台运行\n");
            // printf("ban_ip -config /opt/ban_ip/config.ini -run 或者 ban_ip -config /opt/ban_ip/config.ini -start\n");
            // printf("ban_ip -run -config /opt/ban_ip/config.ini 或者 ban_ip -start -config /opt/ban_ip/config.ini\n");
            printf("ban_ip -config /opt/ban_ip/config.ini -run\n");
            printf("ban_ip -run -config /opt/ban_ip/config.ini\n");
        }
        else
            throw std::logic_error("未知参数和组合");
    }
    else if (argc == 4)
    {
        argv_str = argv[1];
        string config_ini_name;

        if (argv_str == "-config")
        {
            config_ini_name = argv[2];
            argv_str = argv[3];

            if (argv_str == "-run")
            {
                ban_ip_config(m_ban_ip, config_ini_name);
                ban_ip_config(m_ban_ip);
                ban_ip_run(m_ban_ip);
            } 
            // else if (argv_str == "-start")
            // {
            //     ban_ip_config(m_ban_ip, config_ini_name);
            //     ban_ip_start(m_ban_ip);
            // }
            else
                throw std::logic_error("未知的参数组合");
                
        }
        // else if (argv_str == "-start")
        // {
        //     argv_str = argv[2];
        //     if (argv_str == "-config")
        //     {
        //         config_ini_name = argv[3];
        //         ban_ip_config(m_ban_ip, config_ini_name);
        //         ban_ip_start(m_ban_ip);
        //     }
        //     else
        //         throw std::logic_error("未知的参数组合");
        // }
        else if (argv_str == "-run")
        {
            argv_str = argv[2];
            if (argv_str == "-config")
            {
                config_ini_name = argv[3];
                ban_ip_config(m_ban_ip, config_ini_name);
                ban_ip_run(m_ban_ip);
            }
        }
    }
    else
        throw std::logic_error("未知的参数组合");

    return 0;
}

void ban_ip_help()
{
    printf("-help \t(查看帮助)\n");
    printf("-init \t(在默认路径 重新/自动 生成配置文件)\n");
    printf("-config 配置文件\t(指定配置路径的文件)\n");
    printf("-run \t(使用默认路径的配置文件, 运行一次)\n");
    //printf("-start \t(使用默认路径的配置文件,在后台运行进程)\n");
    //printf("-stop \t(使用默认路径的配置文件,停止后台运行的进程)\n");
}

void ban_ip_init()
{
    std::ifstream file_in;
    std::ofstream file_out;

    file_in.open("/var/log/auth.log");
    if (file_in.is_open())
    {
        file_in.close();

        file_out.open("./config.ini");
        file_out << "[log_file]\n";
        file_out << "file_name = /var/log/auth.log\n";
        file_out << "error_num = 5\n\n";
        file_out << "[hosts_file]\n";
        file_out << "file_name = /etc/hosts.deny\n";
        file_out.close();
    }
    else
    {
        file_in.open("/var/log/secure");
        if (file_in.is_open())
        {
            file_in.close();

            file_out.open("./config.ini");
            file_out << "[log_file]\n";
            file_out << "file_name = /var/log/secure\n";
            file_out << "error_num = 5\n\n";
            file_out << "[hosts_file]\n";
            file_out << "file_name = /etc/hosts.deny\n";
            file_out.close();
        }
        else
            throw std::logic_error("非Debian或者CentOS系系统, 无法自动生成配置文件");
    }
}

void ban_ip_config(ban_ip& argv_ban_ip, const char* argv_file_name)
{
    string tmp_file_name(argv_file_name);
    ban_ip_config(argv_ban_ip, tmp_file_name);
}

void ban_ip_config(ban_ip& argv_ban_ip, string& argv_file_name)
{
    argv_ban_ip.config(argv_file_name);
}

void ban_ip_config(ban_ip& argv_ban_ip)
{
    argv_ban_ip.init();
}

void ban_ip_run(ban_ip& argv_ban_ip)
{
    while(true)
    {
        argv_ban_ip.run();
        printf("run\n");
        std::this_thread::sleep_for(std::chrono::minutes(argv_ban_ip.get_int_time()));
        //std::this_thread::sleep_for(std::chrono::seconds(argv_ban_ip.get_int_time()));
    }
}

// void ban_ip_start(ban_ip& argv_ban_ip)
// {
//     printf("ban_ip start\n");
// }

// void ban_ip_stop(ban_ip& argv_ban_ip)
// {
//     printf("ban_ip stop\n");
// }



















