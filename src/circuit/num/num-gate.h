#ifndef CIRCUIT_NUM_GATE_H
#define CIRCUIT_NUM_GATE_H

#include "../gate.h"

struct CircuitVisitor;

struct CircuitGate_ConstInt : public CircuitGate {
    explicit CircuitGate_ConstInt(ImVec2 _pos) : CircuitGate({}, {Int}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "Constant"; }

    int value = 0;

    void acceptVisitor(CircuitVisitor& visitor) override;
};

struct CircuitGate_Add : public CircuitGate {
    explicit CircuitGate_Add(ImVec2 _pos) : CircuitGate({Int, Int}, {Int}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "Add"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

struct CircuitGate_Mul : public CircuitGate {
    explicit CircuitGate_Mul(ImVec2 _pos) : CircuitGate({Int, Int}, {Int}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "Multiply"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

struct CircuitGate_CmpLe : public CircuitGate {
    explicit CircuitGate_CmpLe(ImVec2 _pos) : CircuitGate({Int, Int}, {Bool}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "Compare (<=)"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

#endif //CIRCUIT_NUM_GATE_H
