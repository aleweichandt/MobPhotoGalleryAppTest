#include "../../StdAfx.h"
#include "HttpRequestCurl.h"

HttpRequestCurl::HttpRequestCurl(void)
{
	mCurl = curl_easy_init();
}

HttpRequestCurl::~HttpRequestCurl(void)
{
}

HttpRequest* HttpRequestCurl::setUrl(char* url)
{
	curl_easy_setopt(mCurl, CURLOPT_URL, url);
	return this;
}

HttpRequest* HttpRequestCurl::setMethod(HTTP_METHOD method)
{
	return this;
}

HttpRequest* HttpRequestCurl::setHeader(char* headerName, char* headerValue)
{
	return this;
}

HttpRequest* HttpRequestCurl::setBody(unsigned char*)
{
	return this;
}

void HttpRequestCurl::send()
{

}
