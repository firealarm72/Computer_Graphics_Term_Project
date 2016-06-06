#ifdef IML_WIN

#include <iml_loader.h>
#include <FreeImage.h>
#include <iml_log.h>
#include <fstream>



bool FileLoader::ReadTxtFile(const std::string &path, std::string &dest){
    std::ifstream in(path, std::ios::in | std::ios::binary);

    std::string msg = "Failed to open file " + path;
    ASSERT(in.is_open() && in.good(), msg.c_str());

    in.seekg(0, std::ios::end);         // Set get position to end
    dest.resize(in.tellg());            // Resize string to support enough bytes
    in.seekg(0, std::ios::beg);         // Set get position to beginning
    in.read(&dest[0], dest.size());     // Read file to string
    in.close();

    return true;
}

#endif
