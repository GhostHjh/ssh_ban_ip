#include "my_ini.h"
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::stringstream;

/*my_ini_value*/
/***********************************************************************************************************/
my_ini_value::my_ini_value()                           { value_type = type_t::Nullptr; }
my_ini_value::my_ini_value(const my_ini_value& argv_tmp_ini_value)
{
    value_type = argv_tmp_ini_value.value_type;
    value = argv_tmp_ini_value.value;
}
my_ini_value::my_ini_value(const int& argv_value)      { *this = argv_value; }
my_ini_value::my_ini_value(const double& argv_value)   { *this = argv_value; }
my_ini_value::my_ini_value(const string& argv_value)   { *this = argv_value; }
my_ini_value::my_ini_value(const char* argv_value)     { *this = argv_value; }
my_ini_value::my_ini_value(const bool& argv_value)     { *this = argv_value; }

void my_ini_value::operator = (const int& argv_value)
{
    value_type = type_t::Int;
    value = std::to_string(argv_value);
}

void my_ini_value::operator = (const double& argv_value)
{
    value_type = type_t::Double;
    value = std::to_string(argv_value);
}

void my_ini_value::operator = (const string& argv_value)
{
    value_type = type_t::Str;
    value = argv_value;
}

void my_ini_value::operator = (const char* argv_value)
{
    value_type = type_t::Str;
    value = argv_value;
}

void my_ini_value::operator = (const bool& argv_value)
{
    value_type = type_t::Bool;
    if (argv_value)
        value = "true";
    else
        value = "false";
}

my_ini_value::operator int()
{
    return atoi(value.c_str());
}

my_ini_value::operator double()
{
    return atof(value.c_str());
}

my_ini_value::operator const char*()
{
    return value.c_str();
}

my_ini_value::operator string()
{
    return value;
}

my_ini_value::operator bool()
{
    return (value == "true");
}

int my_ini_value::to_int()          { return atoi(value.c_str()); }
double my_ini_value::to_double()    { return atof(value.c_str()); }
string& my_ini_value::to_str()      { return value; }
bool my_ini_value::to_bool()        { return (value == "true"); }
/***********************************************************************************************************/

my_ini::my_ini() {}
my_ini::my_ini(const string& argv_file_name) : file_name(argv_file_name)
{
    init();
}

bool my_ini::config()
{
    ifstream file_in(file_name);
    if (file_in.is_open())
    {
        file_in.close();
        return true;
    }
    return false;
}

bool my_ini::config(const string& argv_file_name)
{
    file_name = argv_file_name;
    return config();
}

void my_ini::init()
{
    if (!config())
        throw std::logic_error("文件读取异常");

    ifstream file_in(file_name);

    string name;
    string key;
    string value;
    string tmp_file_str;

    int l_fh = -1; 
    int r_fh = -1;
    int d_fh = -1;

    while (getline(file_in, tmp_file_str))
    {
        if (tmp_file_str.size() < 2 && tmp_file_str[0] == '#')
            continue;

        d_fh = tmp_file_str.find_first_of('=');
        if (d_fh == -1)
        {
            l_fh = tmp_file_str.find('[');
            r_fh = tmp_file_str.find(']');
        }

        if (l_fh != -1 && r_fh  != -1)
        {
            string().swap(name);
            name = tmp_file_str.substr(l_fh +1, r_fh -1);
        }
        else if (d_fh != -1)
        {
            string().swap(key);
            string().swap(value);

            for (int i = 0; i < d_fh -1; ++i)
            {
                if (tmp_file_str[i] == ' ' || tmp_file_str[i] == '\t')
                    ++i;
                key += tmp_file_str[i];
            }

            for (int i = d_fh +1; i < tmp_file_str.size(); ++i)
            {
                if (tmp_file_str[i] == ' ' || tmp_file_str[i] == '\t')
                    ++i;
                value += tmp_file_str[i];
            }
            push_back(name, key, value);
        }

        l_fh = -1; 
        r_fh = -1;
        d_fh = -1;
    }

    file_in.close();
}

bool my_ini::count(const string& argv_key)
{
    return (ini_block.count(argv_key) == 1);
}

bool my_ini::count(const string& argv_name_key, const string& argv_value_name_key)
{
    if (count(argv_name_key))
    {
        return (ini_block[argv_name_key].count(argv_value_name_key) == 1);
    }

    return false;
}

bool my_ini::push_back(const string& argv_name, const string& argv_key, const string& argv_value)
{
    if (!count(argv_name))
    {
        ini_block[argv_name][argv_key] = argv_value;
        return true;
    }
    else if (!count(argv_name, argv_key))
    {
        ini_block[argv_name][argv_key] = argv_value;
        return true;
    }

    return false;  
}

my_ini_value my_ini::get(const string& argv_ini_block_name, const string& argv_ini_block_key)
{
    if (count(argv_ini_block_name, argv_ini_block_key))
        return ini_block[argv_ini_block_name][argv_ini_block_key];
    
    return my_ini_value();
}

void my_ini::show()
{
    for (auto a = ini_block.begin(); a != ini_block.end(); ++a)
    {
        cout << '[' << a->first << ']' << endl;

        for (auto b = (a->second).begin(); b != (a->second).end(); ++ b)
            cout << b->first << " : " << b->second.to_str() << endl;
        
        cout << endl << endl;
    }
}







