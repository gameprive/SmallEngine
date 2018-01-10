#pragma once

#include "Core/Base/CompilationOptions.h"
#include "Core/Base/MinWindows.h"

#pragma warning( push )
#pragma warning( disable : 4201 )

#include <ctime>

#include <functional>
#include <memory>
#include <algorithm>
#include <sstream>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#pragma warning( pop )


#include "Core/Base/Base.h"
#include "Core/Debug/DebugNew.h"
#include "Core/Debug/Log.h"
#include "Core/Window/Window.h"
#include "Core/IO/FileSystem.h"
#include "Renderer/System/RenderDevice.h"
#include "Renderer/System/RenderSystem.h"

#include "Application/Engine.h"

#include "Math/Vector3.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Frustum.h"