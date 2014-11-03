#ifndef __UNZIPPER_H__
#define __UNZIPPER_H__

#include "unzip.h"
#include <vector>

#define WRITEBUFFERSIZE (8192)

class Unzipper {
public:
	Unzipper(std::string filename);
	virtual ~Unzipper(void);

	int getDirectorySize();
	char* getFileData(int index);
	int getFileSize(int index);

private:
	struct MemStruct {
		char *memory;
		size_t size;
	};

	int extract(unzFile uf);
	int extractCurrentFile(unzFile uf, char* &filebuffer, int &size_file);

	std::vector<MemStruct>	mFiles;
};

#endif