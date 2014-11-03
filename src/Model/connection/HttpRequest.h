#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#define RANGE_SPEED 16000;

enum HTTP_METHOD {
	GET,
	POST
};

class HttpRequest
{
public:
	enum HttpRequestState {
		HRS_ERROR = -1,
		HRS_INIT,
		HRS_RUN,
		HRS_DONE
	};

	class HttpRequestInterface {
	public:
		virtual void onUpdate(HttpRequest* req) = 0;
		virtual void onComplete(HttpRequest* req) = 0;
	};
public:
	HttpRequest(void);
	virtual ~HttpRequest(void);
//setup
	void setUrl(const std::string url);
	void setMethod(HTTP_METHOD method);
	void setHeader(const std::string headerName, const std::string headerValue);	
	void setBody(const char* body, size_t size);
	void setRangeBytesOffset(unsigned long offset);
	void send(bool async = true);
//update
	void update(int dt);

//result
	unsigned long	getContentLength();
	unsigned int	getResultCode();
	size_t			getResult(char* &result); //TODO check result data type
	
	void addUpdateListener(HttpRequestInterface* caller = 0);
	void addCompleteListener(HttpRequestInterface* caller = 0);

protected:
	HttpRequestInterface*	mOnUpdateCaller;
	HttpRequestInterface*	mOnCompleteCaller;

	HttpRequestState					mState;
	std::string							mUrl;
	HTTP_METHOD							mMethod;
	std::map<std::string,std::string>	mHeaders;
	char*								mBody;
	size_t								mBodySize;

//must fill this values
	unsigned long						mBytesOffset;
	unsigned long						mContentLength;
	
	size_t					mResultSize;
	unsigned int 			mResultCode;
	char*					mResult;

//must implement
	virtual void _performHEAD() = 0;
	virtual void _performGET() = 0;
	virtual void _performRangeGET(int range) = 0;
	virtual void _performPOST() = 0;
};


#endif
