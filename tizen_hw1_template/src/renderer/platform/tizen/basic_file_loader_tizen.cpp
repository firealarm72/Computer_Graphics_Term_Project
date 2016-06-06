#ifdef BASIC_TIZEN

#include <app.h>
#include <Elementary.h>
#include <fstream>

#include "basic/basic_file_loader.h"
#include "basic/basic_utils.h"
#include "sample/sample_launcher.h"

std::string FileLoader::ReadTxtFile(const std::string &filename) const
{

    std::string path = app_get_resource_path() + filename;

    std::ifstream in(path, std::ios::in | std::ios::binary);

    std::string msg = "Failed to open file " + path;
    ASSERT(in.is_open() && in.good(), msg.c_str());

    std::string ret;
    in.seekg(0, std::ios::end);         // Set get position to end
    ret.resize(in.tellg());            // Resize string to support enough bytes
    in.seekg(0, std::ios::beg);         // Set get position to beginning
    in.read(&ret.at(0), ret.size());     // Read file to string
    in.close();

    return ret;

}

#endif
