#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

#include <libPF/ParticleFilter.h>
#include "CarObservationModel.h"
#include "CarMovementModel.h"

#include "ui_MainWindowForm.h"
#include "CarState.h"



class MainWindow : public QMainWindow, private Ui::MainWindowForm
{
    Q_OBJECT

public:
    MainWindow(); 
    ~MainWindow();

public slots:

    void gameLoop();

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void simulateMovement();

protected slots:
    void initStates();

    void changeDisplaySettings();
    void changeRunMode();

    void resample();
    void drift();
    void diffuse();
    void measure();

    void redrawParticles();
    void setResamplingMode();

    void setMovementModelParameters();

    void updateLabels();

private:
    void switchToSingleStepMode();

    CarMovementModel m_MovementModel;
    CarObservationModel m_ObservationModel;
    libPF::ParticleFilter<CarState> m_ParticleFilter;


    CarState m_TrueCarState;

    bool m_leftDown;
    bool m_rightDown;
    bool m_frontDown;
    bool m_backDown;

    bool m_RunFilter;

    QTime m_LoopTimer;

};

#endif // MAINWINDOW_H
