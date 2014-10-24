#include "../StdAfx.h"
#include "PhotoGallery.h"
#include "PhotoDirectory.h"
#include "connection\HttpRequestCurl.h"
#include "json\json.h"

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
#ifdef DEBUG_LOG
			std::cout<<"INIT JSON LOAD...\n";
#endif
			std::string initPath;
			initPath = SERVER_BASE_URL
			initPath += "/INIT.json";
			mInitRequest = new HttpRequestCurl();
			mInitRequest->setMethod(GET);
			mInitRequest->setUrl(initPath);
			mInitRequest->addCompleteListener(this);
			mInitRequest->send();
			return;
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
#ifdef DEBUG_LOG
	std::cout<<"COMPLETE JSON LOAD\n";
#endif
	if(req->getResultCode() == 200) {
		char* result;
		size_t len = req->getResult(result);
		std::string jsonstr(result);

		Json::Reader reader;
		Json::Value init;
		if(reader.parse(jsonstr,init)) {
			//TODO create PhotoDirs
			mState = G_STATE_RUN;
		}
	} else {
		mInitRequest = 0;
	}
	free(req);
}