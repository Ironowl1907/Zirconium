-- All zirconium dependencies

-- Get the directory of this script (Dependencies.lua)
local dependenciesBaseDir = path.getdirectory(_SCRIPT)

IncludeDir = {}
IncludeDir["stb_image"] = path.join(dependenciesBaseDir, "zirconium/vendor/stb_image")
IncludeDir["yaml-cpp"] = path.join(dependenciesBaseDir, "zirconium/vendor/yaml-cpp/include")
IncludeDir["GLFW"] = path.join(dependenciesBaseDir, "zirconium/vendor/glfw/include")
IncludeDir["Glad"] = path.join(dependenciesBaseDir, "zirconium/vendor/glad/include")
IncludeDir["ImGui"] = path.join(dependenciesBaseDir, "zirconium/vendor/imgui")
IncludeDir["glm"] = path.join(dependenciesBaseDir, "zirconium/vendor/glm")
IncludeDir["entt"] = path.join(dependenciesBaseDir, "zirconium/vendor/entt/src/entt")
IncludeDir["ImGuiFD"] = path.join(dependenciesBaseDir, "zirconium/vendor/ImGuiFileDialog/")
IncludeDir["shaderc"] = path.join(dependenciesBaseDir, "zirconium/vendor/shaderc/libshaderc/include")
IncludeDir["spirv-cross"] = path.join(dependenciesBaseDir, "zirconium/vendor/spirv-cross/")
IncludeDir["Box2D"] = path.join(dependenciesBaseDir, "zirconium/vendor/Box2D/include")
IncludeDir["Sol2"] = path.join(dependenciesBaseDir, "zirconium/vendor/sol2/include")
IncludeDir["ImGuizmo"] = path.join(dependenciesBaseDir, "zirconium/vendor/imguizmo/")
