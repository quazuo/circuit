#include "gui.h"
#include "../circuit/visitor.h"

#include <stdexcept>
#include <GLFW/glfw3.h>
#include <cmath>
#include <sstream>
#include <iostream>

constexpr static ImU32 LINK_COLOR = IM_COL32(200, 200, 100, 255);
constexpr static ImU32 GATE_COLOR = IM_COL32(60, 60, 60, 255);
constexpr static ImU32 GATE_COLOR_HOVER = IM_COL32(70, 70, 70, 255);
constexpr static ImU32 GATE_BORDER_COLOR = IM_COL32(100, 100, 100, 255);
constexpr static ImU32 SLOT_COLOR = IM_COL32(150, 150, 150, 255);
constexpr static ImU32 SLOT_COLOR_HOVER = IM_COL32(180, 180, 180, 255);

static ImU32 getPinTypeColor(const CircuitGate::PinType &type) {
    if (type == CircuitGate::Any)
        return IM_COL32(90, 90, 90, 255);
    if (type == CircuitGate::Int)
        return IM_COL32(0, 78, 173, 255);
    if (type == CircuitGate::Bool)
        return IM_COL32(193, 73, 73, 255);

    std::stringstream ss;
    ss << "invalid output pin type!";
    throw std::runtime_error(ss.str());
}

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

static inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

struct CircuitVisitor_RenderContent : public CircuitVisitor {
    void visit(CircuitGate_ConstInt& gate) override {
        ImGui::InputInt("Value", &gate.value);
    }
};

GLFWwindow *Gui::init() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit failed");
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Circuit simulator", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("glfwCreateWindow failed");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

