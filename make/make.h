#ifndef MAKE_H
#define MAKE_H

extern FILE *check_map_file(void);

extern void open_map_file(const char *argv1, FILE **map);

extern void clean_up(FILE **map);

extern std::size_t get_map_file(std::string &_file_content, FILE **map);

extern std::string get_tokens(const char *map_content, std::size_t map_size);

#endif
