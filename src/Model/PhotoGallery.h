#ifndef __PHOTO_GALLERY_H__
#define __PHOTO_GALLERY_H__

class HttpRequest;

class PhotoGallery {
public:
	enum STATE
	{
		STATE_INIT = 0,
		STATE_RUN,
		STATE_END
	};
public:
	PhotoGallery();
	virtual ~PhotoGallery();
	void update(int dt);

	STATE getState() { return mState; };

//callbacks
	void onInitDownloaded(HttpRequest request);
private:
	STATE mState;
	std::vector<PhotoDirectory> mPhotoDirectories;
};

#endif