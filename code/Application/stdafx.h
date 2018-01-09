#pragma once

#include "Core/Base/CompilationOptions.h"

#pragma warning( push )

#include <ctime>

#include <functional>
#include <memory>
#include <algorithm>

#include <string>
#include <vector>

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
#include "Core/Template/Singleton.h"

#include "Core/Base/Base.h"
#include "Core/Debug/DebugNew.h"
#include "Core/Debug/Log.h"
#include "Core/Window/Window.h"
#include "Renderer/System/RenderDevice.h"
#include "Renderer/System/RenderSystem.h"