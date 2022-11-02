#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class my_ini_value
{
private:
    enum type_t
    {
        Nullptr = 0,
        Struct,
        Int,
        Double,
        Str,
        Bool
    };
    
public:
    my_ini_value();
    my_ini_value(const my_ini_value& argv_tmp_ini_value);
    my_ini_value(const int& argv_value);
    my_ini_value(const double& argv_value);
    my_ini_value(const string& argv_value);
    my_ini_value(const char* argv_value);
    my_ini_value(const bool& argv_value);

    void operator = (const int& argv_value);
    void operator = (const double& argv_value);
    void operator = (const string& argv_value);
    void operator = (const char* argv_value);
    void operator = (const bool& argv_value);

    operator int();
    operator double();
    operator const char*();
    operator string();
    operator bool();

    int to_int();
    double to_double();
    string& to_str();
    bool to_bool();

private:
    string value;
    type_t value_type;
};

class my_ini
{
public:
    my_ini();
    my_ini(const string& argv_file_name);

    bool config();
    bool config(const string& argv_file_name);

    void init();
    my_ini_value get(const string& argv_ini_block_name, const string& argv_ini_block_key);

public:
    bool count(const string& argv_key);
    bool count(const string& argv_name_key, const string& argv_value_name_key);
    bool push_back(const string& argv_name, const string& argv_key, const string& argv_value);
    void show();

private:
    string file_name;
    map<string, map<string, my_ini_value>> ini_block;
};



