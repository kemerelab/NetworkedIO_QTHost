#ifndef WMAZEWIDGET_H
#define WMAZEWIDGET_H

#include <QWidget>

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include "mazewidget.h"


class WMazeWidget : public MazeWidget
{
  Q_OBJECT
public:
  explicit WMazeWidget(QWidget *parent = 0);
  ~WMazeWidget();

  enum foodWellId {fwLeft, fwCenter, fwRight, fwEitherOutside, fwAny};

public slots:
  void processFoodWellTrigger(int triggeredChannel);
  void enableControl(void);
  void disableControl(void);
  void resetControl(void);
  void activateFoodWell(int which) {emit rewardFoodWell(which);};

private:
  foodWellId lastFoodWell, lastLastFoodWell;
  QVector<int> rewardCounter;
  FoodWell *leftFoodWell;
  FoodWell *centerFoodWell;
  FoodWell *rightFoodWell;

};

#endif // WMAZEWIDGET_H
