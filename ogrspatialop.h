/// @file ogrspatialop.h
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#ifndef OGR_SPATIALOP_H_
#define OGR_SPATIALOP_H_

#include "ogrsf_frmts.h"

OGRLayer *ComputeBuffer(OGRLayer *poLayer, int radius_);

#endif /* OGRSPATIALOP_H_ */
