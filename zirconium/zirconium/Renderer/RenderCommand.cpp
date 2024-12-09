#include "RendererAPI.h"
#include "zrpch.h"
#include "RenderCommand.h"

#include "../platform/OpenGL/OpenGLRendererAPI.h"

namespace zirconium {
  RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRenderAPI();
};
