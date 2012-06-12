#include "MyObservationModel.h"

MyObservationModel::MyObservationModel() : libPF::ObservationModel<float>() {
}

MyObservationModel::~MyObservationModel() {
}

double MyObservationModel::measure(const float& state) const {
  return 1.0/(fabs(state * state - 2));
}


