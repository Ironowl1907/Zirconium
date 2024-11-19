#include <cassert>
#define BIT(x) (1 << x)
#ifdef ZR_ENABLE_ASSERTS
#define ZR_ASSERT(x, ...)                                                      \
  if (!(x)) {                                                                  \
    ZR_ERROR("Assertion failed: {0}", __VA_ARGS__);                            \
    assert(x);                                                                 \
  }

#define ZR_CORE_ASSERT(x, ...)                                                 \
  if (!(x)) {                                                                  \
    ZR_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);                       \
    assert(x);                                                                 \
  }
#else
#define ZR_ASSERT(x, ...)  ((void)0)
#define ZR_CORE_ASSERT(x, ...)  ((void)0)
#endif
