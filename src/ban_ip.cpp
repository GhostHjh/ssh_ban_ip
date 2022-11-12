#include "ban_ip.h"
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::getline;

ban_ip::ban_ip() {}

ban_ip::~ban_ip()
{
}

ban_ip::ban_ip(const string& argv_config_file_name) : config_file_name(argv_config_file_name)
{
    run();
}


bool ban_ip::config()
{
    ifstream file_in(config_file_name);
    if (file_in.is_open())
    {
        file_in.close();
        return true;
    }

    return false;
}

bool ban_ip::config(const string& argv_config_file_name)
{
    config_file_name = argv_config_file_name;

    return config();
}

bool ban_ip::init()
{
    if (!config())
        throw std::logic_error("未成功读取到配置文件");
    
    my_ini m_ini(config_file_name);

    if (!(m_ini.count("log_file") && m_ini.count("hosts_file")))
        throw std::logic_error("配置文件未配置好");
    
    if (m_ini.count("log_file", "file_name"))
        m_config.log_file_name = m_ini.get("log_file", "file_name").to_str();
    
    if (m_ini.count("log_file", "error_num"))
        m_config.error_num = m_ini.get("log_file", "error_num").to_int();
    else
        m_config.error_num = 5;
    
    if (m_ini.count("hosts_file", "file_name"))
        m_config.hosts_file_name = m_ini.get("hosts_file", "file_name").to_str();
    
    if (m_ini.count("setting", "int_time"))
        m_config.int_time = m_ini.get("setting", "int_time").to_int();
    else
        m_config.int_time = 5;
    
    cout << endl << endl << endl
        << "log_file_name\t" << m_config.log_file_name << endl
        << "log_file_error\t" << m_config.error_num << endl
        << "hosts_file\t" << m_config.hosts_file_name << endl
        << "int_time\t" << m_config.int_time << endl << endl << endl;
    
    return true;
}

void ban_ip::run()
{
    string tmp_file_str;
    int is_error_ip;

    //打开当前的日志文件检测登入失败的记录
    ifstream log_file_in(m_config.log_file_name);
    if (!log_file_in.is_open())
        throw std::logic_error("log file open error!");

    while (getline(log_file_in, tmp_file_str))
    {
        is_error_ip = tmp_file_str.find("Failed password");
        if (is_error_ip != -1)
            log_file_str_s.push_back(tmp_file_str);
    }
    log_file_in.close();
    select_ip(log_file_str_s, log_ip_enum);

    //打开当前的hosts文件读取被记录的ip
    ifstream hosts_file_in(m_config.hosts_file_name);
    if (hosts_file_in.is_open())
    {
        while (getline(hosts_file_in, tmp_file_str))
        {
            is_error_ip = tmp_file_str.find("sshd");
            if (is_error_ip != -1)
                hosts_file_str_s.push_back(tmp_file_str);
        }
        hosts_file_in.close();
        select_ip(hosts_file_str_s, hosts_ip_enum);
    }

    //对比hosts文件, 并决定是否写入新的ip
    ofstream hosts_file_out(m_config.hosts_file_name, std::ios::app);
    if (!hosts_file_out.is_open())
        throw std::logic_error("hosts file open error!");
    
    for (auto a = log_ip_enum.begin(); a != log_ip_enum.end(); ++a)
    {
        if (a->second >= m_config.error_num)
        {
            if (hosts_ip_enum.count(a->first) != 1)
                hosts_file_out << "\nsshd:" << a->first;
        }
    }
    hosts_file_out.close();
}

int ban_ip::get_int_time()
{
    return m_config.int_time;
}




