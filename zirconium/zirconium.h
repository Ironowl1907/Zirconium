#pragma once
#include "zirconium/Application.h"

// --- This shouldn't de here ---
#include "platform/OpenGL/OpenGLShader.h"

// ---- Layers -----
#include "zirconium/imgui/imguiLayer.h"
#include "zirconium/layers/Layer.h"
#include "zirconium/layers/StackLayer.h"

#include "zirconium/input.h"

// ---- Key Codes -----
#include "zirconium/keycodes.h"
#include "zirconium/mouseButtonCodes.h"

// --- Time---
#include "zirconium/Timestep.h"

// --- Logger ---
#include "zirconium/log.h"

//--- Renderer ---
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

// --- Entry Point ---
#include "zirconium/entryPoint.h"
