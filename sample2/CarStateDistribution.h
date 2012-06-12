#ifndef CARSTATEDISTRIBUTION_H
#define CARSTATEDISTRIBUTION_H

#include <libPF/StateDistribution.h>

#include "CarState.h"

namespace libPF
{
    class RandomNumberGenerationStrategy;
}

class CarStateDistribution : public libPF::StateDistribution<CarState>
{
public:
    CarStateDistribution(float xmin, float xmax, float ymin, float ymax);
    ~CarStateDistribution();

    const CarState draw() const;

private:
    float m_XMin, m_XMax, m_YMin, m_YMax;

    libPF::RandomNumberGenerationStrategy* m_RNG;
};

#endif // CARSTATEDISTRIBUTION_H
