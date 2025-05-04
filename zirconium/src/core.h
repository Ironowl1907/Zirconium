#include <memory>
#include <assert.h>
#include "zirconium/Core/log/log.h"
#define BIT(x) (1 << x)

#ifdef __GNUC__
#    define __FUNCSIG__ __PRETTY_FUNCTION__
#endif

// TODO: Create no argrument asserts
#ifdef ZR_ENABLE_ASSERTS
#    define ZR_ASSERT(x, ...)                               \
        if (!(x)) {                                         \
            ZR_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            assert(x);                                      \
        }

#    define ZR_CORE_ASSERT(x, ...)                               \
        if (!(x)) {                                              \
            ZR_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            assert(x);                                           \
        }
#else
#    define ZR_ASSERT(x, ...) ((void)0)
#    define ZR_CORE_ASSERT(x, ...) ((void)0)
#endif

#define ZR_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace zirconium {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

} // namespace zirconium
