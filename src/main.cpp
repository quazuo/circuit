#include "gui/gui.h"

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

    CircuitGatePtr g1 = std::make_shared<CircuitGate>(CircuitGate({40, 50}, 1, 1));
    CircuitGatePtr g2 = std::make_shared<CircuitGate>(CircuitGate({40, 150}, 2, 2));
    CircuitGatePtr g3 = std::make_shared<CircuitGate>(CircuitGate({270, 80}, 3, 3));

    std::vector<CircuitGatePtr> gates = { g1, g2, g3 };

    while (!glfwWindowShouldClose(window)) {
        gui.render(gates);
    }

    gui.shutdown();
    return 0;
}
