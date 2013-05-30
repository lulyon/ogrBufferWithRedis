/// @file redisclient.h
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <hiredis.h>

class redisClient {
public:
  redisClient() :
			con(NULL) {
	}
	;
	~redisClient() {
		if (this->con) {
			redisFree(con);
			con = NULL;
		}
	}
	;
	void disconnect() {
		if (this->con) {
			redisFree(con);
			con = NULL;
		}
	}
	;

	bool connect(const char *ip = "127.0.0.1", int port = 6379, int dbno = 1);
	char *get(const char *key, int *size = NULL);
	bool put(const char *key, const char *value, int size = 0);
private:
	redisContext *con;
};

#endif
