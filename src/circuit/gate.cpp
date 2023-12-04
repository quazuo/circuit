#include "gate.h"

bool CircuitGate::canEval() const {
    for (const auto &input : inputs) {
        if (!input.destGate) {
            std::cout << getId() << " failed! no input\n";
            return false;
        }
    }

    return true;
}

void CircuitGate::clearCaches() {
    for (auto &output : outputs) {
        output.isEval = false;
    }

    for (const auto &input : inputs) {
        if (input.destGate) {
            input.destGate->clearCaches();
        }
    }
}

void CircuitGate::print() const {
    std::cout << getId();
    for (const auto &output : outputs) {
        std::cout << " " << std::boolalpha;
        std::visit([](auto &v) { std::cout << v; }, output.value);
    }
    std::cout << "\n";

    for (const auto &input : inputs) {
        input.destGate->print();
    }
}
