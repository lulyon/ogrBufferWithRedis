/// @file ogrserialize.h
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#ifndef OGR_SERIALIZE_H_
#define OGR_SERIALIZE_H_

size_t SizeOfRegion(OGRLayer *layer);

unsigned char *BytesOfRegion(OGRLayer *layer, size_t size);

OGRLayer *RegionWithBytesAndSize(unsigned char *bytes, size_t size);

#endif
