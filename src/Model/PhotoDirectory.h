
#ifndef __PHOTO_DIRECTORY_H__
#define __PHOTO_DIRECTORY_H__

#include <vector>

class PhotoObject;
class PhotoDirectory {
	enum dir_state {
		D_STATE_ERROR = -1,
		D_STATE_INIT,
		D_STATE_DOWNLOAD,
		D_STATE_DONE
	};
public:
	PhotoDirectory();
	virtual ~PhotoDirectory();
	void update(int dt);
private:
	dir_state mState;
	std::vector<PhotoObject*> mPhotoDirectories;
};

#endif