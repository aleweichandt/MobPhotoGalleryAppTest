#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

class HttpRequest
{
	enum METHOD {
		GET,
		POST
	};
	typedef void (*onCompletionCallback)(HttpRequest);
	typedef void (*onUpdateCallback)(HttpRequest);
public:
	HttpRequest(void);
	virtual ~HttpRequest(void);
//setup
	virtual HttpRequest setUrl(char* url);
	virtual HttpRequest setMethod(METHOD method);
	virtual void		send();
	
	HttpRequest			setCallbacks(onUpdateCallback onUpdate = 0, onCompletionCallback onCompete = 0);
//result
	virtual short		getResultCode();
	virtual char*		getResult(); //TODO change result data type

private:
	onUpdateCallback* mOnUpdateCb;
	onCompletionCallback* mOnCompleteCb;
};

#endif
