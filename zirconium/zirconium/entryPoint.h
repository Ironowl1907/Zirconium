#pragma once

#include "Application.h"

extern zirconium::Application *zirconium::CreateApplication();

int main(int argc, char **argv) {
  zirconium::Log::Init();
  ZIR_CORE_INFO("Initialized core logger!");
  ZIR_INFO("Initialized client logger! Var0 = {0}");
  auto app = zirconium::CreateApplication();
  app->Run();
  delete app;

  return 0;
}
