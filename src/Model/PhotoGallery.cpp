#include "../StdAfx.h"
#include "PhotoGallery.h"
#include "PhotoDirectory.h"
#include "connection\HttpRequestCurl.h"

PhotoGallery::PhotoGallery():
mState(G_STATE_INIT),
mInitRequest(0){

}

PhotoGallery::~PhotoGallery() {

}

void PhotoGallery::update(int dt) {
	switch(mState) {
	case G_STATE_INIT: {
		//Download INIT.json
		if(!mInitRequest) {
			std::cout<<"INIT JSON LOAD";
			std::string initPath;
			initPath = SERVER_BASE_URL
			initPath += "/INIT.json";
			mInitRequest = new HttpRequestCurl();
			mInitRequest->setMethod(GET);
			mInitRequest->setUrl(initPath);
			mInitRequest->addCompleteListener(this);
			mInitRequest->send();
		}
		mInitRequest->update(dt);
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
	std::cout<<"COMPLETE JSON LOAD";
	if(req->getResultCode() == 200) {
		//TODO get data and init PhotoDirectories
		mState = G_STATE_RUN;
	}
}