#include <QDebug>
#include "CarObservationModel.h"

CarObservationModel::CarObservationModel() : libPF::ObservationModel<CarState>() {
}

CarObservationModel::~CarObservationModel() {
}

double CarObservationModel::measure(const CarState& state) const {
    double xdist = (state.getXPos() - m_TrueCarState.getXPos());
    double ydist = (state.getYPos() - m_TrueCarState.getYPos());
    double dist2 = (xdist*xdist + ydist*ydist);
    if (dist2 < 0.5) dist2 = 0.5; // avoid division by zero
    return 1.0/dist2;
}

void CarObservationModel::setTrueCarState(const CarState& state)
{
    m_TrueCarState = state;
}
