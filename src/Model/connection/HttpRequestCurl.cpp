#include "../../StdAfx.h"
#include "HttpRequestCurl.h"

HttpRequestCurl::HttpRequestCurl(void)
:HttpRequest()
{
	curl_global_init(CURL_GLOBAL_ALL);
	mCurl = curl_easy_init();
	curl_easy_setopt(mCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

HttpRequestCurl::~HttpRequestCurl(void)
{
	curl_easy_cleanup(mCurl);
	curl_global_cleanup();
}

void HttpRequestCurl::_performHEAD() {
	curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str());
	curl_easy_setopt(mCurl, CURLOPT_NOBODY, 1L);
	CURLcode resp = curl_easy_perform(mCurl);
	if(resp == CURLE_OK) {
		long respcode;
		curl_easy_getinfo(mCurl,CURLINFO_RESPONSE_CODE, &respcode);
		mResultCode = (unsigned int) respcode;
		if(respcode == 200) {
			double filesize = 0.0;
			curl_easy_getinfo(mCurl,CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);
			mContentLength = (unsigned long)filesize;
		}
	}
}

void HttpRequestCurl::_performGET() {
	if(!mResult) {
		mResult = (char*)malloc(0);
		mResultSize = 0;
	}
	struct MemoryStruct chunk;
	chunk.memory = mResult;
	chunk.size = mResultSize; 

	curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str());
	curl_easy_setopt(mCurl, CURLOPT_HTTPGET, 1L);
	//TODO add headers
	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, (void *)&chunk);
	CURLcode resp = curl_easy_perform(mCurl);
	if(resp == CURLE_OK) {
		long respcode;
		curl_easy_getinfo(mCurl,CURLINFO_RESPONSE_CODE, &respcode);
		mResultCode = (unsigned int) respcode;
	}
	mResult = chunk.memory;
	mResultSize = chunk.size;
	mBytesOffset = chunk.size;
}

void HttpRequestCurl::_performRangeGET(int range) {
	unsigned long nextOffset = range + mBytesOffset;
	if(nextOffset>mContentLength) {
		nextOffset = mContentLength;
	}
	std::ostringstream rangeStr;
	rangeStr << mBytesOffset << "-" << nextOffset;
	curl_easy_setopt(mCurl, CURLOPT_RANGE, rangeStr.str().c_str());
	this->_performGET();
}

void HttpRequestCurl::_performPOST() {
	//TODO
}

size_t HttpRequestCurl::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) {
		/* out of memory! */ 
		return 0;
	}
 
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
 
	return realsize;
}