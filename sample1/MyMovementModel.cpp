#include <libPF/CRandomNumberGenerator.h>

#include "MyMovementModel.h"

MyMovementModel::MyMovementModel() : libPF::MovementModel<float>() {
}

MyMovementModel::~MyMovementModel() {
}

void MyMovementModel::drift(float& /*state*/, double /*dt*/) const {
}

void MyMovementModel::diffuse(float& state, double /*dt*/) const {
  state = state + m_RNG.getGaussian(0.0001);
}
