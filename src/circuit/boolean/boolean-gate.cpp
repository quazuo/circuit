#include "boolean-gate.h"
#include "../visitor.h"

void CircuitGate_ConstTrue::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }

void CircuitGate_Not::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }

void CircuitGate_And::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }
