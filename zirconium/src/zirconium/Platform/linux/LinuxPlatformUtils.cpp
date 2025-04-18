#include "zirconium/Utils/PlatformUtils.h"
#include "zrpch.h"

#include "ImGuiFileDialog.h"

namespace zirconium {

bool FileDialogs::OpenFile(std::string& path, const char* filters) {
    std::string filePathName;
    std::string filePath;
    IGFD::FileDialogConfig config;

    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", filters, config);
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            path = ImGuiFileDialog::Instance()->GetFilePathName();
        } else
            ImGuiFileDialog::Instance()->Close();
        return true;
    }
    return false;
}
bool FileDialogs::SaveFile(std::string& path, const char* filters) {
    std::string filePathName;
    std::string filePath;
    IGFD::FileDialogConfig config;

    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", filters, config);
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            path = ImGuiFileDialog::Instance()->GetFilePathName();
        } else
            ImGuiFileDialog::Instance()->Close();
        return true;
    }
    return false;
}
} // namespace zirconium
