#define _HAS_STD_BYTE 0

// °¢Á¾ include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <functional>
#include <thread>
#include <cmath>
#include <fstream>
#include <mutex>
#include <thread>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#include "Macro.h"
#include "Enum.h"

template<typename T>
using Ref = shared_ptr<T>;

template<typename T>
using WRef = weak_ptr<T>;