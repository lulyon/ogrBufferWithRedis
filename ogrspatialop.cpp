/// @file ogrspatialop.cpp
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#include "ogrspatialop.h"
#include "ogrsf_frmts.h"


OGRLayer *ComputeBuffer(OGRLayer *poLayer, int radius_) {
	OGRFeatureDefn *input_defn = poLayer->GetLayerDefn();
	int featureCount = poLayer->GetFeatureCount();
	poLayer->ResetReading();
	int iFeature = 0;

	OGRSFDriver *pdriver =
			OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("Memory");
	if (!pdriver)
		return NULL;

	OGRDataSource *pds = pdriver->CreateDataSource("DRAMA");
	if (!pds)
		return NULL;

	OGRLayer *newLayer = pds->CreateLayer(poLayer->GetName(),
			poLayer->GetSpatialRef(), wkbMultiPolygon);
	if (!newLayer)
		return NULL;

	for (OGRFeature *poFeature = poLayer->GetNextFeature();
			poFeature != NULL && iFeature < featureCount;
			iFeature++, poFeature = poLayer->GetNextFeature()) {

		OGRGeometry *poGeometry = poFeature->GetGeometryRef();

		OGRGeometry *buffer = NULL;
		OGRGeometry *buffer_raw = poGeometry->Buffer(radius_);

		switch (buffer_raw->getGeometryType()) {
		case wkbMultiPolygon:
			buffer = buffer_raw;
			break;
		case wkbPolygon:
			buffer = new OGRMultiPolygon();
			(dynamic_cast<OGRMultiPolygon *>(buffer))->addGeometryDirectly(
					buffer_raw);
			break;
		default:
			break;
		}

		if (buffer != NULL) {
			OGRFeature *new_feature = new OGRFeature(input_defn);
			new_feature->SetGeometryDirectly(buffer);
			newLayer->CreateFeature(new_feature);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	return newLayer;
}
