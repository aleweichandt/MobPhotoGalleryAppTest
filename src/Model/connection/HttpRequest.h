#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

enum HTTP_METHOD {
	GET,
	POST
};

class HttpRequest
{	
	typedef void (*onCompleteCallback)(HttpRequest*);
	typedef void (*onUpdateCallback)(HttpRequest*);
public:
	class HttpRequestInterface {
	public:
		virtual void onUpdate(HttpRequest* req) = 0;
		virtual void onComplete(HttpRequest* req) = 0;
	};
public:
	HttpRequest(void);
	virtual ~HttpRequest(void);
//setup
	virtual HttpRequest* setUrl(const char* url) = 0;
	virtual HttpRequest* setMethod(HTTP_METHOD method) = 0;
	virtual HttpRequest* setHeader(const char* headerName, const char* headerValue) = 0;	
	virtual HttpRequest* setBody(unsigned char*) = 0; //TODO check data type
	virtual void		 send() = 0;
//result
	short			getResultCode();
	unsigned char*	getResult(); //TODO check result data type
	
	HttpRequest*	setUpdateCallback(onUpdateCallback* callback = 0, HttpRequestInterface* caller = 0);
	HttpRequest*	setCompleteCallback(onCompleteCallback* callback = 0, HttpRequestInterface* caller = 0);

private:
	onUpdateCallback* 		mOnUpdateCb;
	onCompleteCallback* 	mOnCompleteCb;
	HttpRequestInterface*	mOnUpdateCaller;
	HttpRequestInterface*	mOnCompleteCaller;
	
	unsigned int 			mResultCode;
	unsigned char*			mResult;
};


#endif
