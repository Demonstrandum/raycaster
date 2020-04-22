#include "log.h"

#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"
#define RESET "\033[0m"


char *log_prefix(enum log_level level, const char *func) {
    char *level_str = "";

    switch (level) {
    case LOG_LEVEL_INFO:
        level_str = CYAN "INFO";
        break;
    case LOG_LEVEL_WARN:
        level_str = YELLOW "WARNING";
        break;
    case LOG_LEVEL_ERROR:
        level_str = MAGENTA "ERROR";
        break;
    case LOG_LEVEL_FATAL:
        level_str = RED "FATAL ERROR";
        break;
    default:
        level_str = RED "UNKNOWN ERROR";
        break;
    }

    char *s = (char *)malloc(512 * sizeof(char));
    sprintf(s, BOLD "[#] [log: %s%s -- in `%s`]:%s",
            level_str,
            RESET BOLD,
            func, RESET);
    return s;
}
