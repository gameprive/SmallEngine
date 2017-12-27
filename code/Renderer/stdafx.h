#pragma once

#include "Core/Base/CompileConfig.h"

#include "Core/Base/MinWindows.h"

#pragma warning( push )
#pragma warning( disable : 4201 )

#include <cassert>
#include <cstdint>

#include <chrono>
#include <functional>
#include <memory>
#include <algorithm>

#include <string>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#pragma warning( pop )

#include "Core/Base/Base.h"
#include "Core/Debug/DebugNew.h"
#include "Core/Debug/Log.h"
#include "Core/Template/Singleton.h"
#include "Core/IO/FileSystem.h"