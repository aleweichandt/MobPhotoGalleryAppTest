#include "../../stdafx.h"
#include "HttpRequest.h"



HttpRequest::HttpRequest():
mMethod(GET),
mBody(0),
mResultCode(0),
mResult(0),
mState(HRS_ERROR),
mContentCurrent(0)
{
}

HttpRequest::~HttpRequest() {
	mHeaders.clear();
}

void HttpRequest::setUrl(std::string url) {
	mUrl = url;
}

void HttpRequest::setMethod(HTTP_METHOD method) {
	mMethod = method;
}

void HttpRequest::setHeader(std::string headerName, std::string headerValue) {
	mHeaders[headerName] = headerValue;
}

void HttpRequest::setBody(const char* body) {
	memcpy(mBody, body, sizeof(body)*sizeof(unsigned char));
}

void HttpRequest::setRangeBytesOffset(unsigned long offset) {
	mContentCurrent = offset;
}

unsigned int HttpRequest::getResultCode() {
	return mResultCode; 
};

char* HttpRequest::getResult() { 
	return mResult;
};
	
void HttpRequest::addUpdateListener(HttpRequestInterface* caller) {
	mOnUpdateCaller = caller;
};

void HttpRequest::addCompleteListener(HttpRequestInterface* caller) {
	mOnCompleteCaller = caller;
};

void HttpRequest::send(bool async)
{
	if(!async) {
		if(mMethod == POST) {
			_performPOST();
		} else {
			_performGET();
		}
	} else {
		mState = HRS_INIT;
	}
}

void HttpRequest::update(int dt) {
	switch(mState) {
		case HRS_INIT: {
			//setup
			mState = HRS_RUN;
			break;
		}
		case HRS_RUN: {
			if(mMethod == POST) {
				_performPOST();
				mState = HRS_DONE;
			} else {
				int bytes = dt * RANGE_SPEED;
				_performRangeGET(bytes);
				if(this->getResultCode() != 206) {
					mState = HRS_DONE;
				} else {
					if(mOnUpdateCaller) {
						mOnUpdateCaller->onUpdate(this);
					}
				}
			}
			break;
		}
		case HRS_DONE: {
			if(mOnCompleteCaller) {
				mOnCompleteCaller->onComplete(this);
			}
			break;
		}
		case HRS_ERROR: {
			break;
		}
	}
}
