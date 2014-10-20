#include "StdAfx.h"
#include "PhotoGallery.h"
#include "HttpRequest.h"

PhotoGallery::PhotoGallery():
mState(STATE_INIT) {

}

PhotoGallery::~PhotoGallery() {

}

void PhotoGallery::update(int dt) {
	switch(mState) {
	case STATE_INIT: {
		//Download INIT.json
		break;
	}
	case STATE_RUN: {
		//ImageDirectories created and downloading by theirselves
		break;
	}
	case STATE_END: {
		break;
	}
}

void PhotoGallery::onInitDownloaded(HttpRequest request) {
	if(request.getResultCode() == 200) {
		//TODO get data and init PhotoDirectories
		mState = STATE_RUN;
	}
}