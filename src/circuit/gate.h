#ifndef CIRCUIT_GATE_H
#define CIRCUIT_GATE_H

#include "../../deps/imgui/imgui.h"
#include <memory>
#include <utility>
#include <vector>

struct CircuitGate {
    using GateID = int;
    using GatePtr = std::shared_ptr<CircuitGate>;

    struct GateLink {
        GatePtr destGate;
        size_t destSlotIndex;
    };

private:
    static GateID nextId;
    GateID id;
    std::vector<GateLink> inputs;
    size_t inputsCount, outputsCount;

public:
    ImVec2 pos;

    explicit CircuitGate(ImVec2 _pos = {}, size_t ic = 0, size_t oc = 0)
        : id(nextId++), inputsCount(ic), outputsCount(oc), pos(_pos) {
        inputs.resize(inputsCount);
    }

    [[nodiscard]]
    GateID getId() const { return id; }

    [[nodiscard]]
    size_t getInputsCount() const { return inputsCount; }

    [[nodiscard]]
    size_t getOutputsCount() const { return outputsCount; }

    [[nodiscard]]
    const std::vector<GateLink>& getInputs() const { return inputs; }

    void updateInput(GateLink link, size_t index) {
        if (index >= inputsCount) {
            throw std::runtime_error("index too large in updateInput");
        }

        inputs[index] = std::move(link);
    }
};

#endif //CIRCUIT_GATE_H
