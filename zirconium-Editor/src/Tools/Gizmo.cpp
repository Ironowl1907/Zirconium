#include "Gizmo.h"
#include "zrpch.h"

#include "imgui.h"

namespace zirconium {

static void DrawArrow(ImDrawList* draw_list, ImVec2 start, ImVec2 end, ImU32 color, float thickness = 3.0f) {
    draw_list->AddLine(start, end, color, thickness);

    // Calculate direction
    ImVec2 dir = ImVec2(end.x - start.x, end.y - start.y);
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length == 0.0f)
        return;
    dir.x /= length;
    dir.y /= length;

    // Define arrowhead size
    float arrowSize = 10.0f;
    ImVec2 left = ImVec2(end.x - arrowSize * (dir.x - dir.y), end.y - arrowSize * (dir.y + dir.x));
    ImVec2 right = ImVec2(end.x - arrowSize * (dir.x + dir.y), end.y - arrowSize * (dir.y - dir.x));

    // Draw arrowhead
    draw_list->AddTriangleFilled(end, left, right, color);
}

EditorGizmo::EditorGizmo() {}

void EditorGizmo::RenderGizmo() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 center = ImGui::GetCursorScreenPos();

    float arrowLength = 50.0f;

    DrawArrow(draw_list, center, ImVec2(center.x + arrowLength, center.y), IM_COL32(255, 0, 0, 255));

    DrawArrow(draw_list, center, ImVec2(center.x, center.y - arrowLength), IM_COL32(0, 255, 0, 255));

    // DrawArrow(draw_list, center, ImVec2(center.x + arrowLength * 0.7f, center.y + arrowLength * 0.7f),
    //           IM_COL32(0, 0, 255, 255));
}

void EditorGizmo::ShowGizmoWindow() {
    ImGui::Begin("Gizmo Example");
    RenderGizmo();
    ImGui::End();
}

} // namespace zirconium
