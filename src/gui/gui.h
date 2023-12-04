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

    enum SlotType { INPUT, OUTPUT };

    struct CachedLink {
        CircuitGatePtr destGate;
        size_t destSlotIndex;
        SlotType cachedType;
    };

    /* render constants */
    constexpr static float SLOT_RADIUS = 5.0f;
    constexpr static float SLOT_GAP = 20.f;
    constexpr static ImVec2 GATE_WINDOW_PADDING = {8.0f, 8.0f};
    constexpr static float GATE_CORNER_ROUNDING = 4.0f;
    constexpr static float LINK_WIDTH = 3.0f;

public:
    GLFWwindow* init();

    void render(std::vector<CircuitGatePtr>& gates);

    void shutdown();

private:
    void renderGrid();

    void renderGates(std::vector<CircuitGatePtr>& gates);

    void renderGate(CircuitGatePtr& gate);

    void renderGatesLinks(std::vector<CircuitGatePtr>& gates);

    ImVec2 getGateInputSlotPos(const CircuitGatePtr &gate, size_t slotIndex);

    ImVec2 calcGateRectSize(const CircuitGatePtr &gate);

    ImVec2 getGateOutputSlotPos(const CircuitGatePtr &gate, size_t slotIndex);

    void renderLink(ImVec2 p1, ImVec2 p2) const;

    void handleSlotDragDrop(CircuitGatePtr &gate, size_t slotIndex, SlotType slotType);
};

#endif //CIRCUIT_GUI_H
