#ifndef MYMOVEMENTMODEL_H
#define MYMOVEMENTMODEL_H  


#include <libPF/MovementModel.h>
#include <libPF/CRandomNumberGenerator.h>

/** 
 * @class MyMovementModel
 *
 * @brief Test class for ParticleFilter.
 *
 * This simple movement model only adds a small jitter in diffuse() and does
 * nothing in drift().
 * @author Stephan Wirth
 */
class MyMovementModel : public libPF::MovementModel<float> {
    
  public:
    /**
     * empty 
     */
    MyMovementModel();

    /**
     * empty
     */
    ~MyMovementModel();

    /**
     * The drift method is empty in this example.
     * @param state Pointer to the state that has to be manipulated.
     */
    void drift(float& state, double dt) const;

    /**
     * The diffusion consists of a very small gaussian jitter on the
     * state's variable.
     * @param state Pointer to the state that has to be manipulated.
     */
    void diffuse(float& state, double dt) const;

  protected:

  private:

    /// The random number generator
    libPF::CRandomNumberGenerator m_RNG;

};

#endif

