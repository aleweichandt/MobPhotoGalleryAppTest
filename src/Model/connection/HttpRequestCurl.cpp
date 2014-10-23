#include "../../StdAfx.h"
#include "HttpRequestCurl.h"

HttpRequestCurl::HttpRequestCurl(void)
:HttpRequest()
{
	curl_global_init(CURL_GLOBAL_ALL);
	mCurl = curl_easy_init();
}

HttpRequestCurl::~HttpRequestCurl(void)
{
	curl_easy_cleanup(mCurl);
	curl_global_cleanup();
}

void HttpRequestCurl::_performGET() {
	struct MemoryStruct chunk;
 
	chunk.memory = mResult;
	chunk.size = mContentCurrent; 

	curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str());
	//TODO add headers
	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(mCurl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	CURLcode resp = curl_easy_perform(mCurl);
	if(resp == CURLE_OK) {
		long respcode;
		curl_easy_getinfo(mCurl,CURLINFO_RESPONSE_CODE, &respcode);
		mResultCode = (unsigned int) respcode;
	}
	if(chunk.memory) {
		mContentCurrent = chunk.size;
		free(chunk.memory);
	}
}

void HttpRequestCurl::_performRangeGET(int range) {
	std::ostringstream rangeStr;
	rangeStr << (mContentCurrent+1) << "-" << range;
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