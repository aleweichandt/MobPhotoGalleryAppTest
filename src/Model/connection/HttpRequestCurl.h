#ifndef __HTTP_REQUEST_CURL_H__
#define __HTTP_REQUEST_CURL_H__

#include "HttpRequest.h"
#include <curl/curl.h>

class HttpRequestCurl : public HttpRequest
{
public:
	HttpRequestCurl(void);
	virtual ~HttpRequestCurl(void);
//setup
	HttpRequest* setUrl(char* url);
	HttpRequest* setMethod(HTTP_METHOD method);
	HttpRequest* setHeader(char* headerName, char* headerValue);	
	HttpRequest* setBody(unsigned char*) = 0; //TODO check data type
	void		 send();

private:
	CURL* mCurl;
	
	
};

#endif
