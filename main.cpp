#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <regex>
#include "ban_ip.h"
#include "select_ip.h"


int main(int argc, const char** argv)
{
    //std::cout << "hello world!\n";
    //std::string tmp_str("Oct 30 00:16:14 Debian sshd[813438]: Failed password for root from 61.177.172.19 port 23601 ssh2");
    //std::cout << tmp_str.find("Failed password") << "\n";
    //std::cout << tmp_str[37] << "\n";
    //std::regex re_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}");
    //std::smatch results1;
    //std::regex_search(tmp_str, results1, re_ip);
    //std::cout << results1.str() << "\n";

    ban_ip tmp_ban_ip;
    tmp_ban_ip.run("./config2.ini");
    tmp_ban_ip.show();




    return 0;
}

