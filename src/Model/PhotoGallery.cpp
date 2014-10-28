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
	if(mInitRequest) {	
		free(mInitRequest);
	}
	if(!mPhotoDirectories.empty()) {
		PhotoDirectory* pd = mPhotoDirectories.back();
		mPhotoDirectories.pop_back();
		if(pd) {
			free(pd);
		}
	}
	mPhotoDirectories.clear();
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
			initPath += "INIT.json";
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
		std::vector<PhotoDirectory*>::iterator it = mPhotoDirectories.begin();
		while(it != mPhotoDirectories.end()) {
			PhotoDirectory* pd = (*it++);
			pd->update(dt);
		}
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
			if(init.isObject()) {
				boolean nativeConnection = false;
				Json::Value native = init["native_connection"];
				if(native.isBool()) {
					nativeConnection = native.asBool();
				}
				Json::Value files = init["files"];
				if(files.isArray()) {
					Json::Value::iterator it = files.begin();
					while (it != files.end())
					{
						Json::Value file = (*it++);
						if(!file["name"].isString() ||
						   !file["url"].isString() ) {
							   continue;
						}
						std::string name = file["name"].asString();
						std::string url = SERVER_BASE_URL;
						url += file["url"].asString();
#ifdef DEBUG_LOG
						std::cout << "CREATING FILE:" << name << " - URL:" << url << "\n";
#endif
						PhotoDirectory* pd = new PhotoDirectory(name, url, nativeConnection);
						mPhotoDirectories.push_back(pd);
					}
				}
				mState = G_STATE_RUN;
			} else {
				mState = G_STATE_END;
			}
		}
	} else {
		mInitRequest = 0;
	}
	free(req);
}