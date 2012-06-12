#include <libPF/CRandomNumberGenerator.h>
#include "CarMovementModel.h"

using namespace std;

CarMovementModel::CarMovementModel() : libPF::MovementModel<CarState>() ,
    m_XStdDev(10.0),
    m_YStdDev(10.0),
    m_ThetaStdDev(0.0),
    m_SpeedStdDev(400.0),
    m_RotSpeedStdDev(1000.0 / 180.0 * M_PI)
{
    m_RNG = new libPF::CRandomNumberGenerator();
}

CarMovementModel::~CarMovementModel() {
    delete m_RNG;
}

void CarMovementModel::drift(CarState& state, double dt) const {
    double speed = state.getSpeed();
    double orientation = state.getTheta();
    state.setXPos(state.getXPos() + cos(orientation) * dt * speed);
    state.setYPos(state.getYPos() + sin(orientation) * dt * speed);
    state.setTheta(state.getTheta() + dt * state.getRotationSpeed());
}

void CarMovementModel::diffuse(CarState& state, double dt) const {
    state.setXPos(state.getXPos() + m_RNG->getGaussian(m_XStdDev) * dt);
    state.setYPos(state.getYPos() + m_RNG->getGaussian(m_YStdDev) * dt);
    state.setSpeed(state.getSpeed() + m_RNG->getGaussian(m_SpeedStdDev) * dt);
    state.setTheta(state.getTheta() + m_RNG->getGaussian(m_ThetaStdDev) * dt);
    state.setRotationSpeed(state.getRotationSpeed() + m_RNG->getGaussian(m_RotSpeedStdDev) * dt);
}

void CarMovementModel::setXStdDev(double d)
{
    m_XStdDev = d;
}

double CarMovementModel::getXStdDev() const
{
    return m_XStdDev;
}

void CarMovementModel::setYStdDev(double d)
{
    m_YStdDev = d;
}

double CarMovementModel::getYStdDev() const
{
    return m_YStdDev;
}

void CarMovementModel::setThetaStdDev(double d)
{
    m_ThetaStdDev = d;
}

double CarMovementModel::getThetaStdDev() const
{
    return m_ThetaStdDev;
}

void CarMovementModel::setSpeedStdDev(double d)
{
    m_SpeedStdDev = d;
}

double CarMovementModel::getSpeedStdDev() const
{
    return m_SpeedStdDev;
}

void CarMovementModel::setRotationSpeedStdDev(double d)
{
    m_RotSpeedStdDev = d;
}

double CarMovementModel::getRotationSpeedStdDev() const
{
    return m_RotSpeedStdDev;
}
