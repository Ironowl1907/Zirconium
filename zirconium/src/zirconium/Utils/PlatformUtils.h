#pragma once

#include <string>
#include <filesystem>

namespace zirconium {

class FileDialogs {
public:
    static bool OpenFile(std::string& path, const char* flags = ".zr");
    static bool SaveFile(std::string& path, const char* flags = ".zr");
};

inline bool CopyFiles(const std::string& templatePath, const std::string& newScriptPath) {
    namespace fs = std::filesystem;
    try {
        if (!fs::exists(templatePath)) {
            ZR_CORE_ERROR("Source file does not exist {}", templatePath);
            return false;
        }

        fs::copy_file(templatePath, newScriptPath, fs::copy_options::overwrite_existing);
        std::cout << "Created script: " << newScriptPath << std::endl;
        ZR_CORE_TRACE("Created file {}", newScriptPath);
        return true;
    } catch (const fs::filesystem_error& e) {
        ZR_CORE_ERROR("Filesystem error {}", e.what());
        return false;
    }
}

} // namespace zirconium
