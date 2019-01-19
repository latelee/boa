/**
 * @file   log_utils.h
 * @author Late Lee 
 * @date   2013-04-30
 * @brief  
 *         ��־����
 *
 * @note   �ֵȼ�������û������������ʽ��Ĭ�ϴ�ӡ���ն�
 *
 * @log
 *         2013-05-01
 *            �޸���־�ص���������ʽ������
 *            ��־�ص�������ӵȼ�����
 *
 */

#ifndef _LOG_UTILS_H
#define _LOG_UTILS_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// ����˺꣬���ӡ��Ϣ��ʱ���
#define NEED_TIMESTAMP

// Ĭ�ϵȼ�
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
 * @brief ������־����ʽ�ص�����
 * 
 * @param fn[in] : �ص�����ָ��
 *
 * @note
 *        ���û������ã���ʹ��Ĭ�ϴ���ʽ������ӡ���ն�
 */
void log_set_callback(log_handle_fn_t fn);

/** 
 * @brief ���ô�ӡ��־�ȼ�
 * 
 * @param level[in] : �ȼ�
 *
 */
void log_set_level(int level);

/** 
 * @brief ��־��ӡ������
 * 
 * @param level[in] : �ȼ�
 * @param fmt[in]   : �ɱ����
 *
 */
void log_trace(int level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // _LOG_UTILS_H
