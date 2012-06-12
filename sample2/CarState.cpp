#include <cmath>

#include "CarState.h"

CarState::CarState() :
        m_XPos(0.0),
        m_YPos(0.0),
        m_SinTheta(0.0),
        m_CosTheta(1.0),
        m_Speed(0.0),
        m_RotationSpeed(0.0)
{
}

CarState::~CarState()
{
}

CarState CarState::operator*(float factor) const
{
    CarState newState;
    newState.m_XPos = m_XPos * factor;
    newState.m_YPos = m_YPos * factor;
    newState.m_Speed = m_Speed * factor;
    newState.m_RotationSpeed = m_RotationSpeed * factor;
    newState.m_SinTheta = m_SinTheta * factor;
    newState.m_CosTheta = m_CosTheta * factor;
    return newState;
}

CarState& CarState::operator+=(const CarState& other)
{
    m_XPos += other.m_XPos;
    m_YPos += other.m_YPos;
    m_Speed += other.m_Speed;
    m_SinTheta += other.m_SinTheta;
    m_CosTheta += other.m_CosTheta;
    m_RotationSpeed += other.m_RotationSpeed;
    return *this;
}

float CarState::getXPos() const
{
    return m_XPos;
}

void CarState::setXPos(float x)
{
    m_XPos = x;
}

float CarState::getYPos() const
{
    return m_YPos;
}

void CarState::setYPos(float y)
{
    m_YPos = y;
}

float CarState::getTheta() const
{
    return atan2(m_SinTheta, m_CosTheta);
}

void CarState::setTheta(float t)
{
    m_SinTheta = sin(t);
    m_CosTheta = cos(t);
}

float CarState::getSpeed() const
{
    return m_Speed;
}

void CarState::setSpeed(float s)
{
    m_Speed = s;
}

float CarState::getRotationSpeed() const
{
    return m_RotationSpeed;
}

void CarState::setRotationSpeed(float s)
{
    m_RotationSpeed = s;
}
