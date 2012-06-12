#ifndef CAROBSERVATIONMODEL_H
#define CAROBSERVATIONMODEL_H

#include <vector>
#include <libPF/ObservationModel.h>

#include "CarState.h"

/** 
 * @class CarObservationModel
 *
 * @brief Observation model that measures the position of a car.
 *
 * @author Stephan Wirth
 *
 * @brief Test class for ParticleFilter.
 *
 */
class CarObservationModel : public libPF::ObservationModel<CarState> {
    
  public:

    /** 
     * empty
     */
    CarObservationModel();

    /**
     * empty
     */
    ~CarObservationModel();

    /**
     *
     * @param state Reference to the state that has to be weightened.
     * @return weight for the given state.
     */
    double measure(const CarState& state) const;

    void setTrueCarState(const CarState& state);

  protected:

  private:

    CarState m_TrueCarState;

};

#endif
