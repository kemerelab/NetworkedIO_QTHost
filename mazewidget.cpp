#include "mazewidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QKeyEvent>
#include <QDebug>

FoodWell::FoodWell(int id, QWidget *parent) :
  QPushButton(parent), id(id)
{
  counterText = new QLabel(this);
  counterText->setText(QString("%1").arg(0));
  counterText->setAlignment(Qt::AlignCenter);
  counterText->setStyleSheet("color:rgba(255,255,255,128)");
  counterText->setFont(QFont("Arial",36,3));
  setLayout(new QVBoxLayout(this));
  layout()->addWidget(counterText);
  layout()->setMargin(0);

  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  setFocusPolicy(Qt::NoFocus);

  foodWellState = disabled;
  setDisplayState(foodWellState);

  connect(this, SIGNAL(clicked()), this, SLOT(foodWellClicked()));
}

void FoodWell::setCounter(int newCount) {
  counterText->setText(QString("%1").arg(newCount));
}

void FoodWell::setDisplayState(foodWellStateEnum state) {
  switch (state) {
  case enabled:
    setStyleSheet("QPushButton:hover {background-color:rgba(200,200,50,128)}"
          "QPushButton:pressed {background-color:rgba(200,200,100,128)}"
          "QPushButton {margin:0; border:2px solid; border-color: rgba(100,200,100,128); background-color: none;}");
    break;
  case okLast:
    setStyleSheet("QPushButton:hover {background-color:rgba(255,255,100,128)}"
          "QPushButton:pressed {background-color:rgba(200,200,100,128)}"
          "QPushButton {margin:0; color: none; border:none; background-color: rgba(100,200,100,128);}");
    break;
  case wrongLast:
    setStyleSheet("QPushButton:hover {background-color:rgba(200,200,50,128)}"
          "QPushButton:pressed {background-color:rgba(200,200,100,128)}"
          "QPushButton {margin:0; color: none; border:none; background-color: rgba(255,128,128,128);}");
    break;
  case disabled:
  default:
    setStyleSheet("QPushButton:hover {background-color:rgba(200,200,50,128)}"
          "QPushButton:pressed {background-color:rgba(200,200,100,128)}"
          "QPushButton {margin:0; color: none; border:none; background-color: none;}");
    break;
  }
}


ImageTile::ImageTile(const QString &filename, QWidget *parent) :
  QLabel(parent)
{
  setPixmap(QPixmap(filename));
  setStyleSheet("QLabel {margin:0; background-color: none; padding: 0; border: none;}");
  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  setScaledContents(true);

}

MazeWidget::MazeWidget(QWidget *parent) :
  QWidget(parent)
{
  controlEnabled = false;
  setFocusPolicy(Qt::StrongFocus);
}

void MazeWidget::keyPressEvent(QKeyEvent *event) {
  if (!controlEnabled) {
      QWidget::keyPressEvent(event);
      return;
  }
  else {
    int fw = event->text().toInt();
    if (fw > 0 & fw <= BehavioralInputLabels.size())
      processFoodWellTrigger(fw-1);
    else
        QWidget::keyPressEvent(event);
  }
}


