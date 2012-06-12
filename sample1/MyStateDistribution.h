#ifndef MYSTATEDISTRIBUTION_H
#define MYSTATEDISTRIBUTION_H

#include <libPF/StateDistribution.h>
#include <libPF/CRandomNumberGenerator.h>

/**
 * @class MyStateDistribution
 *
 * @brief Distribution to draw a random state from
 *
 * As the state in this example is a simple float that tries
 * to "track" the squareroot of two, the distribution returns
 * a uniformly distributed number between zero and two.
 *
 * @author Stephan Wirth
 *
 */
class MyStateDistribution : public libPF::StateDistribution<float>
{
public:
    /**
     * Empty constructor
     */
    MyStateDistribution();

    /**
     * Empty destructor
     */
    ~MyStateDistribution();

    /**
     * @return a random number between 0 and 2
     */
    const float draw() const;

private:

    /// The random number generator
    libPF::CRandomNumberGenerator m_RNG;
};

#endif // MYSTATEDISTRIBUTION_H
