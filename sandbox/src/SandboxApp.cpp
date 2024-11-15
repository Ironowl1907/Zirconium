#include "../../zirconium/zirconium.h"
#include <cstdio>


class SandBox : public zirconium::Application {

public:
  SandBox() {}
  ~SandBox() {}
};

zirconium::Application* zirconium::CreateApplication(){
  return new SandBox();

}
