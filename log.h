#pragma once

#include <stdio.h>
#include <stdlib.h>

enum log_level {
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,
};

char *log_prefix(enum log_level, const char *);

#define LOG(level, str, ...) \
	do { \
        char *p = log_prefix(LOG_LEVEL_##level, __func__); \
        fprintf(stderr, "%s " str "\n", p, ##__VA_ARGS__); \
	} while (0)

#define log_info(s, ...)  LOG(INFO,  s, ##__VA_ARGS__)
#define log_warn(s, ...)  LOG(WARN,  s, ##__VA_ARGS__)
#define log_error(s, ...) LOG(ERROR, s, ##__VA_ARGS__)
#define log_fatal(s, ...) LOG(FATAL, s, ##__VA_ARGS__)


