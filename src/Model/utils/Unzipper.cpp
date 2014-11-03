#include "../../StdAfx.h"
#include "Unzipper.h"

/*#ifdef _WIN32
#define USEWIN32IOAPI
#include "contrib/minizip/iowin32.h"
#endif*/

Unzipper::Unzipper(std::string filename) {
	unzFile uf;
	uf = unzOpen(filename.c_str());
	if(uf != NULL) {
		extract(uf);
	}
	unzClose(uf);
}

Unzipper::~Unzipper(void) {
	if(!mFiles.empty()) {
		char* m = mFiles.back().memory;
		if(m) {
			free(m);
		}
		mFiles.pop_back();
	}
	mFiles.clear();
}

int Unzipper::getDirectorySize() {
	return mFiles.size();
}

char* Unzipper::getFileData(int index) {
	return mFiles[index].memory;
}

int Unzipper::getFileSize(int index) {
	return mFiles[index].size;
}

int Unzipper::extract(unzFile uf) {
	uLong i;
	unz_global_info64 gi;
	int err;
	FILE* fout=NULL;

	err = unzGetGlobalInfo64(uf,&gi);
	if (err!=UNZ_OK) {
		//TODO print error
	}
	for (i=0;i<gi.number_entry;i++)
	{
		int size = 0;
		char* filebuffer = (char*)malloc(size);
		if (extractCurrentFile(uf, filebuffer, size) != UNZ_OK)
			break;
		MemStruct ms;
		ms.memory = filebuffer;
		ms.size = size;
		mFiles.push_back(ms);

		if ((i+1)<gi.number_entry)
		{
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK)
			{
				//TODO print error
				break;
			}
		}
	}

	return 0;
}


int Unzipper::extractCurrentFile(unzFile uf, char* &filebuffer, int &size_file) {
	int err=UNZ_OK;
	void* buf;
	uInt size_buf;

	size_buf = WRITEBUFFERSIZE;
	buf = (void*)malloc(size_buf);
	if (buf==NULL) {
		return UNZ_INTERNALERROR;
	}

	err = unzOpenCurrentFilePassword(uf, 0);
	if (err!=UNZ_OK) {
		return err;
	}

	do {
		err = unzReadCurrentFile(uf,buf,size_buf);
		if (err<0) {
			break; //REAL ERROR
		}
		if (err>0) {
			filebuffer = (char*)realloc(filebuffer, size_file + err + 1);
			memcpy(&(filebuffer[size_file]), buf, err);	
			size_file += err;
			filebuffer[size_file] = 0;
		}
	} while(err > 0);

	if (err==UNZ_OK) {
		err = unzCloseCurrentFile(uf);
		if (err!=UNZ_OK) {
			return err;
		}
	} else {
		unzCloseCurrentFile(uf); /* don't lose the error */
	}

	free(buf);
	return err;
}