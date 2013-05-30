/// @file ogrbuffertest.cpp
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#include "ogrserialize.h"
#include "ogrshapefileio.h"
#include "redisclient.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>

#define STRLEN 1000

void Test1(const char *layername) {
  char *filepath_tail = ".shp";

//	char *layername = "Hunan_EPSG3785";

	char filepath[STRLEN] = "/home/luliang/data/";
	char outpath[STRLEN] = "/home/luliang/result/";

	strcat(filepath, layername);
	strcat(filepath, filepath_tail);

	strcat(outpath, layername);
	strcat(outpath, "_1");
	strcat(outpath, filepath_tail);

	int radius_ = 100000;

	OGRLayer *poLayer = ReadLayerAtPath(filepath);

	OGRLayer *newLayer = ComputeBuffer(poLayer, radius_);

	WriteLayerAtPath(newLayer, outpath);

	if (poLayer) {
		delete poLayer;
		poLayer = 0;
	}
	if (newLayer) {
		delete newLayer;
		newLayer = 0;
	}
}

void Test2(const char *layername) {
	char *filepath_tail = ".shp";

	//	char *layername = "Hunan_EPSG3785";

	char filepath[STRLEN] = "/home/luliang/data/";
	char outpath[STRLEN] = "/home/luliang/result/";

	strcat(filepath, layername);
	strcat(filepath, filepath_tail);

	strcat(outpath, layername);
	strcat(outpath, "_2");
	strcat(outpath, filepath_tail);

	int radius_ = 100000;
	const char *key = layername;

	{
		OGRLayer *poLayer = ReadLayerAtPath(filepath);
		size_t size = SizeOfRegion(poLayer);
		char *value = BytesOfRegion(poLayer, size);

		redisClient client;
		client.connect();
		client.put(key, value, size);
		free(value);

		if (poLayer) {
			delete poLayer;
			poLayer = 0;
		}

	}

	{
		int size;
		redisClient client;
		client.connect();
		char *value = client.get(key, &size);
		OGRLayer *poLayer = RegionWithBytesAndSize((unsigned char *) value,
				size);

		OGRLayer *newLayer = ComputeBuffer(poLayer, radius_);

		size_t newsize = SizeOfRegion(newLayer);
		char *newvalue = BytesOfRegion(newLayer, newsize);
		char *newkey = (char *)malloc(STRLEN);
		strcpy(newkey, layername);
		strcat(newkey, "_buffer");

		redisClient client;
		client.connect();
		client.put(newkey, newvalue, newsize);
		free(newkey);
		free(newvalue);

		WriteLayerAtPath(newLayer, outpath);

		if (poLayer) {
			delete poLayer;
			poLayer = 0;
		}

		if (newLayer) {
			delete newLayer;
			newLayer = 0;
		}
	}

}

int main() {
	char *layername = "Hunan_EPSG3785";
	Test1(layername);
	Test2(layername);

	return 0;
}
