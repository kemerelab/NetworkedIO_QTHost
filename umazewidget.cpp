#include "umazewidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>

UMazeWidget::UMazeWidget(QWidget *parent):
  MazeWidget(parent)
{
  // left == 0, right == 1
  BehavioralInputLabels.append("Left foodwell"); // left
  BehavioralInputLabels.append("Right foodwell"); // right
  OutputChannelLabels.append("Left food pump"); // left
  OutputChannelLabels.append("Right food pump"); // right

  rewardCounter.resize(2);

  QGridLayout *layout = new QGridLayout(this);


  layout->addWidget(new ImageTile(":/maze/images/track-end-vertical.svg", this), 0, 0);
  layout->addWidget(new ImageTile(":/maze/images/track-end-vertical.svg", this), 0, 1);

  leftFoodWell = new FoodWell(fwLeft,this);
  layout->addWidget(leftFoodWell,0,0);
  rightFoodWell = new FoodWell(fwRight,this);
  layout->addWidget(rightFoodWell,0,1);

  connect(leftFoodWell, SIGNAL(clicked(int)), this, SLOT(activateFoodWell(int)));
  connect(rightFoodWell, SIGNAL(clicked(int)), this, SLOT(activateFoodWell(int)));

  layout->addWidget(new ImageTile(":/maze/images/track-left-corner.svg", this), 1, 0);
  layout->addWidget(new ImageTile(":/maze/images/track-right-corner.svg", this), 1, 1);


  layout->setSpacing(0);
  layout->setMargin(0);

  setLayout(layout);

  resetControl();

  controlEnabled = false;
}

void UMazeWidget::resetControl(void) {
  lastFoodWell = fwEither;
  rewardCounter[fwLeft] = 0; rewardCounter[fwRight] = 0;
  leftFoodWell->setCounter(0);
  rightFoodWell->setCounter(0);
  disableControl();

  emit rewardCounterUpdate(rewardCounter);
}

void UMazeWidget::enableControl(void) {
  controlEnabled = true;
  switch (lastFoodWell) {
  case fwLeft:
    rightFoodWell->setDisplayState(FoodWell::enabled);
    break;
  case fwRight:
    leftFoodWell->setDisplayState(FoodWell::enabled);
    break;
  case fwEither:
  default:
    leftFoodWell->setDisplayState(FoodWell::enabled);
    rightFoodWell->setDisplayState(FoodWell::enabled);
    break;
  }
};

void UMazeWidget::disableControl(void) {
  controlEnabled = false;
  leftFoodWell->setDisplayState(FoodWell::disabled);
  rightFoodWell->setDisplayState(FoodWell::disabled);
};


void UMazeWidget::processFoodWellTrigger(int triggeredFoodWell) {
  if (controlEnabled) {
    if (triggeredFoodWell == fwLeft) {
      if ((lastFoodWell == fwRight) | (lastFoodWell == fwEither)) {
        // reward
        emit rewardFoodWell(fwLeft);
        lastFoodWell = fwLeft;
        rewardCounter[fwLeft]++;
        leftFoodWell->setCounter(rewardCounter[fwLeft]);
        leftFoodWell->setDisplayState(FoodWell::okLast);
        rightFoodWell->setDisplayState(FoodWell::enabled);
        emit rewardCounterUpdate(rewardCounter);
      }
      else {
        // wrong
        leftFoodWell->setDisplayState(FoodWell::wrongLast);
      }
    }
    else if (triggeredFoodWell == fwRight) {
      if ((lastFoodWell == fwLeft) | (lastFoodWell == fwEither)) {
        // reward
        emit rewardFoodWell(fwRight);
        lastFoodWell = fwRight;
        rewardCounter[fwRight]++;
        rightFoodWell->setCounter(rewardCounter[fwRight]);
        rightFoodWell->setDisplayState(FoodWell::okLast);
        leftFoodWell->setDisplayState(FoodWell::enabled);
        emit rewardCounterUpdate(rewardCounter);
      }
      else {
        // wrong
        rightFoodWell->setDisplayState(FoodWell::wrongLast);
      }
    }
  }

  qDebug() << "Got foodwell trigger" << BehavioralInputLabels[triggeredFoodWell] << "Control Enabled?" << controlEnabled;
}

UMazeWidget::~UMazeWidget()
{

}
