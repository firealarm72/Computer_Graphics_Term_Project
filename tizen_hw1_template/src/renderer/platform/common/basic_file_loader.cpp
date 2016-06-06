#include "basic/basic_file_loader.h"

FileLoader *FileLoader::mInstance = nullptr;

FileLoader::FileLoader() {

}

FileLoader::~FileLoader() {

}

FileLoader *FileLoader::GetInstance() {
    if (!mInstance) {
        mInstance = new FileLoader();
    }
    return mInstance;
}


void FileLoader::Release() {
    if (mInstance) {
        delete mInstance;
        mInstance = nullptr;

    }
}