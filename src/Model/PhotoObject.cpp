#include "../StdAfx.h"
#include "PhotoObject.h"

PhotoObject::PhotoObject():
mInfo(0){

}

PhotoObject::~PhotoObject() {
	if(mInfo) {
		free(mInfo);
	}
}


void PhotoObject::setInfo(char* info, int size) {
	mInfo = (char*)malloc(size);
	memcpy(mInfo, info, size);
}