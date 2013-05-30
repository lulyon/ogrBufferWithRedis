/// @file ogrshapefileio.h
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#ifndef OGR_SHPAEFILE_IO_H_
#define OGR_SHPAEFILE_IO_H_

OGRLayer *ReadLayerAtPath(const char *layerPath);
void WriteLayerAtPath(OGRLayer *outLayer, const char *outSourceString);

#endif
