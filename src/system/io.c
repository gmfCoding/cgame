//https://stackoverflow.com/a/60250581 Atul
// Tested on Windows 10 64bit

#if defined(_WIN32)
    #include <windows.h>
    #include <Shlwapi.h>
    #include <io.h> 
    
    #define access _access_s
#endif

#ifdef __APPLE__
    #include <libgen.h>
    #include <limits.h>
    #include <mach-o/dyld.h>
    #include <unistd.h>
#endif

#ifdef __linux__
    #include <limits.h>
    #include <libgen.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include "io.h"
    #include "libcml.h"

    #if defined(__sun)
        #define PROC_SELF_EXE "/proc/self/path/a.out"
    #else
        #define PROC_SELF_EXE "/proc/self/exe"
    #endif

#endif

#if defined(_WIN32)



char* io_get_executable_filepath() {
   char rawPathName[MAX_PATH];
   GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
   return char*(rawPathName);
}

// TODO: delete[] causing breaks?   
char* io_get_executable_dir() {
    char* executablePath = io_get_executable_filepath();
    char* exePath = new char[executablePath.length() + 1]; 
    strcpy_s(exePath, executablePath.length() + 1, executablePath.c_str());
    PathRemoveFileSpecA(exePath);
    char* directory = char*(exePath);
    delete[] exePath;
    return directory;
}

char* mergePaths(char* pathA, char* next) {
  char combined[MAX_PATH];
  PathCombineA(combined, pathA.c_str(), next.c_str());
  char* mergedPath(combined);
  return mergedPath;
}

#endif

#ifdef __linux__


char *io_fileread(const char *path)
{		
    /* declare a file pointer */
    FILE    *infile;
    char    *buffer;
    long    numbytes;

    /* open an existing file for reading */
    infile = fopen(path, "r");

    /* quit if the file does not exist */
    if(infile == NULL)
        return NULL;

    /* Get the number of bytes */
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    /* reset the file position indicator to 
    the beginning of the file */
    fseek(infile, 0L, SEEK_SET);	

    /* grab sufficient memory for the 
    buffer to hold the text */
    buffer = calloc(numbytes + 1, sizeof(char));	

    /* memory error */
    if(buffer == NULL)
        return NULL;

    /* copy all the text into the buffer */
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);
    return buffer;
}

char* io_get_executable_filepath()
{
   char rawPathName[PATH_MAX];
   realpath(PROC_SELF_EXE, rawPathName);
   return strdup(rawPathName);
}

char* io_get_executable_dir()
{
    char* executablePath = io_get_executable_filepath();
    char* executableDir = strdup(dirname(executablePath));
    free(executablePath);
    return executableDir;
}

char* io_merge_path(char* pathA, char* next)
{
    char path[PATH_MAX] = "";

    cl_strlcat(path, pathA, PATH_MAX);
    cl_strlcat(path, "/", PATH_MAX);
    cl_strlcat(path, next, PATH_MAX);
    return strdup(path);
}

void io_merge_path_curr(char path[PATH_MAX], const char* next)
{
    cl_strlcat(path, "/", PATH_MAX);
    cl_strlcat(path, next, PATH_MAX);
}

#endif

bool io_file_exists(const char* file_path)
{
   return access(file_path, 1) == 0;
}