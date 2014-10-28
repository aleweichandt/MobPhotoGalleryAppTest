
#ifndef __PHOTO_DIRECTORY_H__
#define __PHOTO_DIRECTORY_H__

#include <vector>
#include "connection\HttpRequest.h"

#define WORKING_DIR ""

class HttpRequest;
class PhotoObject;
class PhotoDirectory : public HttpRequest::HttpRequestInterface{
	enum dir_state {
		D_STATE_ERROR = -1,
		D_STATE_INIT,
		D_STATE_DOWNLOAD,
		D_STATE_DONE
	};
public:
	PhotoDirectory(std::string name, std::string url, bool native);
	virtual ~PhotoDirectory();

	void	update(int dt);

//callbacks
	void onUpdate(HttpRequest* req);
	void onComplete(HttpRequest* req);
private:
	dir_state					mState;
	std::vector<PhotoObject*>	mPhotos;
	std::string					mName;
	std::string					mUrl;
	HttpRequest*				mRequest;

	float						mProgress;

	size_t			getDownloadedFileSize();
	std::string		getFileName();
};

#endif