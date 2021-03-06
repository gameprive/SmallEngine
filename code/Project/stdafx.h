#pragma once

#include "Core/Base/CompilationOptions.h"

#pragma warning( push )
#pragma warning( disable : 4201 )

#include <cassert>
#include <cstdint>
#include <ctime>

#include <functional>
#include <memory>
#include <algorithm>

#include <string>
#include <vector>
#include <unordered_map>

#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#			include "glm/gtc/matrix_transform.hpp"
#			include "glm/gtc/type_ptr.hpp"	
#			include "glm/gtc/random.hpp"
#			include "glm/gtc/matrix_inverse.hpp"
#			include "glm/gtx/rotate_vector.hpp"

#include <glmmodel/glmmodel.h>


#pragma warning( pop )


#include "Core/Base/Base.h"
#include "Core/Debug/DebugNew.h"
#include "Core/Debug/Log.h"
#include "Core/Window/Window.h"

#include "Renderer/Resource/ShaderProgram.h"


#include "Global.h"