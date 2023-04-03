#include "select_ip.h"
#include <regex>
#include <set>

const char* select_ip::_login_failed_feature_str = "Failed password";

select_ip::select_ip(std::string& argv_log_line_str, std::set<std::string>& argv_login_blacklisk_set, std::map<std::string, int>& argv_login_failures_map, int& argv_login_failures_size)
    : _log_line_str(argv_log_line_str), _login_blacklisk_set(argv_login_blacklisk_set), _login_failures_map(argv_login_failures_map), _login_failures_size(argv_login_failures_size),
    _ip('\0', 16), _log_failed_feature_str_index(0), _log_failed_find_index(0), _regex_ip("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}")
{ /*tmp_int = 0;*/ }

void select_ip::get_ip(std::string& argv_tmp)
{
    std::regex_search(argv_tmp, _regex_return, _regex_ip);
    argv_tmp = _regex_return.str();
}

void select_ip::run()
{
    login_failed_ip();

    if (_ip.size() != 0)
    {
        //++tmp_int;
        if (_login_blacklisk_set.find(_ip) == _login_blacklisk_set.end())
        {
            ++(_login_failures_map[_ip]);

            if (_login_failures_map[_ip] > _login_failures_size)
            {
                _login_failures_map.erase(_ip);
                _login_blacklisk_set.emplace(_ip);
            }
        }
    }
}

bool select_ip::login_failed()
{
    _log_failed_find_index = _log_line_str.find(_login_failed_feature_str);
    return (_log_failed_find_index >= 0 &&  _log_failed_find_index <= _log_line_str.size());
}

void select_ip::login_failed_ip()
{
    _ip.clear();
    if (login_failed())
    {
        std::regex_search(_log_line_str, _regex_return, _regex_ip);
        _ip = _regex_return.str();
    }
}