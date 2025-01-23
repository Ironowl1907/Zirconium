#pragma once

#include <string>

namespace zirconium {

  class FileDialogs{
    public:
    static bool OpenFile(std::string& path);
    static bool SaveFile(std::string& path);
  };

}
