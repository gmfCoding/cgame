#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "io.h"
#include "logging.h"

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
}

void te_vlogf(enum te_log_level level, const char* category, const char* fmt, va_list list)
{
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

void te_log_set_category(const char* category, bool status)
{
    for (int i = 0; i < 100; i++)
    {
        if (g_log_context.categories[i] == NULL)
        {
            g_log_context.categories[i] = category;
            g_log_context.category_enabled[i] = status;
            return;
        }
        else if (strncmp(g_log_context.categories[i], category, strlen(category)) == 0)
        {
            g_log_context.category_enabled[i] = status;
            return;
        }
    }
}

void logging_default_categories(void)
{
    te_log_set_category("io", true);
    te_log_set_category("material", true);
    te_log_set_category("rendering", true);
    te_log_set_category("input", true);
    te_log_set_category("audio", true);
    te_log_set_category("physics", true);
    te_log_set_category("network", true);
    te_log_set_category("ui", true);
    te_log_set_category("resource", true);
}