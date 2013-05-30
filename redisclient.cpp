/// @file redisclient.cpp
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <hiredis.h>

#include "redisclient.h"

bool redisClient::connect(const char *ip, int port, int dbno) {
  this->con = redisConnect(ip, port);
	if (this->con == NULL || con->err) {
		printf("Connection error: %s\n", con->errstr);
		if (con)
			redisFree(con);
		return false;
	}
	redisReply *reply = redisCommand(con, "select %d", dbno);
	if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
		printf("Select db error: %s\n", reply->str);
		if (reply)
			freeReplyObject(reply);
		return false;
	}
	freeReplyObject(reply);
	return true;
}

char *redisClient::get(const char *key, int *pSize) {
	if (this->con == NULL) {
		printf("Redis connection is not available\n");
		return NULL;
	}
	redisReply *reply = redisCommand(con, "GET %s", key);
	if (reply == NULL || reply->type != REDIS_REPLY_STRING) {
		printf("Redis reply error: not a string\n");
		if (reply)
			freeReplyObject(reply);
		return NULL;
	}
	if (pSize)
		*pSize = reply->len;
	char *result = (char *)malloc(reply->len * sizeof(char));
	if(result == NULL) {
		printf("redis get result malloc failed\n");
		return NULL;
	}
	memcpy(result, reply->str, reply->len);

	freeReplyObject(reply);
	return result;
}

bool redisClient::put(const char *key, const char *value, int size) {
	if (this->con == NULL) {
		printf("Redis connection is not available\n");
		return false;
	}
	redisReply *reply = NULL;
	if (size) {
		reply = redisCommand(con, "SET %s %b", key, value, size);
	} else {
		reply = redisCommand(con, "SET %s %s", key, value);
	}

	if (reply == NULL || reply->type == REDIS_REPLY_ERROR) {
		printf("Redis set command error: %s\n", reply->str);
		if (reply)
			freeReplyObject(reply);
		return false;
	}

	freeReplyObject(reply);
	return true;
}
