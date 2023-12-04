#ifndef CIRCUIT_VISITOR_H
#define CIRCUIT_VISITOR_H

#include <functional>
#include "boolean/boolean-gate.h"
#include "num/num-gate.h"

struct CircuitVisitor {
    virtual ~CircuitVisitor() = default;

    virtual void visit(CircuitGate_ConstTrue& gate) { (void) gate; };
    virtual void visit(CircuitGate_Not& gate) { (void) gate; };
    virtual void visit(CircuitGate_And& gate) { (void) gate; };

    virtual void visit(CircuitGate_ConstInt& gate) { (void) gate; };
    virtual void visit(CircuitGate_Add& gate) { (void) gate; };
    virtual void visit(CircuitGate_Mul& gate) { (void) gate; };
    virtual void visit(CircuitGate_CmpLe& gate) { (void) gate; };
};

struct CircuitVisitor_Eval : public CircuitVisitor {
    void visit(CircuitGate_ConstTrue& gate) override {
        gate.outputs[0].value = true;
        gate.outputs[0].isEval = true;
    }

    void visit(CircuitGate_Not& gate) override {
        if (!gate.canEval() || !evalInputs(gate)) {
            isOk = false;
            return;
        }

        if (gate.outputs[0].isEval) return;

        const CircuitGate::OutputPin& pin = gate.getOutputForInput(0);
        gate.outputs[0].value = !get<bool>(pin.value);
        gate.outputs[0].isEval = true;
    }

    void visit(CircuitGate_And& gate) override {
        visitBinary<bool, bool, bool>(gate, [](bool a, bool b) { return a && b; });
    }

    void visit(CircuitGate_ConstInt& gate) override {
        gate.outputs[0].value = gate.value;
        gate.outputs[0].isEval = true;
    }

    void visit(CircuitGate_Add& gate) override {
        visitBinary<int, int, int>(gate, [](int a, int b) { return a + b; });
    }

    void visit(CircuitGate_Mul& gate) override {
        visitBinary<int, int, int>(gate, [](int a, int b) { return a * b; });
    }

    void visit(CircuitGate_CmpLe& gate) override {
        visitBinary<int, int, bool>(gate, [](int a, int b) { return a <= b; });
    }

    [[nodiscard]]
    bool didEvalCorrectly() const { return isOk; }

private:
    bool isOk = true;

    bool evalInputs(CircuitGate& gate) {
        for (size_t i = 0; i < gate.inputsCount; i++) {
            gate.inputs[i].destGate->acceptVisitor(*this);
            if (!gate.getOutputForInput(i).isEval) return false;
        }

        return true;
    }

    template<typename U1, typename U2, typename T>
    void visitBinary(CircuitGate& gate, std::function<T(U1, U2)> fn) {
        if (!gate.canEval() || !evalInputs(gate)) {
            isOk = false;
            return;
        }

        if (gate.outputs[0].isEval) return;

        const CircuitGate::OutputPin& pin0 = gate.getOutputForInput(0);
        const CircuitGate::OutputPin& pin1 = gate.getOutputForInput(1);
        gate.outputs[0].value = fn(get<U1>(pin0.value), get<U2>(pin1.value));
        gate.outputs[0].isEval = true;
    }
};

#endif //CIRCUIT_VISITOR_H
