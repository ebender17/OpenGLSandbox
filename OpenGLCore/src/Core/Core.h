#pragma once

#ifdef OPENGLCORE_DEBUG
#define OPENGLCORE_ENABLE_ASSERTS
#endif

#ifdef OPENGLCORE_ENABLE_ASSERTS
#define OPENGLCORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define OPENGLCORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)