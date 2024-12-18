#pragma once

#include "Application.h"

extern zirconium::Application* zirconium::CreateApplication();

int main(int argc, char** argv) {
    zirconium::Log::Init();
    ZR_CORE_INFO("Initialized core logger!");
    ZR_INFO("Initialized client logger!");
    auto app = zirconium::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
