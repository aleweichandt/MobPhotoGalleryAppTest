#include "../../stdafx.h"
#include "HttpRequest.h"



HttpRequest::HttpRequest():
mMethod(GET),
mBody(0),
mResultCode(0),
mResult(0),
mResultSize(0),
mState(HRS_ERROR),
mBytesOffset(0),
mContentLength(0)
{
}

HttpRequest::~HttpRequest() {
	mHeaders.clear();
	if(mResult) {
		free(mResult);
	}
	if(mBody) {
		free(mBody);
	}
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
	mBytesOffset = offset;
}

unsigned long HttpRequest::getContentLength() {
	if(!mContentLength) {
		_performHEAD();
		if(mResultCode != 200) {
			return 0;
		}
	}
	return mContentLength;
}

unsigned int HttpRequest::getResultCode() {
	return mResultCode; 
};

size_t HttpRequest::getResult(char* &result) { 
	result = mResult;
	return mResultSize;
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
			if(!mContentLength || mMethod != POST) {
				_performHEAD();
				mState = (mResultCode==200)?HRS_RUN:HRS_DONE;
			} else {
				mState = HRS_RUN;
			}
			break;
		}
		case HRS_RUN: {
			if(mMethod == POST) {
				_performPOST();
				mState = HRS_DONE;
			} 
			if(mMethod == GET) {
				int bytes = (dt?dt:1) * RANGE_SPEED;
				_performRangeGET(bytes);
				if(mResultCode != 206) {
					mState = HRS_DONE;
				} else {
					if(mResultSize >= mContentLength) {
						mResultCode = 200;
						mState = HRS_DONE;
					}
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
