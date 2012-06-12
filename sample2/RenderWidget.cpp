#include "RenderWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>

#include <libPF/ParticleFilter.h>

#include "CarMovementModel.h"
#include "CarObservationModel.h"


using namespace libPF;

//-----------------------------------------------------------------------------
RenderWidget::RenderWidget(QWidget *parent /*= 0*/) : QGLWidget(parent),
  m_ParticleFilter(NULL),
  m_DrawParticleWeights(true)
{
   m_LastDrawTime = clock();
   setMouseTracking(true);
}
//-----------------------------------------------------------------------------
RenderWidget::~RenderWidget()
{
}

void RenderWidget::setParticleFilter(ParticleFilter<CarState>* filter)
{
    m_ParticleFilter = filter;
}

void RenderWidget::setTrueCarState(CarState& state)
{
    m_TrueCarState = state;
}

void RenderWidget::setDrawParticleWeights(bool b)
{
    m_DrawParticleWeights = b;
}

//-----------------------------------------------------------------------------
void RenderWidget::paintEvent(QPaintEvent* /*event*/)
{
        QPainter painter;
        painter.begin(this);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

        QColor color;
        unsigned int nearestToMouseIndex = m_ParticleFilter->numParticles();
        double minMouseDistance = width() + height(); // some high value
        for (unsigned int i = 0; i < m_ParticleFilter->numParticles(); ++i)
        {
            const CarState& state = m_ParticleFilter->getState(i);
            double weight = m_ParticleFilter->getWeight(i);
            float hue = pow(weight, 0.2);
            float value = pow(weight, 0.05);
            color.setHsvF(hue, 1.0, value);
            painter.setPen(color);
            drawCarState(painter, state);
            double xDist = pow(state.getXPos() - m_MousePosition.x(), 2);
            double yDist = pow(state.getYPos() - m_MousePosition.y(), 2);
            double distanceToMouse =  xDist + yDist;
            if (distanceToMouse < minMouseDistance)
            {
                minMouseDistance = distanceToMouse;
                nearestToMouseIndex = i;
            }
        }
        if (nearestToMouseIndex < m_ParticleFilter->numParticles())
        {
            double x = m_ParticleFilter->getState(nearestToMouseIndex).getXPos();
            double y = m_ParticleFilter->getState(nearestToMouseIndex).getYPos();
            double weight = m_ParticleFilter->getWeight(nearestToMouseIndex);
            painter.setPen(Qt::white);
            painter.drawText(m_MousePosition.x(), m_MousePosition.y(), QString::number(weight, 'f'));
            painter.drawLine(x, y, m_MousePosition.x(), m_MousePosition.y());
        }

        painter.setPen(Qt::red);
        drawCarState(painter, m_ParticleFilter->getBestState());

        painter.setPen(Qt::yellow);
        drawCarState(painter, m_ParticleFilter->getBestXPercentEstimate(20.0));

        painter.setPen(Qt::magenta);
        drawCarState(painter, m_ParticleFilter->getMmseEstimate());

        painter.setPen(Qt::white);
        drawCarState(painter, m_TrueCarState);

        if (m_DrawParticleWeights)
        {
            painter.setPen(Qt::green);
            // draw particle weights
            unsigned int num = m_ParticleFilter->numParticles();
            float ystart = height() - 1;
            for (unsigned int i = 0; i < num - 1; ++i)
            {
                float x1 = 1.0 * width() / num * i;
                float x2 = 1.0 * width() / num * (i+1);
                float scaledWeight1 = 1.0 / pow(2, -log(m_ParticleFilter->getWeight(i))); // log scale
                float scaledWeight2 = 1.0 / pow(2, -log(m_ParticleFilter->getWeight(i+1)));
                float y1 = ystart - scaledWeight1 * ystart;
                float y2 = ystart - scaledWeight2 * ystart;
                painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            }
        }


        clock_t currentTime = clock();
        double dt = ((double)currentTime - (double)m_LastDrawTime) / CLOCKS_PER_SEC;
        m_LastDrawTime = currentTime;
        float thisFps = dt;
        float alpha = 0.99;
        m_Fps = alpha * m_Fps + (1 - alpha) * thisFps;
        painter.drawText(10, 20, QString::number(1.0/m_Fps));
        painter.end();


}
//-----------------------------------------------------------------------------

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_MousePosition = event->pos();
}

//-----------------------------------------------------------------------------

void RenderWidget::drawCarState(QPainter& painter, const CarState& state)
{
    float x = state.getXPos();
    float y = state.getYPos();
    QRectF rect(x-4, y-4, 8, 8);
    painter.drawEllipse(rect);
    float xoff = 8.0 * cos(state.getTheta());
    float yoff = 8.0 * sin(state.getTheta());
    QLineF line(x, y, x+xoff, y+yoff);
    painter.drawLine(line);
}
