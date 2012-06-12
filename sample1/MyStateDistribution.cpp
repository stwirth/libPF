
#include "MyStateDistribution.h"

MyStateDistribution::MyStateDistribution()
{
}

MyStateDistribution::~MyStateDistribution()
{
}

const float MyStateDistribution::draw() const
{
    return m_RNG.getUniform(0.0, 2.0);
}
