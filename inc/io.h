/*
 * @Author: christopher.lovell 
 * @Date: 2024-07-01 17:25:59 
 * @Last Modified by: christopher.lovell
 * @Last Modified time: 2024-07-01 18:58:01
 */
#include <stdbool.h>

#ifdef __linux__
 # define PATH_MAX 4096
#endif

char *io_fileread(const char *path);

char* io_get_executable_filepath();
char* io_get_executable_dir();
char* io_merge_path(char* pathA, char* next);
void io_merge_path_curr(char path[PATH_MAX], const char* next);
bool io_file_exists(const char* file_path);