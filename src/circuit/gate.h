#ifndef CIRCUIT_GATE_H
#define CIRCUIT_GATE_H

#include "../../deps/imgui/imgui.h"
#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <variant>

struct CircuitVisitor;

struct CircuitGate {
    using GateID = int;
    using GatePtr = std::shared_ptr<CircuitGate>;
    enum PinType {UNSET, Any, Int, Bool};

    struct InputPin {
        PinType type = UNSET;
        GatePtr destGate;
        size_t destSlotIndex = 0;
    };

    struct OutputPin {
        PinType type = UNSET;
        std::variant<int, bool> value;
        bool isEval = false;
    };

    friend struct CircuitVisitor_Eval;

private:
    static GateID nextId;
    GateID id;
    std::vector<InputPin> inputs;
    std::vector<OutputPin> outputs;
    size_t inputsCount, outputsCount;

public:
    ImVec2 pos;

    explicit CircuitGate(std::initializer_list<PinType> inTypes, std::initializer_list<PinType> outTypes, ImVec2 _pos)
        : id(nextId++), inputsCount(inTypes.size()), outputsCount(outTypes.size()), pos(_pos) {
        inputs.resize(inputsCount);
        outputs.resize(outputsCount);

        auto it = inTypes.begin();
        for (size_t i = 0; i < inputsCount; i++, it++) {
            inputs[i].type = *it;
        }

        it = outTypes.begin();
        for (size_t i = 0; i < outputsCount; i++, it++) {
            outputs[i].type = *it;
        }
    }

    virtual ~CircuitGate() = default;

    [[nodiscard]]
    GateID getId() const { return id; }

    [[nodiscard]]
    size_t getInputsCount() const { return inputsCount; }

    [[nodiscard]]
    size_t getOutputsCount() const { return outputsCount; }

    [[nodiscard]]
    const std::vector<InputPin>& getInputs() const { return inputs; }

    [[nodiscard]]
    const InputPin& getInput(size_t index) const { return inputs[index]; }

    [[nodiscard]]
    const OutputPin& getOutput(size_t index) const { return outputs[index]; }

    [[nodiscard]]
    const OutputPin& getOutputForInput(size_t index) {
        return inputs[index].destGate->getOutput(inputs[index].destSlotIndex);
    }

    [[nodiscard]]
    virtual std::string getName() const = 0;

    void updateInput(GatePtr destGate, size_t destSlotIndex, size_t index) {
        if (index >= inputsCount) {
            throw std::runtime_error("index too large in updateInput");
        }

        inputs[index].destGate = destGate;
        inputs[index].destSlotIndex = destSlotIndex;
    }

    [[nodiscard]]
    bool canEval() const;

    virtual void acceptVisitor(CircuitVisitor& visitor) = 0;

    void clearCaches();

    void print() const;
};

using CircuitGatePtr = std::shared_ptr<CircuitGate>;

#endif //CIRCUIT_GATE_H
