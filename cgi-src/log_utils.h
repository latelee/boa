/**
 * @file   log_utils.h
 * @author Late Lee 
 * @date   2013-04-30
 * @brief  
 *         日志函数
 *
 * @note   分等级输出，用户可设置输出方式，默认打印到终端
 *
 * @log
 *         2013-05-01
 *            修改日志回调函数，格式更自由
 *            日志回调函数添加等级参数
 *
 */

#ifndef _LOG_UTILS_H
#define _LOG_UTILS_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// 定义此宏，则打印信息含时间戳
#define NEED_TIMESTAMP

// 默认等级
#define DEFAULT_LOG_LEVEL LOG_DEBUG

// log level
#define LOG_QUIET -1
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARN  3
#define LOG_INFO  4
#define LOG_DEBUG 5
#define LOG_INDEBUG 6

typedef void (*log_handle_fn_t)(int level, const char* fmt, va_list vl);

/** 
 * @brief 设置日志处理方式回调函数
 * 
 * @param fn[in] : 回调函数指针
 *
 * @note
 *        如用户不设置，则使用默认处理方式，即打印到终端
 */
void log_set_callback(log_handle_fn_t fn);

/** 
 * @brief 设置打印日志等级
 * 
 * @param level[in] : 等级
 *
 */
void log_set_level(int level);

/** 
 * @brief 日志打印函数。
 * 
 * @param level[in] : 等级
 * @param fmt[in]   : 可变参数
 *
 */
void log_trace(int level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // _LOG_UTILS_H
