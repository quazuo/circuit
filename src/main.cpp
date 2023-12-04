#include "gui/gui.h"
#include "circuit/boolean/boolean-gate.h"
#include "circuit/num/num-gate.h"
#include "circuit/visitor.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

CircuitGate::GateID CircuitGate::nextId;

int main() {
    Gui gui;
    GLFWwindow* window = gui.init();

    CircuitGatePtr g1 = std::make_shared<CircuitGate_Not>(CircuitGate_Not({40, 50}));
    CircuitGatePtr g2 = std::make_shared<CircuitGate_Not>(CircuitGate_Not({40, 150}));
    CircuitGatePtr g3 = std::make_shared<CircuitGate_And>(CircuitGate_And({270, 80}));
    CircuitGatePtr g4 = std::make_shared<CircuitGate_ConstTrue>(CircuitGate_ConstTrue({160, 200}));
    CircuitGatePtr g5 = std::make_shared<CircuitGate_ConstInt>(CircuitGate_ConstInt({160, 200}));
    CircuitGatePtr g6 = std::make_shared<CircuitGate_ConstInt>(CircuitGate_ConstInt({160, 200}));
    CircuitGatePtr g7 = std::make_shared<CircuitGate_ConstInt>(CircuitGate_ConstInt({160, 200}));
    CircuitGatePtr g11 = std::make_shared<CircuitGate_ConstInt>(CircuitGate_ConstInt({160, 200}));
    CircuitGatePtr g8 = std::make_shared<CircuitGate_Add>(CircuitGate_Add({160, 200}));
    CircuitGatePtr g9 = std::make_shared<CircuitGate_Mul>(CircuitGate_Mul({160, 200}));
    CircuitGatePtr g10 = std::make_shared<CircuitGate_CmpLe>(CircuitGate_CmpLe({160, 200}));

    std::vector<CircuitGatePtr> gates = { g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11 };

    while (!glfwWindowShouldClose(window)) {
        gui.render(gates);
    }

    gui.shutdown();
    return 0;
}
