#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QLabel>

class MazeInfo
{
public:
  QStringList BehavioralInputLabels;
  QStringList OutputChannelLabels;
};

class MazeWidget : public QWidget, public MazeInfo
{
    Q_OBJECT
public:
  MazeWidget(QWidget *parent = 0);
  bool controlEnabled;
  //virtual ~MazeWidget();

signals:
  void rewardFoodWell(int rewardChannel);
  void rewardCounterUpdate(QVector<int> counters);

public slots:
  virtual void processFoodWellTrigger(int triggeredChannel) {};
  virtual void resetControl(void) {};
  virtual void enableControl(bool enable) {
    controlEnabled = enable;
    if (enable) {enableControl();}
    else {disableControl();};
  }
  virtual void enableControl(void) {controlEnabled = true;};
  virtual  void disableControl(void) {controlEnabled = false;};


protected:
    void keyPressEvent(QKeyEvent *event);

};

class FoodWell : public QPushButton
{
  Q_OBJECT
public:
  FoodWell(int id, QWidget *parent = 0);
  QLabel *counterText;
  enum foodWellStateEnum {disabled, enabled, okLast, wrongLast};

  void setCounter(int count);

public slots:
  void setDisplayState(foodWellStateEnum state);
  void foodWellClicked(void) {emit clicked(id);};

signals:
  void clicked(int id);

private:
  foodWellStateEnum foodWellState;
  int id;
};

class ImageTile : public QLabel
{
  Q_OBJECT

public:
  ImageTile(const QString &filename, QWidget *parent);

};


#endif // MAZEWIDGET_H
