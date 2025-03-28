#pragma once

#include <string>

namespace zirconium {

  class FileDialogs{
    public:
    static bool OpenFile(std::string& path, const char* flags = ".zr");
    static bool SaveFile(std::string& path, const char* flags = ".zr");
  };

}