void Gui::render(std::vector<CircuitGatePtr> &gates) {
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();

    // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
    // Based on your use case you may want one or the other.
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                             | ImGuiWindowFlags_NoMove
                             | ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("gate editor", nullptr, flags)) {
        ImGui::Text("position: (%.2f,%.2f)", (double) state.scrolling.x, (double) state.scrolling.y);
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Checkbox("Show grid", &state.showGrid);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
        ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
        ImGui::PopStyleVar(); // WindowPadding
        ImGui::PushItemWidth(120.0f);

        if (state.showGrid)
            renderGrid();

        renderGates(gates);
        renderGatesLinks(gates);

        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive()
            && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)) {
            state.scrolling = state.scrolling + io.MouseDelta;
        }

        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }
    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Gui::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Gui::renderGrid() {
    const ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
    const float GRID_SZ = 64.0f;
    const ImVec2 win_pos = ImGui::GetCursorScreenPos();
    const ImVec2 canvas_sz = ImGui::GetWindowSize();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    for (float x = fmodf(state.scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
        draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
    for (float y = fmodf(state.scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
        draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
}

void Gui::renderGates(std::vector<CircuitGatePtr> &gates) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(2);

    for (auto &gate: gates) {
        renderGate(gate);
    }

    drawList->ChannelsMerge();
}

ImVec2 Gui::getGateInputSlotPos(const CircuitGatePtr &gate, size_t slotIndex) {
    const ImVec2 rectMin = state.scrolling + gate->pos;
    const ImVec2 rectSize = gatesRenderInfo.at(gate->getId()).rectSize;

    const size_t n = gate->getInputsCount();
    const float yOffsetBase = rectSize.y / 2 - (n - 1) * SLOT_GAP / 2 - n * SLOT_RADIUS;
    const float yOffset = yOffsetBase + 2 * slotIndex * SLOT_RADIUS + slotIndex * SLOT_GAP;
    return rectMin + ImVec2(0, yOffset);
}

ImVec2 Gui::getGateOutputSlotPos(const CircuitGatePtr &gate, size_t slotIndex) {
    const ImVec2 rectMin = state.scrolling + gate->pos;
    const ImVec2 rectSize = gatesRenderInfo.at(gate->getId()).rectSize;

    const size_t n = gate->getOutputsCount();
    const float yOffsetBase = rectSize.y / 2 - (n - 1) * SLOT_GAP / 2 - n * SLOT_RADIUS;
    const float yOffset = yOffsetBase + 2 * slotIndex * SLOT_RADIUS + slotIndex * SLOT_GAP;
    return rectMin + ImVec2(rectSize.x, yOffset);
}

ImVec2 Gui::calcGateRectSize(const CircuitGatePtr &gate) {
    const bool isLeftGap = gate->getInputsCount() != 0;
    const bool isRightGap = gate->getOutputsCount() != 0;

    ImVec2 rectSize = ImGui::GetItemRectSize();
    if (isLeftGap)
        rectSize.x += SLOT_GAP;
    if (isRightGap)
        rectSize.x += SLOT_GAP;
    const size_t biggerSlotCount = std::max(gate->getInputsCount(), gate->getOutputsCount());
    rectSize.y = std::max(
            rectSize.y,
            biggerSlotCount * SLOT_RADIUS * 2 + (biggerSlotCount + 1) * SLOT_GAP
    );
    rectSize = rectSize + GATE_WINDOW_PADDING + GATE_WINDOW_PADDING;

    gatesRenderInfo.emplace(gate->getId(), rectSize);
    return rectSize;
}

void Gui::handleSlotDragDrop(CircuitGatePtr &gate, size_t slotIndex, SlotType slotType) {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip)) {
        CachedLink cachedLink = {gate, slotIndex, slotType};
        ImGui::SetDragDropPayload("CIRCUIT_LINK", &cachedLink, sizeof(cachedLink), ImGuiCond_Once);
        ImGui::EndDragDropSource();

        if (slotType == INPUT) {
            renderLink(getGateInputSlotPos(gate, slotIndex), ImGui::GetMousePos());
        } else {
            renderLink(ImGui::GetMousePos(), getGateOutputSlotPos(gate, slotIndex));
        }

    } else if (ImGui::BeginDragDropTarget()) {
        const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CIRCUIT_LINK");
        if (!payload || !payload->IsDelivery()) {
            ImGui::EndDragDropTarget();
            return;
        }

        const CachedLink *cachedLink = (CachedLink *) payload->Data;

        if (cachedLink->cachedType == slotType) {
            ImGui::EndDragDropTarget();
            return;
        }

        const CircuitGate::PinType type1 = slotType == INPUT
                                           ? gate->getInput(slotIndex).type
                                           : gate->getOutput(slotIndex).type;

        const CircuitGate::PinType type2 = cachedLink->cachedType == INPUT
                                           ? cachedLink->destGate->getInput(cachedLink->destSlotIndex).type
                                           : cachedLink->destGate->getOutput(cachedLink->destSlotIndex).type;

        if (type1 != type2) {
            std::cout << "TypeError\n"; // todo nicer error
            ImGui::EndDragDropTarget();
            return;
        }

        if (slotType == INPUT) {
            gate->updateInput(cachedLink->destGate, cachedLink->destSlotIndex, slotIndex);
        } else {
            cachedLink->destGate->updateInput(gate, slotIndex, cachedLink->destSlotIndex);
        }

        ImGui::EndDragDropTarget();
    }
}

static void onClickEvalButton(CircuitGatePtr &gate) {
    CircuitVisitor_Eval visitor;
    gate->acceptVisitor(visitor);
    if (visitor.didEvalCorrectly()) {
        std::cout << "\n";
        gate->print();
    }
    gate->clearCaches();
}

void Gui::renderGate(CircuitGatePtr &gate) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::PushID(gate->getId());

    const ImVec2 rectMin = state.scrolling + gate->pos;
    const bool isLeftGap = gate->getInputsCount() != 0;

    // display gate contents first
    drawList->ChannelsSetCurrent(1); // foreground
    ImGui::SetCursorScreenPos(rectMin + GATE_WINDOW_PADDING + (isLeftGap ? ImVec2(SLOT_GAP, 0) : ImVec2()));
    ImGui::BeginGroup();
    ImGui::Text("%s [ID %d]", gate->getName().c_str(), gate->getId());

    CircuitVisitor_RenderContent renderVisitor;
    gate->acceptVisitor(renderVisitor);

    ImGui::Button("eval", {50, 20});
    if (ImGui::IsItemActive() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        onClickEvalButton(gate);
    }
    ImGui::EndGroup();

    const ImVec2 rectSize = calcGateRectSize(gate);
    const ImVec2 rectMax = rectMin + rectSize;

    // Display link slots
    for (size_t slotIndex = 0; slotIndex < gate->getInputsCount(); slotIndex++) {
        const ImVec2 circleCenter = getGateInputSlotPos(gate, slotIndex);

        ImGui::SetCursorScreenPos(circleCenter - ImVec2(SLOT_RADIUS, SLOT_RADIUS));
        const std::string buttonId = std::to_string(gate->getId()) + std::string("i") + std::to_string(slotIndex);
        ImGui::InvisibleButton(buttonId.c_str(), ImVec2(2 * SLOT_RADIUS, 2 * SLOT_RADIUS));
        handleSlotDragDrop(gate, slotIndex, INPUT);

        ImU32 slotColor = getPinTypeColor(gate->getInput(slotIndex).type);
        drawList->AddCircleFilled(circleCenter, SLOT_RADIUS, slotColor);
    }

    for (size_t slotIndex = 0; slotIndex < gate->getOutputsCount(); slotIndex++) {
        const ImVec2 circleCenter = getGateOutputSlotPos(gate, slotIndex);

        ImGui::SetCursorScreenPos(circleCenter - ImVec2(SLOT_RADIUS, SLOT_RADIUS));
        const std::string buttonId = std::to_string(gate->getId()) + std::string("o") + std::to_string(slotIndex);
        ImGui::InvisibleButton(buttonId.c_str(), ImVec2(2 * SLOT_RADIUS, 2 * SLOT_RADIUS));
        handleSlotDragDrop(gate, slotIndex, OUTPUT);

        ImU32 slotColor = getPinTypeColor(gate->getOutput(slotIndex).type);
        drawList->AddCircleFilled(circleCenter, SLOT_RADIUS, slotColor);
    }

    // display box
    drawList->ChannelsSetCurrent(0); // background
    ImGui::SetCursorScreenPos(rectMin);
    ImGui::InvisibleButton("gate", rectSize);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        gate->pos = gate->pos + io.MouseDelta;
    }

    ImU32 bgColor = (ImGui::IsItemHovered() || ImGui::IsItemActive()) ? GATE_COLOR_HOVER : GATE_COLOR;
    drawList->AddRectFilled(rectMin, rectMax, bgColor, GATE_CORNER_ROUNDING);
    drawList->AddRect(rectMin, rectMax, GATE_BORDER_COLOR, GATE_CORNER_ROUNDING);

    ImGui::PopID();
}

void Gui::renderGatesLinks(std::vector<CircuitGatePtr> &gates) {
    for (auto &gate: gates) {
        const std::vector<CircuitGate::InputPin> &inputs = gate->getInputs();

        for (size_t i = 0; i < inputs.size(); i++) {
            if (!inputs[i].destGate)
                continue;

            const ImVec2 mySlot = getGateInputSlotPos(gate, i);
            const ImVec2 otherSlot = getGateOutputSlotPos(inputs[i].destGate, inputs[i].destSlotIndex);
            renderLink(mySlot, otherSlot);
        }
    }
}

void Gui::renderLink(const ImVec2 p1, const ImVec2 p2) const {
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const float xDist = std::abs(p1.x - p2.x);
    const float bezierOffset = std::min(xDist / 2, 50.0f);
    drawList->AddBezierCubic(p1, p1 - ImVec2(bezierOffset, 0), p2 + ImVec2(bezierOffset, 0), p2,
                             LINK_COLOR, LINK_WIDTH);
}
