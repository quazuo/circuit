#include "num-gate.h"
#include "../visitor.h"

void CircuitGate_ConstInt::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }

void CircuitGate_Add::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }

void CircuitGate_Mul::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }

void CircuitGate_CmpLe::acceptVisitor(CircuitVisitor &visitor) { visitor.visit(*this); }
