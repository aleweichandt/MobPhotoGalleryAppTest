
#ifndef __PHOTO_GALLERY_H__
#define __PHOTO_GALLERY_H__

#include "connection/HttpRequest.h"
#include <vector>

#define SERVER_BASE_URL "localhost/server";

class PhotoDirectory;

class PhotoGallery : public HttpRequest::HttpRequestInterface {
public:
	enum gal_state
	{
		G_STATE_INIT = 0,
		G_STATE_RUN,
		G_STATE_END
	};
public:
	PhotoGallery();
	virtual ~PhotoGallery();
	void update(int dt);

	gal_state getState() { return mState; };

//callbacks
	void onUpdate(HttpRequest* req);
	void onComplete(HttpRequest* req);
private:
	gal_state mState;
	std::vector<PhotoDirectory*> mPhotoDirectories;
	HttpRequest* mInitRequest;
};

#endif