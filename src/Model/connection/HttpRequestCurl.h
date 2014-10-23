#ifndef __HTTP_REQUEST_CURL_H__
#define __HTTP_REQUEST_CURL_H__

#include "HttpRequest.h"
#include <curl/curl.h>

class HttpRequestCurl : public HttpRequest
{
public:

	HttpRequestCurl(void);
	virtual ~HttpRequestCurl(void);
private:
	struct MemoryStruct {
		char *memory;
		size_t size;
	};

	CURL* mCurl;

	void _performGET();
	void _performRangeGET(int range);
	void _performPOST();

	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif
