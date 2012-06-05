
#include "wmazewidget.h"
#include <QGridLayout>
#include <QKeyEvent>

WMazeWidget::WMazeWidget(QWidget *parent)
{
  BehavioralInputLabels.append("Left foodwell"); // left
  BehavioralInputLabels.append("Center foodwell"); // center
  BehavioralInputLabels.append("Right foodwell"); // right
  OutputChannelLabels.append("Left food pump"); // left
  OutputChannelLabels.append("Center food pump"); // right
  OutputChannelLabels.append("Right food pump"); // right

  rewardCounter.resize(3);

  QGridLayout *layout = new QGridLayout(this);

  layout->addWidget(new ImageTile(":/maze/images/track-end-vertical.svg", this), 0, 0);
  layout->addWidget(new ImageTile(":/maze/images/track-end-vertical.svg", this), 0, 1);
  layout->addWidget(new ImageTile(":/maze/images/track-end-vertical.svg", this), 0, 2);

  leftFoodWell = new FoodWell(fwLeft,this);
  layout->addWidget(leftFoodWell,0,0);
  centerFoodWell = new FoodWell(fwCenter,this);
  layout->addWidget(centerFoodWell,0,1);
  rightFoodWell = new FoodWell(fwRight,this);
  layout->addWidget(rightFoodWell,0,2);

  connect(leftFoodWell, SIGNAL(clicked(int)), this, SLOT(activateFoodWell(int)));
  connect(centerFoodWell, SIGNAL(clicked(int)), this, SLOT(activateFoodWell(int)));
  connect(rightFoodWell, SIGNAL(clicked(int)), this, SLOT(activateFoodWell(int)));

  layout->addWidget(new ImageTile(":/maze/images/track-left-corner.svg", this),1,0);
  layout->addWidget(new ImageTile(":/maze/images/track-right-corner.svg", this),1,2);
  layout->addWidget(new ImageTile(":/maze/images/track-tee.svg", this),1,1);

  layout->setSpacing(0);
  layout->setMargin(0);

  setLayout(layout);

  resetControl();
}

void WMazeWidget::resetControl(void) {
  lastFoodWell = fwAny;
  lastLastFoodWell = fwAny;
  rewardCounter[fwLeft] = 0; rewardCounter[fwRight] = 0; rewardCounter[fwCenter] = 0;
  leftFoodWell->setCounter(0);
  rightFoodWell->setCounter(0);
  centerFoodWell->setCounter(0);
  disableControl();

  emit rewardCounterUpdate(rewardCounter);
}

void WMazeWidget::enableControl(void) {
  controlEnabled = true;
  switch (lastFoodWell) {
  case fwLeft:
  case fwRight:
    centerFoodWell->setDisplayState(FoodWell::enabled);
    break;
  case fwCenter:
    if (lastLastFoodWell == fwLeft)
      rightFoodWell->setDisplayState(FoodWell::enabled);
    else if (lastLastFoodWell == fwRight)
      leftFoodWell->setDisplayState(FoodWell::enabled);
    else {
      rightFoodWell->setDisplayState(FoodWell::enabled);
      leftFoodWell->setDisplayState(FoodWell::enabled);
    }
    break;
  case fwEitherOutside:
    centerFoodWell->setDisplayState(FoodWell::enabled);
    break;
  case fwAny:
  default:
    centerFoodWell->setDisplayState(FoodWell::enabled);
    rightFoodWell->setDisplayState(FoodWell::enabled);
    leftFoodWell->setDisplayState(FoodWell::enabled);
    break;
  }
};

void WMazeWidget::disableControl(void) {
  controlEnabled = false;
  leftFoodWell->setDisplayState(FoodWell::disabled);
  centerFoodWell->setDisplayState(FoodWell::disabled);
  rightFoodWell->setDisplayState(FoodWell::disabled);
};


void WMazeWidget::processFoodWellTrigger(int triggeredFoodWell) {
  if (controlEnabled) {
    if (triggeredFoodWell == fwLeft) {
      if (((lastFoodWell == fwCenter) & (lastLastFoodWell == fwRight)) | (lastFoodWell == fwAny)) {
        // reward
        emit rewardFoodWell(fwLeft);
        lastLastFoodWell = lastFoodWell;
        lastFoodWell = fwLeft;
        rewardCounter[fwLeft]++;
        leftFoodWell->setCounter(rewardCounter[fwLeft]);
        leftFoodWell->setDisplayState(FoodWell::okLast);
        rightFoodWell->setDisplayState(FoodWell::disabled);
        centerFoodWell->setDisplayState(FoodWell::enabled);
        emit rewardCounterUpdate(rewardCounter);
      }
      else {
        if (lastFoodWell != fwLeft) {
        // wrong
          lastLastFoodWell = lastFoodWell;
          lastFoodWell = fwLeft;
          leftFoodWell->setDisplayState(FoodWell::wrongLast);
          rightFoodWell->setDisplayState(FoodWell::disabled);
          centerFoodWell->setDisplayState(FoodWell::enabled);
        }
      }
    }
    else if (triggeredFoodWell == fwRight) {
        if (((lastFoodWell == fwCenter) &
 ((lastLastFoodWell == fwLeft) | (lastLastFoodWell == fwAny) | (lastLastFoodWell==fwEitherOutside))) |
              (lastFoodWell == fwAny)) {
        // reward
        emit rewardFoodWell(fwRight);
        lastLastFoodWell = lastFoodWell;
        lastFoodWell = fwRight;
        rewardCounter[fwRight]++;
        rightFoodWell->setCounter(rewardCounter[fwRight]);
        rightFoodWell->setDisplayState(FoodWell::okLast);
        leftFoodWell->setDisplayState(FoodWell::disabled);
        centerFoodWell->setDisplayState(FoodWell::enabled);
        emit rewardCounterUpdate(rewardCounter);
      }
      else {
        if (lastFoodWell != fwRight) {
        // wrong
          lastLastFoodWell = lastFoodWell;
          lastFoodWell = fwRight;
          rightFoodWell->setDisplayState(FoodWell::wrongLast);
          leftFoodWell->setDisplayState(FoodWell::disabled);
          centerFoodWell->setDisplayState(FoodWell::enabled);
        }
      }
    }
    else if (triggeredFoodWell == fwCenter) {
      if ((lastFoodWell == fwLeft) | (lastFoodWell == fwRight) | (lastFoodWell == fwEitherOutside) | (lastFoodWell == fwAny)) {
        // reward
        emit rewardFoodWell(fwCenter);
        if (lastFoodWell == fwLeft) {
          rightFoodWell->setDisplayState(FoodWell::enabled);
          leftFoodWell->setDisplayState(FoodWell::disabled);
        }
        else if (lastFoodWell == fwRight) {
          leftFoodWell->setDisplayState(FoodWell::enabled);
          rightFoodWell->setDisplayState(FoodWell::disabled);
        }
        else if ((lastFoodWell == fwEitherOutside) | (lastFoodWell == fwAny)) {
          leftFoodWell->setDisplayState(FoodWell::enabled);
          rightFoodWell->setDisplayState(FoodWell::enabled);
        }
        lastLastFoodWell = lastFoodWell;
        lastFoodWell = fwCenter;
        rewardCounter[fwCenter]++;
        centerFoodWell->setCounter(rewardCounter[fwCenter]);
        centerFoodWell->setDisplayState(FoodWell::okLast);
        emit rewardCounterUpdate(rewardCounter);
      }
      else {
        // repeat at center
      }
    }
  }
}

WMazeWidget::~WMazeWidget()
{

}
