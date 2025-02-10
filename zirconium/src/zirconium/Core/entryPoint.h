#pragma once

#include "Application.h"
#include "zirconium/Core/Application.h"

extern zirconium::Application* zirconium::CreateApplication(zirconium::ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
    ZR_PROFILE_BEGIN_SESSION("Startup", "Zirconium_Profile_Startup.json");
    zirconium::Log::Init();
    auto app = zirconium::CreateApplication({argc, argv});
    ZR_PROFILE_END_SESSION();

    ZR_PROFILE_BEGIN_SESSION("Runtime", "Zirconium_Profile_Runtime.json");
    app->Run();
    ZR_PROFILE_END_SESSION();

    ZR_PROFILE_BEGIN_SESSION("Shutdown", "Zirconium_Profile_Shutdown.json");
    delete app;
    ZR_PROFILE_END_SESSION();

    return 0;
}
