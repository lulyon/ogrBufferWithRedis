/// @file ogrserialize.cpp
/// @author yuanman@ict.ac.cn luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#include "ogrsf_frmts.h"
#include "ogrserialize.h"

size_t SizeOfRegion(OGRLayer *layer) {
  size_t type_size = sizeof(OGRwkbGeometryType);
	size_t count_size = sizeof(int);
	size_t size = type_size + count_size;

	layer->ResetReading();
	for (OGRFeature *feature = layer->GetNextFeature(); feature != NULL;
			feature = layer->GetNextFeature()) {
		OGRGeometry *geometry = feature->GetGeometryRef();
		if (geometry) {
			size += geometry->WkbSize();
		}
		OGRFeature::DestroyFeature(feature);
	}

	return size;
}


unsigned char *BytesOfRegion(OGRLayer *layer, size_t size) {
	unsigned char *bytes = (unsigned char *) malloc(size);
	size_t offset = 0;

	size_t type_size = sizeof(OGRwkbGeometryType);
	OGRwkbGeometryType type = layer->GetGeomType();
	memcpy(bytes + offset, &type, type_size);
	offset += type_size;

	size_t count_size = sizeof(int);
	int count = layer->GetFeatureCount();
	memcpy(bytes + offset, &count, count_size);
	offset += count_size;

	layer->ResetReading();
	for (OGRFeature *feature = layer->GetNextFeature(); feature != NULL;
			feature = layer->GetNextFeature()) {
		OGRGeometry *geometry = feature->GetGeometryRef();
		if (geometry) {
			geometry->exportToWkb(wkbNDR, bytes + offset);
			offset += geometry->WkbSize();
		} else {
			(*(int *) (bytes + type_size))--;
		}
		OGRFeature::DestroyFeature(feature);
	}

	return bytes;
}

OGRLayer *RegionWithBytesAndSize(unsigned char *bytes, size_t size) {
	size_t type_size = sizeof(OGRwkbGeometryType);
	size_t count_size = sizeof(int);
	size_t offset = type_size + count_size;

	if (size < offset) {
		return NULL;
	}

	OGRwkbGeometryType type;
	memcpy(&type, bytes, type_size);

	int count;
	memcpy(&count, bytes + type_size, count_size);
	if (count < 0) {
		return NULL;
	}

//	OGRLayer *layer = new OGRMemLayer("", NULL, type);
	OGRSFDriver *pdriver =
			OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("Memory");
	if (!pdriver)
		return NULL;

	OGRDataSource *pds = pdriver->CreateDataSource("DRAMA");
	if (!pds)
		return NULL;

	OGRLayer *layer = pds->CreateLayer("", NULL, type);
	if (!layer)
		return NULL;

	OGRFeatureDefn *defn = layer->GetLayerDefn();
	for (int i = 0; i < count && offset < size; i++) {
		OGRGeometry *geometry = NULL;
		switch (type) {
		case wkbPoint:
		case wkbPoint25D:
			geometry = new OGRPoint();
			break;
		case wkbLineString:
		case wkbLineString25D:
			geometry = new OGRLineString();
			break;
		case wkbPolygon:
		case wkbPolygon25D:
			geometry = new OGRPolygon();
			break;
		case wkbMultiPoint:
		case wkbMultiPoint25D:
			geometry = new OGRMultiPoint();
			break;
		case wkbMultiLineString:
		case wkbMultiLineString25D:
			geometry = new OGRMultiLineString();
			break;
		case wkbMultiPolygon:
		case wkbMultiPolygon25D:
			geometry = new OGRMultiPolygon();
			break;
		case wkbGeometryCollection:
		case wkbGeometryCollection25D:
			geometry = new OGRGeometryCollection();
			break;
		default:
			break;
		}
		geometry->importFromWkb(bytes + offset);
		OGRFeature *feature = new OGRFeature(defn);
		feature->SetGeometryDirectly(geometry);
		layer->CreateFeature(feature);
		offset += geometry->WkbSize();
	}
	return layer;
}
