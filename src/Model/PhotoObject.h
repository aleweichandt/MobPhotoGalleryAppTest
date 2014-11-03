
#ifndef __PHOTO_OBJECT_H__
#define __PHOTO_OBJECT_H__

class PhotoObject {
public:
	PhotoObject();
	virtual ~PhotoObject();

	void setInfo(char* info, int size);
private:
	char* mInfo;
};

#endif