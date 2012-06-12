
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>
#include <QSignalMapper>
#include <QRadioButton>

#include <libPF/ParticleFilter.h>

#include "MainWindow.h"

#include "CarMovementModel.h"
#include "CarObservationModel.h"
#include "CarStateDistribution.h"

MainWindow::MainWindow() : QMainWindow(),
m_ParticleFilter(250, &m_ObservationModel, &m_MovementModel),
m_RunFilter(true)
{
    setupUi(this);

    this->grabKeyboard();

    m_frontDown = m_backDown = m_rightDown = m_leftDown = 0;
    m_RenderWidget->setParticleFilter(&m_ParticleFilter);

    connect(m_ParticleWeightPlotCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeDisplaySettings()));
    connect(m_LoopModeRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeRunMode()));
    connect(m_ResampleButton, SIGNAL(clicked()), this, SLOT(resample()));
    connect(m_DriftButton, SIGNAL(clicked()), this, SLOT(drift()));
    connect(m_DiffuseButton, SIGNAL(clicked()), this, SLOT(diffuse()));
    connect(m_MeasureButton, SIGNAL(clicked()), this, SLOT(measure()));
    connect(m_RedrawParticlesButton, SIGNAL(clicked()), this, SLOT(redrawParticles()));
    connect(m_ResamplingModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setResamplingMode()));

    // read standard values into spin boxes
    m_XSpinBox->setValue(m_MovementModel.getXStdDev());
    m_YSpinBox->setValue(m_MovementModel.getYStdDev());
    m_ThetaSpinBox->setValue(m_MovementModel.getThetaStdDev() / M_PI * 180.0);
    m_SpeedSpinBox->setValue(m_MovementModel.getSpeedStdDev());
    m_RotationSpeedSpinBox->setValue(m_MovementModel.getRotationSpeedStdDev() / M_PI * 180.0);

    // connect spin boxes
    connect(m_XSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMovementModelParameters()));
    connect(m_YSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMovementModelParameters()));
    connect(m_ThetaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMovementModelParameters()));
    connect(m_SpeedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMovementModelParameters()));
    connect(m_RotationSpeedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMovementModelParameters()));

    QTimer::singleShot(50, this, SLOT(initStates()));
    // start the loop
    QTimer::singleShot(100, this, SLOT(gameLoop()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::initStates()
{
    m_TrueCarState.setXPos(m_RenderWidget->width()/2); // x-position
    m_TrueCarState.setYPos(m_RenderWidget->height()/2); // y-position
    m_TrueCarState.setTheta(0.0);
    m_TrueCarState.setSpeed(0.0);
    m_TrueCarState.setRotationSpeed(0.0);
    m_ParticleFilter.setPriorState(m_TrueCarState);
}

void MainWindow::simulateMovement()
{
    if (m_LoopTimer.isNull())
    {
        m_LoopTimer.start();
        return;
    }
    double dt = m_LoopTimer.restart() / 1000.0;

    double acceleration = m_AccelerationSpinBox->value();
    double maxSpeed = m_MaxSpeedSpinBox->value();
    double newSpeed = m_TrueCarState.getSpeed();
    if (m_frontDown)
        newSpeed += acceleration * dt; // accelerate
    else if (m_backDown)
        newSpeed -= acceleration * dt; // break
    else
        newSpeed *= 0.99; // rolling
    // clamp speed
    if (newSpeed > maxSpeed) newSpeed = maxSpeed;
    if (newSpeed < -maxSpeed) newSpeed = -maxSpeed;
    m_TrueCarState.setSpeed(newSpeed);

    double torque = m_TorqueSpinBox->value() / 180.0 * M_PI;
    double maxRotSpeed = m_MaxRotationSpeedSpinBox->value() / 180.0 * M_PI;
    double newRotSpeed = m_TrueCarState.getRotationSpeed();
    if (m_leftDown)
        newRotSpeed -= torque * dt;
    else if (m_rightDown)
        newRotSpeed += torque * dt;
    else
        newRotSpeed *= 0.95; // rolling
    // clamp rotation speed
    if (newRotSpeed > maxRotSpeed) newRotSpeed = maxRotSpeed;
    if (newRotSpeed < -maxRotSpeed) newRotSpeed = -maxRotSpeed;
    m_TrueCarState.setRotationSpeed(newRotSpeed);

    // propagate state (we use our own movement model with fixed dt)
    m_MovementModel.drift(m_TrueCarState, dt);
    m_RenderWidget->setTrueCarState(m_TrueCarState);
}

void MainWindow::gameLoop()
{
    simulateMovement();
    // update the observation model
    m_ObservationModel.setTrueCarState(m_TrueCarState);
    if (m_RunFilter)
    {
      m_ParticleFilter.filter();
    }
    m_RenderWidget->repaint();
    updateLabels();
    QTimer::singleShot(10, this, SLOT(gameLoop()));
}

void MainWindow::changeDisplaySettings()
{
    if (m_ParticleWeightPlotCheckBox->isChecked())
    {
        m_RenderWidget->setDrawParticleWeights(true);
    } else
    {
        m_RenderWidget->setDrawParticleWeights(false);
    }
}

void MainWindow::changeRunMode()
{
    if (m_RunFilter == false)
    {
        m_LoopModeRadioButton->setChecked(true);
        m_SingleStepModeGroupBox->setEnabled(false);
        m_RunFilter = true;
        m_ParticleFilter.resetTimer();
    } else {
        m_SingleStepModeRadioButton->setChecked(true);
        m_SingleStepModeGroupBox->setEnabled(true);
        m_RunFilter = false;
    }
}

void MainWindow::resample()
{
    m_ParticleFilter.resample();
    m_RenderWidget->repaint();
    updateLabels();
}
void MainWindow::drift()
{
    float timestep = 1.0/60.0;
    m_ParticleFilter.drift(timestep);
    m_RenderWidget->repaint();
    updateLabels();
}
void MainWindow::diffuse()
{
    float timestep = 1.0/60.0;
    m_ParticleFilter.diffuse(timestep);
    m_RenderWidget->repaint();
    updateLabels();
}
void MainWindow::measure()
{
    m_ParticleFilter.measure();
    m_RenderWidget->repaint();
    updateLabels();
}

void MainWindow::redrawParticles()
{
    float xmin = 0;
    float xmax = m_RenderWidget->width();
    float ymin = 0;
    float ymax = m_RenderWidget->height();
    CarStateDistribution distribution(xmin, xmax, ymin, ymax);
    m_ParticleFilter.drawAllFromDistribution(distribution);
}

void MainWindow::setResamplingMode()
{
    QString text = m_ResamplingModeComboBox->currentText();
    if (text == "Always")
    {
        m_ParticleFilter.setResamplingMode(libPF::RESAMPLE_ALWAYS);
    } else if (text == "NEFF")
    {
        m_ParticleFilter.setResamplingMode(libPF::RESAMPLE_NEFF);
    } else if (text == "Never")
    {
        m_ParticleFilter.setResamplingMode(libPF::RESAMPLE_NEVER);
    }
}

void MainWindow::setMovementModelParameters()
{
    m_MovementModel.setXStdDev(m_XSpinBox->value());
    m_MovementModel.setYStdDev(m_YSpinBox->value());
    m_MovementModel.setThetaStdDev(m_ThetaSpinBox->value() / 180.0 * M_PI);
    m_MovementModel.setSpeedStdDev(m_SpeedSpinBox->value());
    m_MovementModel.setRotationSpeedStdDev(m_RotationSpeedSpinBox->value() / 180.0 * M_PI);
}

void MainWindow::updateLabels()
{
    int prec = 4;
    m_TrueXLabel->setText(QString::number(m_TrueCarState.getXPos(), 'f', prec));
    m_TrueYLabel->setText(QString::number(m_TrueCarState.getYPos(), 'f', prec));
    m_TrueThetaLabel->setText(QString::number(m_TrueCarState.getTheta() / M_PI * 180.0, 'f', prec));
    m_TrueSpeedLabel->setText(QString::number(m_TrueCarState.getSpeed(), 'f', prec));
    m_TrueRotationSpeedLabel->setText(QString::number(m_TrueCarState.getRotationSpeed() / M_PI * 180.0, 'f', prec));

    const CarState& estimatedCarState = m_ParticleFilter.getMmseEstimate();

    m_EstimatedXLabel->setText(QString::number(estimatedCarState.getXPos(), 'f', prec));
    m_EstimatedYLabel->setText(QString::number(estimatedCarState.getYPos(), 'f', prec));
    m_EstimatedThetaLabel->setText(QString::number(estimatedCarState.getTheta() / M_PI * 180.0, 'f', prec));
    m_EstimatedSpeedLabel->setText(QString::number(estimatedCarState.getSpeed(), 'f', prec));
    m_EstimatedRotationSpeedLabel->setText(QString::number(estimatedCarState.getRotationSpeed() / M_PI * 180.0, 'f', prec));

}

//-----------------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if ((e->key() == Qt::Key_W) || (e->key()==Qt::Key_Up))
        m_frontDown=true;
    else if ((e->key() == Qt::Key_S) || (e->key()==Qt::Key_Down))
        m_backDown=true;
    else if ((e->key() == Qt::Key_A) || (e->key()==Qt::Key_Left))
        m_leftDown=true;
    else if ((e->key() == Qt::Key_D) || (e->key()==Qt::Key_Right))
        m_rightDown=true;
    else if (e->key() == Qt::Key_R)
    {
        initStates();
    }
    else if (e->key() == Qt::Key_1)
        resample();
    else if (e->key() == Qt::Key_2)
        drift();
    else if (e->key() == Qt::Key_3)
        diffuse();
    else if (e->key() == Qt::Key_4)
        measure();
    else if (e->key() == Qt::Key_Tab)
        changeRunMode();
    else
        e->ignore();
}
//-----------------------------------------------------------------------------
void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    if ((e->key() == Qt::Key_W) || (e->key()==Qt::Key_Up))
        m_frontDown=false;
    else if ((e->key() == Qt::Key_S) || (e->key()==Qt::Key_Down))
        m_backDown=false;
    else if ((e->key() == Qt::Key_A) || (e->key()==Qt::Key_Left))
        m_leftDown=false;
    else if ((e->key() == Qt::Key_D) || (e->key()==Qt::Key_Right))
        m_rightDown=false;
}
//-----------------------------------------------------------------------------
