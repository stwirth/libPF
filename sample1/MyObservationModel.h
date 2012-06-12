#ifndef MYOBSERVATIONMODEL_H
#define MYOBSERVATIONMODEL_H  

#include <vector>

#include <libPF/ObservationModel.h>

/** 
 * @class MyObservationModel
 *
 * @brief Observation model that measures a MyState.
 *
 * The measurement is made according to the formula
 * \f[
 *     w = \frac{1}{|x^2 - 2|}
 * \f]
 * where x is the variable of MyState and w is the weight that is returned.
 * This is a measure for the distance from x to the squareroot of two. If
 * The distance is low, the returned weight is high.
 *
 * @author Stephan Wirth
 *
 * @brief Test class for ParticleFilter.
 *
 */
class MyObservationModel : public libPF::ObservationModel<float> {
    
  public:

    /** 
     * empty
     */
    MyObservationModel();

    /**
     * empty
     */
    ~MyObservationModel();

    /**
     *
     * @param state Reference to the state that has to be weightened.
     * @return weight for the given state.
     */
    double measure(const float& state) const;

  protected:

  private:

};

#endif
