#ifndef CIRCUIT_GUI_H
#define CIRCUIT_GUI_H

#include "../../deps/imgui/imgui.h"
#include "../../deps/imgui/backends/imgui_impl_glfw.h"
#include "../../deps/imgui/backends/imgui_impl_opengl3.h"
#include "../circuit/gate.h"
#include <vector>
#include <optional>
#include <map>

class Gui {
    struct State {
        bool showGrid = true;
        ImVec2 scrolling;
    };

    State state;

    GLFWwindow* window;

    struct GateRenderInfo {
        ImVec2 rectSize;
    };

    std::map<CircuitGate::GateID, GateRenderInfo> gatesRenderInfo;

    std::optional<CircuitGate::GateLink> cachedLink;
    enum { INPUT, OUTPUT } cachedType;

    /* render constants */
    constexpr static float SLOT_RADIUS = 5.0f;
    constexpr static float SLOT_GAP = 20.f;
    constexpr static ImVec2 GATE_WINDOW_PADDING = {8.0f, 8.0f};
    constexpr static float GATE_CORNER_ROUNDING = 4.0f;
    constexpr static float LINK_WIDTH = 3.0f;

    // colors
    constexpr static ImU32 LINK_COLOR = IM_COL32(200, 200, 100, 255);
    constexpr static ImU32 GATE_COLOR = IM_COL32(60, 60, 60, 255);
    constexpr static ImU32 GATE_COLOR_HOVER = IM_COL32(70, 70, 70, 255);
    constexpr static ImU32 GATE_BORDER_COLOR = IM_COL32(100, 100, 100, 255);
    constexpr static ImU32 SLOT_COLOR = IM_COL32(150, 150, 150, 255);
    constexpr static ImU32 SLOT_COLOR_HOVER = IM_COL32(180, 180, 180, 255);

public:
    GLFWwindow* init();

    void render(std::vector<CircuitGate::GatePtr>& gates);

    void shutdown();

private:
    void renderGrid();

    void renderGates(std::vector<CircuitGate::GatePtr>& gates);

    void renderGate(CircuitGate::GatePtr& gate);

    void renderGatesLinks(std::vector<CircuitGate::GatePtr>& gates);

    ImVec2 getGateInputSlotPos(const CircuitGate::GatePtr &gate, size_t slotIndex);

    ImVec2 calcGateRectSize(const CircuitGate::GatePtr &gate);

    ImVec2 getGateOutputSlotPos(const CircuitGate::GatePtr &gate, size_t slotIndex);

    void renderLink(ImVec2 p1, ImVec2 p2) const;

    void handleInputSlot(CircuitGate::GatePtr &gate, size_t slotIndex);

    void handleOutputSlot(CircuitGate::GatePtr &gate, size_t slotIndex);
};

#endif //CIRCUIT_GUI_H
