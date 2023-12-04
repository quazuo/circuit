#ifndef CIRCUIT_BOOLEAN_GATE_H
#define CIRCUIT_BOOLEAN_GATE_H

#include <iostream>
#include "../gate.h"

struct CircuitVisitor;

struct CircuitGate_ConstTrue : public CircuitGate {
    explicit CircuitGate_ConstTrue(ImVec2 _pos) : CircuitGate({}, {Bool}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "ConstantTrue"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

struct CircuitGate_Not : public CircuitGate {
    explicit CircuitGate_Not(ImVec2 _pos) : CircuitGate({Bool}, {Bool}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "Not"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

struct CircuitGate_And : public CircuitGate {
    explicit CircuitGate_And(ImVec2 _pos) : CircuitGate({Bool, Bool}, {Bool}, _pos) { }

    [[nodiscard]]
    std::string getName() const override { return "And"; }

    void acceptVisitor(CircuitVisitor& visitor) override;
};

#endif //CIRCUIT_BOOLEAN_GATE_H
