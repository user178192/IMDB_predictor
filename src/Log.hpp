/*
��ģ�鲻��һ���ࣨ��ʷԭ�򣩣�����ȫ�ֺ����ͺ�
���Զ�̬����������
�̰߳�ȫ

by fusiyuan2010@gmail.com
*/
#ifndef _LOG_H
#define _LOG_H

enum LOG_LEVEL
{
		LTRACE,
		LDEBUG,
		LINFO,
        LWARN,
		LERROR,
        LDUMP,
};


int LogInit(const char *log_file, int level);
int LogMsg(int curlevel,const char *sourceName,int sourceLine,const char *functionName,const char *szFormat, ...);
int LogDestroy();


#ifdef _LOG_NONE
#define LOG_TRACE(format,...)
#define LOG_WARN(format,...)
#define LOG_DEBUG(format,...)
#define LOG_WARN(format,...)
#define LOG_INFO(format,...)
#define LOG_ERROR(format,...)
#define LOG_DUMP(format,...)
#endif

#ifdef _LOG_DEBUG
#define LOG_TRACE(format,...)
#define LOG_DEBUG(format,...) LogMsg(LDEBUG,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_INFO(format,...) LogMsg(LINFO,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_WARN(format,...) LogMsg(LWARN,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_ERROR(format,...) LogMsg(LERROR,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif

#ifdef _LOG_INFO
#define LOG_TRACE(format,...)
#define LOG_DEBUG(format,...)
#define LOG_INFO(format,...) LogMsg(LINFO,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_WARN(format,...) LogMsg(LWARN,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_ERROR(format,...) LogMsg(LERROR,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif

#ifdef _LOG_WARN
#define LOG_TRACE(format,...)
#define LOG_DEBUG(format,...)
#define LOG_INFO(format,...)
#define LOG_WARN(format,...) LogMsg(LWARN,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_ERROR(format,...) LogMsg(LERROR,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif
#ifdef _LOG_ERROR
#define LOG_TRACE(format,...)
#define LOG_DEBUG(format,...)
#define LOG_INFO(format,...)
#define LOG_WARN(format,...)
#define LOG_ERROR(format,...) LogMsg(LERROR,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif

#ifdef _LOG_DUMP
#define LOG_TRACE(format,...)
#define LOG_DEBUG(format,...)
#define LOG_INFO(format,...)
#define LOG_WARN(format,...)
#define LOG_ERROR(format,...)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif 

#ifndef LOG_TRACE
#define LOG_TRACE(format,...) LogMsg(LTRACE,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DEBUG(format,...) LogMsg(LDEBUG,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_INFO(format,...) LogMsg(LINFO,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_WARN(format,...) LogMsg(LWARN,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_ERROR(format,...) LogMsg(LERROR,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#define LOG_DUMP(format,...) LogMsg(LDUMP,__FILE__,__LINE__,__PRETTY_FUNCTION__ ,format,##__VA_ARGS__)
#endif


#endif
