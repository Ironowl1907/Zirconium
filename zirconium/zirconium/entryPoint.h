#pragma once

#include "Application.h"
extern zirconium::Application *zirconium::CreateApplication();

int main(int argc, char **argv) {
  auto app = zirconium::CreateApplication();
  app->Run();
  delete app;

  return 0;
}
