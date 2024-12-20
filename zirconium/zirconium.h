#pragma once
#include "zirconium/Core/Application.h"

// --- This shouldn't be here ---
#include "zirconium/Platform/OpenGL/OpenGLShader.h"

// ---- Layers -----
#include "zirconium/ImGui/imguiLayer.h"
#include "zirconium/Layers/Layer.h"
#include "zirconium/Layers/StackLayer.h"

#include "zirconium/Core/input.h"

// ---- Key Codes -----
#include "zirconium/Core/keycodes.h"
#include "zirconium/Core/mouseButtonCodes.h"

// --- Time---
#include "zirconium/Core/Timestep.h"

// --- Logger ---
#include "zirconium/Core/log/log.h"

//--- Renderer ---
#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/Camera.h"
#include "zirconium/Renderer/OrthoCameraController.h"
#include "zirconium/Renderer/GraphicsContext.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer.h"
#include "zirconium/Renderer/RendererAPI.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/VertexArray.h"
#include "zirconium/Renderer/Texture.h"


#include "zirconium/Renderer/Renderer2D.h"


// --- Helpers ---
#include "zirconium/Core/Transform.h"
