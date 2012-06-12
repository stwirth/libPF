#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QGLWidget>
#include <map>
#include <ctime>

#include <libPF/ParticleFilter.h>

#include "CarMovementModel.h"
#include "CarObservationModel.h"
#include "CarState.h"

class QTimer;

class RenderWidget : public QGLWidget
{
	Q_OBJECT
	public:
		RenderWidget(QWidget *parent = 0);
		~RenderWidget();

                void setParticleFilter(libPF::ParticleFilter<CarState>* pf);
                void setTrueCarState(CarState& state);

                void setDrawParticleWeights(bool b);
	protected:
		void paintEvent(QPaintEvent *event);

                void mouseMoveEvent(QMouseEvent* event);

	private:
                void drawCarState(QPainter& painter, const CarState& state);

        private:
                libPF::ParticleFilter<CarState>* m_ParticleFilter;
                CarState m_TrueCarState;

                bool m_DrawParticleWeights;

                float m_Fps;

                clock_t m_LastDrawTime;

                QPoint m_MousePosition;
};

#endif
