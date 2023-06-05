#include <iostream>
#include <string.h>
#include "make.h"


static FILE *create_map_file(void)
{
    FILE *map_file = fopen("C:\\make\\map.txt", "w");
    if(map_file != nullptr)
    {
        std::fprintf(map_file, "%s", "makeMap\n{\n#Examples\n"
                                               "\"c\" : \"gcc main.c\",\n"
                                               "\"cpp\" : \"g++ main.cpp\",\n"
                                               "\"xc8\" : \"xc8-cc -mcpu=18f4620 main.c -o build/executable\",\n"
                                               "\"rust\" : \"rustc main.rs\",\n"
                                               "\"zig\" : \"zig run\",\n"
                                               "\"java\" : \"javac main.java\"\n"
                                               "}");
        std::puts("map.txt has been created in this directory \"C:\\make\", "
                  "You may open map.txt to complete the configurations and run the program again.");
        std::fclose(map_file); 
        std::exit(EXIT_SUCCESS);  
    }
    else
    {
        std::puts("Couldn't create map.txt in this directory \"C:\\make\" ... program terminated.");
        return map_file; 
    }
}


FILE *check_map_file(void)
{
    FILE *map_file = fopen("C:\\make\\map.txt", "r");
    if(map_file == nullptr)
    {
        return create_map_file();
    }
    else
    {
        return map_file; 
    }
}


void open_map_file(const char *argv1, FILE **map)
{
    std::string open_map_file = {};
    open_map_file.append(argv1);
    open_map_file.append(" ");
    open_map_file.append("C:\\make\\map.txt");
    open_map_file.shrink_to_fit();
    std::system(open_map_file.c_str());
    clean_up(map);
}


void clean_up(const char *__ptr, FILE **map)
{
    std::printf("Invalid option ==> (%s) ... program terminated.\n", __ptr); 
    clean_up(map);
}


void clean_up(FILE **map)
{
    FILE **__tmp = map;
    std::fclose(*__tmp);
    *map = nullptr;
}


std::size_t get_map_file(std::string &file_content, FILE **map)
{
    std::size_t counter = 0;
    int ch = 0;
    while(true)
    {
        ch = fgetc(*map);
        if(ch == EOF)
        {
            break;
        }
        file_content.push_back(ch); 
        counter++;
    }
    rewind(*map);
    file_content.shrink_to_fit();
    return counter;
}


std::string get_tokens(const char *map_content, std::size_t map_size)
{
    int ignore = true; 
    int read = true;
    int flag = false;

    std::size_t i = 0;
    std::size_t wspace = 0;

    std::string tokens = {};

    while (i < map_size)
    {
        if(!strncmp(&map_content[i], "makeMap", 7))
        {
            break;
        }
        ++i;
    }

    if(i == map_size)
    {
        return tokens; 
    }

    for (; i < map_size; ++i)
    {
        if((read == true) && map_content[i] == '#')
        {
            read = false; 
        }
        if((read == false) && map_content[i] == '\n')
        {
            read = true;
        }
        
        if(read == true)
        {
            if((map_content[i-1] != '\\' && map_content[i] == '"') && ignore == true)
            {
                ignore = false; 
            }
            else if(((map_content[i-1] != '\\' && map_content[i] == '"') || (map_content[i-1] != '\\' && map_content[i] == ':')) && ignore == false)
            {
                ignore = true; 
                if((map_content[i] == '"') && flag == true)
                {
                    flag = false;
                }
            }
        }
        else
        {
            continue;
        }

        if(map_content[i] == ':' && (ignore == true))
        {
            flag = true;
            tokens.push_back(map_content[i]);
            continue;
        }
        else if(map_content[i] == ' ' && (ignore == true))
        {  
            continue; 
        }
        else
        {

            if((map_content[i] == '\\') && !strncmp(&map_content[i+1], "033[", 4))
            {
                i += 5;
                for (; map_content[i] == ' '; ++i)
                {
                    tokens.push_back(map_content[i]);
                }
            }

            if((map_content[i] == '\\') && map_content[i+1] == ':')
            {
                tokens.push_back(':');
                ++i; 
                continue;
            }
            else if((map_content[i] == '\\') && map_content[i+1] == '#')
            {
                tokens.push_back('#'); 
                ++i; 
                continue;
            }

            if(map_content[i] == ' ' && (flag == false))
            {
                continue; 
            }
            else if(map_content[i] == ' ' && (flag == true))
            {
                wspace++; 
                if(wspace > 1)
                {
                    continue; 
                }
            }
            else if(map_content[i] != ' ' && (flag == true))
            {
                wspace = 0;
            }
            else
            {
            }

            tokens.push_back(map_content[i]);  
        }
    }
    tokens.shrink_to_fit();
    return tokens;
}
