#ifndef __BASIC_FILE_LOADER_H__
#define __BASIC_FILE_LOADER_H__

#include <string>

class FileLoader
{
private:
	static FileLoader* mInstance;

	FileLoader();

	~FileLoader();

public:
	static FileLoader* GetInstance();

	void Release();

	std::string ReadTxtFile(const std::string& filename) const;
};


#endif //__BASIC_FILE_LOADER_H__


