#ifndef TE_LOGGING_H
#define TE_LOGGING_H

enum te_log_level {
    LOG_LEVEL_NONE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
};

enum te_log_filter_state {
    LOG_LEVEL_FILTER_ENABLED,
    LOG_LEVEL_FILTER_FILE_ONLY,
    LOG_LEVEL_FILTER_DISABLED,
};


struct te_log_context {
    enum te_log_level min;
    int unfiltered_fd;
    int display_fd;

    const char* categories[100];
    enum te_log_filter_state category_disabled[100];

    struct {
        char* directory;
        bool timestamp;
    } path_options;
};

extern const char* log_level_strings[LOG_LEVEL_COUNT];

void te_logf(enum te_log_level level, const char* category, const char* fmt, ...);
void te_log_set_category(const char* category, enum te_log_filter_state level);
void logging_default_categories(void);
#endif