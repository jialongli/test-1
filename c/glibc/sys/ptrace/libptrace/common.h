#ifndef _COMMON_H
#define _COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define debug(fmt...) do{fprintf(stderr, "[%s:%s %d]", __FILE__, __func__, __LINE__);fprintf(stderr, fmt);}while(0)

#define ERR(fmt, args...) fprintf(stderr, fmt, ##args);
#define MSG(fmt, args...) fprintf(stderr, fmt, ##args);

#define IF(con, ret) if(con) { return ret; }
#define IFMSG(con, ret, fmt, args...) \
	if(con) \
	{ \
		if(strlen(fmt) > 0) \
			MSG(fmt, ##args); \
		if(errno != 0) \
			ERR(": %s\r\n", strerror(errno)); \
		return ret; \
	}
#define GO(con, label) if(con) { goto label; }
#endif
