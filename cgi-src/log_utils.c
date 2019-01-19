#include "log_utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define LINE_SIZE 1024

static int g_log_level = DEFAULT_LOG_LEVEL;

// 自实现的
static int mysprintf(char *buffer, int len, const char* fmt, ...)
{
    va_list ap;
     int ret = 0;

    va_start(ap, fmt);
#ifdef WIN32
     ret = _vsnprintf(buffer, len, fmt, ap);
#else
    ret = vsnprintf(buffer, len, fmt, ap);
#endif
    va_end(ap);
    if (ret > len - 1)
        ret = len - 1;
    /* 当指定长度太小时，返回-1，非实际长度(VS 2003) */
    else if (ret == -1)
        ret = len - 1;

    buffer[ret] = '\0';
    //buffer[len -1] = '\0';

     return ret;
}

// from ffmpeg
static int avpriv_vsnprintf(char *s, size_t n, const char *fmt,
                     va_list ap)
{
    int ret;
    va_list ap_copy;

    if (n == 0)
#ifdef WIN32
        return _vscprintf(fmt, ap);
#else
        return vsnprintf(NULL, 0, fmt, ap);
#endif

    else if (n > INT_MAX)
        //return AVERROR(EOVERFLOW);
        return -1;

    /* we use n - 1 here because if the buffer is not big enough, the MS
     * runtime libraries don't add a terminating zero at the end. MSDN
     * recommends to provide _snprintf/_vsnprintf() a buffer size that
     * is one less than the actual buffer, and zero it before calling
     * _snprintf/_vsnprintf() to workaround this problem.
     * See http://msdn.microsoft.com/en-us/library/1kt27hek(v=vs.80).aspx */
    memset(s, 0, n);
    va_copy(ap_copy, ap);
    //ap_copy = ap;
#ifdef WIN32
    ret = _vsnprintf(s, n - 1, fmt, ap_copy);
#else
    ret = vsnprintf(s, n - 1, fmt, ap_copy);
#endif
    va_end(ap_copy);
    if (ret == -1)
#ifdef WIN32
        ret = _vscprintf(fmt, ap);
#else
        ret = vsnprintf(NULL, 0, fmt, ap);
#endif
    return ret;
}

// from ffmpeg
static int avpriv_snprintf(char *s, size_t n, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = avpriv_vsnprintf(s, n, fmt, ap);
    va_end(ap);

    return ret;
}

static void get_timestamp(char *buffer)
{
    time_t t;
    struct tm *p;
    int len;

    // UTC时间，必须有时区文件
#ifdef TIME_UTC
    struct tm tmp_tm = {0};
    t = time(NULL);
    p = localtime_r(&t, &tmp_tm);
#else
    t = time(NULL);
    p = localtime(&t);
#endif
    /* 时间格式：[2011-11-15 12:47:34] */
    len = avpriv_snprintf(buffer, 23, "[%04d-%02d-%02d %02d:%02d:%02d] ",
        p->tm_year+1900, p->tm_mon+1,
        p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    buffer[len] = '\0';
}

////////////////////////////////////////////////////////////////////////////

// 默认输出函数，打印到终端
static void log_default_callback(int level, const char* fmt, va_list vl)
{
    char buffer[LINE_SIZE] = {0};
    char line[LINE_SIZE] = {0};
    int ret = -1;

#ifdef NEED_TIMESTAMP
    char time_buffer[32] = {0};
#endif

    if (level > g_log_level)
    {
        return;
    }

#ifdef NEED_TIMESTAMP
    get_timestamp(time_buffer);
    //memcpy(buffer, time_buffer, 22);
    ret = vsprintf(buffer, fmt, vl);
    avpriv_snprintf(line, sizeof(line), "%s%s", time_buffer, buffer);
#else
    ret = vsprintf(buffer, fmt, vl);
    avpriv_snprintf(line, sizeof(line), "%s", buffer);
#endif

    fputs(line, stdout);
}

static log_handle_fn_t log_callback = log_default_callback;

void log_set_callback(log_handle_fn_t fn)
{
    log_callback = fn;
}

void log_set_level(int level)
{
    g_log_level = level;
}

int log_get_level(void)
{
    return g_log_level;
}

void log_trace(int level, const char* fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);

    if (log_callback)
    {
        log_callback(level, fmt, vl);
    }

    va_end(vl);
}
