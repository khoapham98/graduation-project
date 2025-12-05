/**
 * @file    log.h
 * @brief   print log header file
 */
#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <stdarg.h>
#include "device_setup.h"

static inline void log_output(const char *tag, const char *fmt, ...) {
    va_list args;

#if LOG_TO_CONSOLE
    va_start(args, fmt);
    printf("[%s] ", tag);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
#endif

#if LOG_TO_FILE
    FILE *fp = fopen(LOG_FILE_PATH, "a");
    if (fp) {
        va_start(args, fmt);
        fprintf(fp, "[%s] ", tag);
        vfprintf(fp, fmt, args);
        fprintf(fp, "\n");
        fclose(fp);
        va_end(args);
    }
#endif
}

#define LOG_INF(fmt, ...) log_output("INF", fmt, ##__VA_ARGS__)
#define LOG_WRN(fmt, ...) log_output("WRN", fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) log_output("ERR", fmt, ##__VA_ARGS__)

#endif