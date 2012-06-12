#ifndef CARSTATE_H
#define CARSTATE_H


/**
 * @class CarState
 * @brief Sample state for a particle filter that simulates a car.
 *
 * This state has the following parameters:
 * @li <b>xpos</b> the x-Position of the car
 * @li <b>ypos</b> the y-Position of the car
 * @li <b>theta</b> the orientation of the car (in radiants)
 * @li <b>speed</b> the forward speed of the car
 * @li <b>rotationSpeed</b> the speed with which the car rotates.
 */
class CarState
{
public:
    CarState();
    ~CarState();

    CarState operator*(float factor) const;

    CarState& operator+=(const CarState& other);


    float getXPos() const;

    void setXPos(float x);

    float getYPos() const;

    void setYPos(float y);

    float getTheta() const;

    void setTheta(float t);

    float getSpeed() const;

    void setSpeed(float s);

    float getRotationSpeed() const;

    void setRotationSpeed(float s);


private:

    float m_XPos;
    float m_YPos;
    float m_SinTheta;
    float m_CosTheta;
    float m_Speed;
    float m_RotationSpeed;
};

#endif // CARSTATE_H
