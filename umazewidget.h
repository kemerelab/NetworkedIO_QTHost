#ifndef UMAZEWIDGET_H
#define UMAZEWIDGET_H

#include <QWidget>
#include "mazewidget.h"


class UMazeWidget : public MazeWidget
{
  Q_OBJECT
public:
  explicit UMazeWidget(QWidget *parent = 0);
  ~UMazeWidget();
  enum foodWellId {fwLeft, fwRight, fwEither};

public slots:
  void processFoodWellTrigger(int triggeredChannel);
  void enableControl(void);
  void disableControl(void);
  void resetControl(void);
  void activateFoodWell(int which) {emit rewardFoodWell(which);};

private:
  foodWellId lastFoodWell;
  QVector<int> rewardCounter;
  FoodWell *leftFoodWell;
  FoodWell *rightFoodWell;

};

#endif // UMAZEWIDGET_H
