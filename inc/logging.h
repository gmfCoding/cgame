#ifndef TE_LOGGING_H
#define TE_LOGGING_H

enum te_log_level {
    LOG_LEVEL_NONE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
};

struct te_log_context {
    enum te_log_level min;
    int unfiltered_fd;
    int display_fd;

    const char* categories[100];
    bool category_disabled[100];

    struct {
        char* directory;
        bool timestamp;
    } path_options;
};

extern const char* log_level_strings[LOG_LEVEL_COUNT];

void te_log_set_category(const char* category, bool status);
void te_logf(enum te_log_level level, const char* category, const char* fmt, ...);

void logging_default_categories(void);
#endif