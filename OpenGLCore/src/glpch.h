#pragma once

#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef OPENGLCORE_PLATFORM_WINDOWS
    #include <Windows.h>
    #define MSAA_SAMPLES 4
#endif