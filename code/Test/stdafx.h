#pragma once

#include "Core/Base/CompileConfig.h"
#include "Core/Base/MinWindows.h"

#pragma warning( push )
#pragma warning( disable : 4201 )

#include <ctime>

#include <functional>
#include <memory>
#include <algorithm>

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#pragma warning( pop )


#include "Core/Base/Base.h"
#include "Core/Debug/DebugNew.h"
#include "Core/Debug/Log.h"
#include "Core/Window/Window.h"
#include "Renderer/System/RenderDevice.h"
#include "Renderer/System/RenderSystem.h"