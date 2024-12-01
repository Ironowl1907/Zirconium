#include "zirconium/input.h"

namespace zirconium {

class LinuxInput : public Input {
protected:
  virtual bool IsKeyPressedImpl(int keycode) override;
  virtual bool IsMouseButtonPressedImpl(int keycode) override;
  virtual float GetMouseXImpl() override;
  virtual float GetMouseYImpl() override;
  virtual std::pair<float, float> GetMousePosImpl() override;
};

} // namespace zirconium
