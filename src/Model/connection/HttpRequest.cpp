#include "../../stdafx.h"
#include "HttpRequest.h"



HttpRequest::HttpRequest() {
}

HttpRequest::~HttpRequest() {
}

short HttpRequest::getResultCode() {
	return mResultCode; 
};
unsigned char* HttpRequest::getResult() { 
	return mResult;
};
	
HttpRequest* HttpRequest::setUpdateCallback(onUpdateCallback* callback, HttpRequestInterface* caller) {
	mOnUpdateCb = callback; mOnUpdateCaller = caller;
	return this;
};

HttpRequest* HttpRequest::setCompleteCallback(onCompleteCallback* callback, HttpRequestInterface* caller) {
	mOnUpdateCb = callback; mOnUpdateCaller = caller;
	return this;
};
