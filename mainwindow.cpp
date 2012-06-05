#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "behaviorcontrol.h"
#include "networkdatainterface.h"
#include "umazewidget.h"
#include "wmazewidget.h"

#include "settingsdialog.h"

#include <QDebug>
#include <QtGui>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(bControl,SIGNAL(updateElapsedTime(QString)),ui->labelTimeElapsed,SLOT(setText(QString)));
    //connect(ui->configureButton,SIGNAL(clicked()),inputNetworkData,SLOT(sendQueryDatagram()));
    connect(this,SIGNAL(fakeData(char,QTime)),bControl, SLOT(newDataReceived(char,QTime)));

    maze = new MazeWidget(ui->mazeFrameWidget);

    QVBoxLayout *frameLayout = new QVBoxLayout(ui->mazeFrameWidget);
    frameLayout->addWidget(maze);
    ui->mazeFrameWidget->setLayout(frameLayout);

    QGridLayout *layout = new QGridLayout(maze);
    QPushButton *uMazeButton = new QPushButton(maze);
    uMazeButton->setText("U-Maze");
    uMazeButton->setStyleSheet("padding:25px 25px");
    layout->addWidget(uMazeButton,1,1);
    QPushButton *wMazeButton = new QPushButton(maze);
    wMazeButton->setText("W-Maze");
    wMazeButton->setStyleSheet("padding:25px 25px");
    layout->addWidget(wMazeButton,1,2);
    layout->setColumnStretch(3,0);
    layout->setRowStretch(2,0);

    connect(uMazeButton,SIGNAL(clicked()),this,SLOT(on_actionU_Maze_triggered()));
    connect(wMazeButton,SIGNAL(clicked()),this,SLOT(on_actionW_Maze_triggered()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonPlay_clicked()
{
  bControl->startControl();
  maze->enableControl();
  ui->buttonPause->setEnabled(true);
  ui->action_Pause_Control->setEnabled(true);
}

void MainWindow::on_buttonPause_clicked()
{
  ui->buttonPlay->setChecked(false);
  ui->buttonPause->setDisabled(true);
  bControl->pauseControl();
  maze->disableControl();
}

void MainWindow::on_buttonReset_clicked()
{
  bControl->resetControl();
  maze->resetControl();
  ui->buttonPlay->setChecked(false);
  ui->buttonPause->setDisabled(true);
}

void MainWindow::on_configureButton_clicked()
{
  SettingsDialog configureDialog(maze, this);
  configureDialog.exec();
}


void MainWindow::on_actionW_Maze_triggered()
{
    if (!maze->controlEnabled) {
      disconnect(maze,0,0,0);
      disconnect(bControl,0,maze,0);
      delete(maze);
      maze = new WMazeWidget(ui->mazeFrameWidget);
      ui->mazeFrameWidget->layout()->addWidget(maze);
      bControl->initializeChannels(maze->BehavioralInputLabels.size(),maze->OutputChannelLabels.size());
      connect(bControl,SIGNAL(behavioralInputActive(int)),maze,SLOT(processFoodWellTrigger(int)));
      connect(maze, SIGNAL(rewardFoodWell(int)), bControl, SLOT(triggerOutput(int)));
      //connect(maze, SIGNAL(rewardCounterUpdate(QVector<int>)), bControl, SLOT())
    }
    else {
      QMessageBox msgBox;
      msgBox.setText("Cannot load a new maze while behavior is enabled.");
      msgBox.exec();
    }
}

void MainWindow::on_actionU_Maze_triggered()
{
  if (!maze->controlEnabled) {
    disconnect(maze,0,0,0);
    disconnect(bControl,0,maze,0);
    delete(maze);
    maze = new UMazeWidget(ui->mazeFrameWidget);
    ui->mazeFrameWidget->layout()->addWidget(maze);
    bControl->initializeChannels(maze->BehavioralInputLabels.size(),maze->OutputChannelLabels.size());
    connect(bControl,SIGNAL(behavioralInputActive(int)),maze,SLOT(processFoodWellTrigger(int)));
    connect(maze, SIGNAL(rewardFoodWell(int)), bControl, SLOT(triggerOutput(int)));
    //connect(maze, SIGNAL(rewardCounterUpdate(QVector<int>)), bControl, SLOT())
  }
  else {
    QMessageBox msgBox;
    msgBox.setText("Cannot load a new maze while behavior is enabled.");
    msgBox.exec();
  }
}
