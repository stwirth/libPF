#include <cmath>
#include <libPF/CRandomNumberGenerator.h>

#include "CarStateDistribution.h"

CarStateDistribution::CarStateDistribution(float xmin, float xmax, float ymin, float ymax) :
        m_XMin(xmin),
        m_XMax(xmax),
        m_YMin(ymin),
        m_YMax(ymax)
{
    m_RNG = new libPF::CRandomNumberGenerator();
}

CarStateDistribution::~CarStateDistribution()
{
  delete m_RNG;
}

const CarState CarStateDistribution::draw() const
{
    CarState state;
    state.setXPos(m_RNG->getUniform(m_XMin, m_XMax));
    state.setYPos(m_RNG->getUniform(m_YMin, m_YMax));
    state.setTheta(m_RNG->getUniform(-M_PI, M_PI));
    return state;
}
