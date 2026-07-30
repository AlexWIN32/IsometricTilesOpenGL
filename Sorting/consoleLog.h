//taken from https://gist.github.com/JNjenga/195fdca8514c13df82dc6804bde548aa

#include <stdio.h>
#include <string>

#define RESET_COLOR 	"\x1B[0m"
#define INFO_COLOR 	"\x1B[1;34m"
#define DEBUG_COLOR 	"\x1B[1;30m"
#define WARNING_COLOR 	"\x1B[1;33m"
#define ERROR_COLOR 	"\x1B[31m"

#define LOG(f, ...)         log("[LOG]",  RESET_COLOR,f, __VA_ARGS__)
#define LOG_INFO(f, ...)    log("[INFO]", INFO_COLOR, f, __VA_ARGS__)
#define LOG_DEBUG(f, ...)   log("[DEBUG]", DEBUG_COLOR, f, __VA_ARGS__)
#define LOG_WARNING(f, ...) log("[WARNING]", WARNING_COLOR, f, __VA_ARGS__)
#define LOG_ERROR(f, ...)   log("[ERROR]", ERROR_COLOR, f, __VA_ARGS__)

template<typename ... Args>
void log(const char * type, const char * color, const char * s, Args ... args)
{
    printf("%s%s - ", color, type);
    printf(s, args...);
    printf("%s\n", RESET_COLOR);
}
