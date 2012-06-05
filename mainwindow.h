#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDeclarative/QDeclarativeView>

#include "mazewidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
  void fakeData(char, QTime);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  MazeWidget *maze;

private:
    Ui::MainWindow *ui;

private slots:
    void on_buttonPause_clicked();
    void on_configureButton_clicked();
    void on_buttonReset_clicked();
    void on_buttonPlay_clicked();

    void on_actionW_Maze_triggered();
    void on_actionU_Maze_triggered();
};

#endif // MAINWINDOW_H
