#include "select_ip.h"
#include <regex>
#include <set>
#include <string>

const char* select_ip::_login_failed_feature_str = "Failed password";

select_ip::select_ip(std::string& argv_log_line_str, std::string& argv_ip)
    : _log_line_str(argv_log_line_str), _ip(argv_ip), _log_failed_find_index(0), _regex_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}"), _regex_ip_bool(false)
{ /*tmp_int = 0;*/ }

const bool& select_ip::get_failed_feature()
{
    return login_failed();
}

const bool& select_ip::get_ip()
{
    return login_failed_ip();
}

//void select_ip::run()
//{
//    login_failed_ip();
//
//    if (_ip.size() != 0)
//    {
//        //++tmp_int;
//        if (_login_blacklisk_set.find(_ip) == _login_blacklisk_set.end())
//        {
//            ++(_login_failures_map[_ip]);
//
//            if (_login_failures_map[_ip] > _login_failures_size)
//            {
//                _login_failures_map.erase(_ip);
//                _login_blacklisk_set.emplace(_ip);
//            }
//        }
//    }
//}

const bool& select_ip::login_failed()
{
    _log_failed_find_index = _log_line_str.find(_login_failed_feature_str);
    _regex_ip_bool = (_log_failed_find_index >= 0 &&  _log_failed_find_index <= _log_line_str.size());
    return _regex_ip_bool;
}

const bool& select_ip::login_failed_ip()
{
    _ip.clear();
    std::regex_search(_log_line_str, _regex_return, _regex_ip);
    _ip = _regex_return.str();
    _regex_ip_bool = (_ip.size() != 0);

    return _regex_ip_bool;
}