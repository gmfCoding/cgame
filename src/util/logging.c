#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "io.h"
#include "logging.h"
#include <dirent.h>

const char* log_level_strings[LOG_LEVEL_COUNT] = {
    [LOG_LEVEL_NONE] = "NONE",
    [LOG_LEVEL_INFO] = "INFO",
    [LOG_LEVEL_WARNING] = "WARNING",
    [LOG_LEVEL_ERROR] = "ERROR",
};

struct te_log_context g_log_context = {
    .min = LOG_LEVEL_INFO,
    .display_fd = STDERR_FILENO,
    .unfiltered_fd = -1,
    .path_options = {
        .directory = "logs/",
        .timestamp = true
    }
};

static int te_log_category_index(const char* category)
{
    for (int i = 0; i < 100; i++)
    {
        if (g_log_context.categories[i] && strncmp(g_log_context.categories[i], category, strlen(category)) == 0)
        {
            return i;
        }
    }
    return -1;
}

static bool te_log_category_enabled(const char* category)
{
    int index = te_log_category_index(category);
    if (index != -1)
    {        
        return g_log_context.category_disabled[index] == false;
    }
    return false;
}

void te_dvlogf_internal(enum te_log_level level, int descriptor, const char* category, const char* fmt, va_list args)
{
    va_list list;
    va_copy(list, args);
    if(isatty(descriptor))
    {
        const char* colour_code = "";
        switch (level)
        {
            case LOG_LEVEL_INFO:
                colour_code = "\x1b[32m"; // Green
                break;
            case LOG_LEVEL_WARNING:
                colour_code = "\x1b[33m"; // Yellow
                break;
            case LOG_LEVEL_ERROR:
                colour_code = "\x1b[31m"; // Red
                break;
            default:
                colour_code = "\x1b[0m"; // Reset
                break;
        }
        dprintf(descriptor, "[%s%s\x1b[0m](%s) ", colour_code, log_level_strings[level], category);
    }
    else
    {
        dprintf(descriptor, "[%s](%s) ", log_level_strings[level], category);
    }
    vdprintf(descriptor, fmt, list);
    dprintf(descriptor, "\n");
    va_end(list);
}

void te_log_delete_old_logs(void)
{
    // if (g_log_context.path_options.directory == NULL)
    //     return;
    // char path[PATH_MAX];
    // io_get_executable_dir(path);
    // io_merge_path_curr(path, g_log_context.path_options.directory);

    // DIR* dir = opendir(path);
    // if (dir == NULL)
    // {
    //     fprintf(stderr, "Failed to open log directory '%s' for cleanup\n", path);
    //     return;
    // }

    // struct dirent* entry;
    // time_t now = time(NULL);
    // int file_count = 0;
    // #ifndef DT_REG
    //     #define DT_REG 8
    //     #error "DT_REG not defined, defining it as 8. This may cause issues if the actual value is different on this platform."
    // #endif
    // // Loop over files in the log directory, and if there are more than 8 .txt files, delete the oldest ones
    // // Loop over files in the log directory, and find the earliest modified .txt file, and delete it if there are more than 3 .txt files
    // while ((entry = readdir(dir)) != NULL)
    // {
    //     if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL)
    //     {
    //         file_count++;
    //         if (file_count > 0)
    //         {
    //             char file_path[PATH_MAX];
    //             snprintf(file_path, sizeof(file_path), "%s%s", path, entry->d_name);
    //             struct stat st;
    //             if (stat(file_path, &st) == 0)                {
    //                 double age = difftime(now, st.st_mtime);
    //                 if (age > 60 * 60 * 24) // If the file is older than 1 day, delete it
    //                 {   
    //                     if (remove(file_path) == 0)
    //                     {
    //                         fprintf(stderr, "Deleted old log file '%s'\n", file_path);
    //                     }
    //                     else
    //                     {
    //                         fprintf(stderr, "Failed to delete old log file '%s'\n", file_path);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // closedir(dir);
}

void te_setup_log_file(void)
{
    if (g_log_context.path_options.directory == NULL)
        return;
    char path[PATH_MAX];
    char file_path[PATH_MAX];

    io_get_executable_dir(path);
    io_merge_path_curr(path, g_log_context.path_options.directory);

    mkdir(path, 0755);
    snprintf(file_path, sizeof(file_path), "%slog_%ld.txt", path, time(NULL));
    g_log_context.unfiltered_fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (g_log_context.unfiltered_fd == -1)
    {
        fprintf(stderr, "Failed to open log file '%s' for writing\n", file_path);
    }
    te_log_delete_old_logs();
}

void te_vlogf(enum te_log_level level, const char* category, const char* fmt, va_list list)
{
    if (te_log_category_enabled(category) == false)
        return;
    if (g_log_context.unfiltered_fd == -1)
        te_setup_log_file();

    te_dvlogf_internal(level, g_log_context.unfiltered_fd, category, fmt, list);
    if (level < g_log_context.min)
        return;
    te_dvlogf_internal(level, g_log_context.display_fd, category, fmt, list);
}

void te_logf(enum te_log_level level, const char* category, const char* fmt, ...)
{

    va_list args;
    va_start(args, fmt);
    te_vlogf(level, category, fmt, args);
    va_end(args);
}


void te_log_set_category(const char* category, enum te_log_filter_state level)
{
    for (int i = 0; i < 100; i++)
    {
        // First null category, add new category
        if (g_log_context.categories[i] == NULL)
        {
            g_log_context.categories[i] = category;
            g_log_context.category_disabled[i] = level;
            return;
        }
        else if (strncmp(g_log_context.categories[i], category, strlen(category)) == 0)
        {
            g_log_context.category_disabled[i] = level;
            return;
        }
    }
}

void logging_default_categories(void)
{
    te_log_set_category("io",        LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("material",  LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("rendering", LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("input",     LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("audio",     LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("physics",   LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("network",   LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("ui",        LOG_LEVEL_FILTER_ENABLED);
    te_log_set_category("resource",  LOG_LEVEL_FILTER_ENABLED);
}