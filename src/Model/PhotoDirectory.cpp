#include "../StdAfx.h"
#include "PhotoDirectory.h"
#include "PhotoObject.h"
#include "connection\HttpRequestCurl.h"
#include "utils\Unzipper.h"

PhotoDirectory::PhotoDirectory(std::string name, std::string url, bool native):
mState(D_STATE_INIT),
mName(name),
mUrl(url),
mProgress(0.0){
	if(native) {
		//TODO create native request
	} else {
		mRequest = new HttpRequestCurl();
	}
}

PhotoDirectory::~PhotoDirectory() {
	if(mRequest) {
		free(mRequest);
	}
	if(!mPhotos.empty()) {
		PhotoObject* pd = mPhotos.back();
		mPhotos.pop_back();
		if(pd) {
			free(pd);
		}
	}
	mPhotos.clear();
}

void PhotoDirectory::update(int dt) {
	switch(mState) {
	case D_STATE_INIT: {
		mRequest->setUrl(mUrl);
		mRequest->setMethod(GET);
		mRequest->addUpdateListener(this);
		mRequest->addCompleteListener(this);
		unsigned long urllen = mRequest->getContentLength();
		if(urllen == getDownloadedFileSize()) { //TODO fix
			free(mRequest);
			mState = D_STATE_UNZIP;
		} else {
			mRequest->send();
			mState = D_STATE_DOWNLOAD;
		}
		break;
	}
	case D_STATE_DOWNLOAD: {
		mRequest->update(dt);
		break;
	}
	case D_STATE_UNZIP: {
		Unzipper un(getFileName());
		int len = un.getDirectorySize();
		for(int i=0; i<len; i++) {
			PhotoObject* po = new PhotoObject();
			po->setInfo(un.getFileData(i), un.getFileSize(i));
			mPhotos.push_back(po);
		}
		mState = D_STATE_DONE;
		break;
	}
	case D_STATE_DONE: {
		break;
	}
	case D_STATE_ERROR: {
		break;
	}
	}
}

void PhotoDirectory::onUpdate(HttpRequest* req){
	char* result;
	size_t dsize = req->getResult(result);
	long lenf = req->getContentLength();
	mProgress = (float)dsize / (float)lenf;
#ifdef DEBUG_LOG
	std::cout << "file:" << mName << " - " << (mProgress * 100) <<"...\n";
#endif
	std::ofstream file(getFileName(), std::ios::binary|std::ios::ate);
	if(file.is_open()) {
		std::streampos fsize = file.tellp();
		int len = dsize - fsize;
		file.write(&result[fsize], len);
		file.close();
	}
}

void PhotoDirectory::onComplete(HttpRequest* req){
	mState = D_STATE_UNZIP;
#ifdef DEBUG_LOG
	std::cout << "file:" << mName << " DONE...\n";
#endif
	free(req);
}

std::string PhotoDirectory::getFileName() {
	std::ostringstream fnameStream;
	std::string fname(mName);
	std::replace(fname.begin(), fname.end(),' ', '_'); 
	fnameStream << WORKING_DIR << fname << ".zip";
	return fnameStream.str();
}

size_t	PhotoDirectory::getDownloadedFileSize() {
	std::streampos size = 0;
	std::ifstream file (getFileName(), std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
	}
	return (size_t) size;
}