/// @file ogrshapefileio.cpp
/// @author luliang@ict.ac.cn
/// @copybrief Copyright 2012 ICT, CAS. All rights reserved.
/// @version 0.6
/// @date 2013-05-15

#include "ogrsf_frmts.h"
#include "ogrshapefileio.h"

#include <assert.h>

OGRLayer *ReadLayerAtPath(const char *layerPath) {
  OGRRegisterAll();

	std::string pszName(layerPath);

	OGRDataSource *poDS = OGRSFDriverRegistrar::Open(pszName.c_str(), FALSE);
	if (!poDS) {
		printf("data source %s is corrupt.\n", pszName.c_str());
		return NULL;
	}

	OGRLayer *poLayer = poDS->GetLayer(0);
	assert(poLayer);

	return poLayer;
}

void WriteLayerAtPath(OGRLayer *outLayer, const char *outSourceString) {
	OGRRegisterAll();
	std::string pszName(outSourceString);
	OGRSFDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
					"ESRI Shapefile");

	OGRDataSource *poDS = poDriver->CreateDataSource(pszName.c_str());
	if (!poDS) {
		printf("data source %s is corrupt.\n", pszName.c_str());
		return;
	}

	char **papszOptions = NULL;
	papszOptions = CSLSetNameValue(papszOptions, "OVERWRITE", "YES");

	poDS->CopyLayer(outLayer, pszName.c_str(),
			papszOptions);
	CSLDestroy(papszOptions);

	OGRDataSource::DestroyDataSource(poDS);
}
