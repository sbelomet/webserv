
#include <iostream>
#include "Config.hpp"

int main(int argc, char **argv)
{
    Config  config;

    if (argc != 2)
        std::cout << "Need a config file" << std::endl;
    else
    {
        config.makeConfig(argv[1]);
        std::map<std::string, Section>::iterator it;
        for (it = config.getConfig().begin(); it != config.getConfig().end(); it++)
        {
            std::cout << "header: " << it->first << std::endl;
            Section section = it->second;
            std::map<std::string, std::string>::iterator sub_it;
            for (sub_it = section.getSection().begin(); sub_it != section.getSection().end(); sub_it++)
            {
                std::cout << "section: " << sub_it->first << " : " << sub_it->second << std::endl;
            }
            std::cout << std::endl;
        }
    }
    return (0);
}


/*     while (std::getline(infile, line))
    {
        if (line.empty() || line[0] == '\n')
            continue ;
        line = trim(line);
        std::stringstream   ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, ' ');
        std::getline(ss, value, '\n');
        if (key.empty() || value.empty() || check_closing(key))
            continue ; // throw exception
        if (check_header(value) && key != curr_section)
        {
            Section section;
            curr_section = key;
            this->insertSection(curr_section, section);
        }
        else
            getSectionFromMap(curr_section).insertData(key, value);
    } */
