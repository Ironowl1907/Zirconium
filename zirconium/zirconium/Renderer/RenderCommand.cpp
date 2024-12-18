#include "zirconium/Renderer/RendererAPI.h"
#include "zrpch.h"
#include "zirconium/Renderer/RenderCommand.h"

#include "zirconium/Platform/OpenGL/OpenGLRendererAPI.h"

namespace zirconium {
  RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRenderAPI();
};
