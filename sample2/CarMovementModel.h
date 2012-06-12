#ifndef CARMOVEMENTMODEL_H
#define CARMOVEMENTMODEL_H

#include <libPF/MovementModel.h>

#include "CarState.h"

class RandomNumberGenerator;

/** 
 * @class CarMovementModel
 *
 * @brief Test class for ParticleFilter.
 *
 * This movement model propagates a car state according to the translation and
 * rotation speed.
 *
 * @author Stephan Wirth
 */
class CarMovementModel : public libPF::MovementModel<CarState> {
    
  public:
    /**
     * Constructor
     */
    CarMovementModel();

    /**
     * Destructor
     */
    ~CarMovementModel();

    /**
     * The drift method propagates the car using its speed.
     * @param state Pointer to the state that has to be manipulated.
     */
    void drift(CarState& state, double dt) const;

    /**
     * The diffusion consists of a very small gaussian jitter on the
     * state's variable.
     * @param state Pointer to the state that has to be manipulated.
     */
    void diffuse(CarState& state, double dt) const;

    /**
     * @param d new standard deviation for the diffusion of x
     */
    void setXStdDev(double d);

    /**
     * @return the standard deviation for the diffusion of x
     */
    double getXStdDev() const;

    /**
     * @param d new standard deviation for the diffusion of y
     */
    void setYStdDev(double d);

    /**
     * @return the standard deviation for the diffusion of y
     */
    double getYStdDev() const;

    /**
     * @param d new standard deviation for the diffusion of theta
     */
    void setThetaStdDev(double d);

    /**
     * @return the standard deviation for the diffusion of theta
     */
    double getThetaStdDev() const;

    /**
     * @param d new standard deviation for the diffusion of speed
     */
    void setSpeedStdDev(double d);

    /**
     * @return the standard deviation for the diffusion of speed
     */
    double getSpeedStdDev() const;

    /**
     * @param d new standard deviation for the diffusion of rotation speed
     */
    void setRotationSpeedStdDev(double d);

    /**
     * @return the standard deviation for the diffusion of rotation speed
     */
    double getRotationSpeedStdDev() const;

  protected:

  private:

    /// Stores the random number generator
    libPF::RandomNumberGenerationStrategy* m_RNG;

    /// Store the standard deviations of the model
    double m_XStdDev;
    double m_YStdDev;
    double m_ThetaStdDev;
    double m_SpeedStdDev;
    double m_RotSpeedStdDev;


};

#endif

