#include "../StdAfx.h"
#include "PhotoGallery.h"
#include "PhotoDirectory.h"

PhotoGallery::PhotoGallery():
mState(G_STATE_INIT) {

}

PhotoGallery::~PhotoGallery() {

}

void PhotoGallery::update(int dt) {
	switch(mState) {
	case G_STATE_INIT: {
		//Download INIT.json
		break;
	}
	case G_STATE_RUN: {
		//ImageDirectories created and downloading by theirselves
		break;
	}
	case G_STATE_END: {
		break;
	}
	}
}

void PhotoGallery::onUpdate(HttpRequest* req){
	//do nothing
}

void PhotoGallery::onComplete(HttpRequest* req){
	if(req->getResultCode() == 200) {
		//TODO get data and init PhotoDirectories
		mState = G_STATE_RUN;
	}
}