#include <QDebug>
#include "behaviorcontrol.h"
#include "networkdatainterface.h"

BehaviorControl::BehaviorControl(QObject *parent) :
    QObject(parent)
{

  clockTimer = new QTimer(this);
  connect(clockTimer, SIGNAL(timeout()), this, SLOT(clockTimerTick()));

  timeElapsed = QTime(0,0,0,0);

  CurrentInputState = 0; // should be 255; // start out all inputs high

  for (int i = 0; i < 8; i++) {
    BehavioralInputChannels << QString("%1").arg(i);
    OutputChannels << QString("%1").arg(i);
  }

  /*  // future debouncing
  for (int i = 0; i < 8; i++) {
    debounceTime[i] = 250; // ms
    TimeLastChange.add(QTime(0,0,0,0);
  }*/
}

void BehaviorControl::initializeChannels(int nInputChannels, int nOutputChannels) {
  BehavioralInputMap.resize(nInputChannels);
  for (int i = 0; i < nInputChannels; i++) {
    BehavioralInputDirection << RisingEdge;
    BehavioralInputMap[i] = i;
  }

  OutputMap.resize(nOutputChannels);
  for (int i = 0; i < nOutputChannels; i++) {
    OutputChannelPulseLength << 100;
    OutputMap[i] = i;
  }
}

void BehaviorControl::countersChanged(QList<int> what) {
  qDebug() << "Received counter signal! " << what;
}

void BehaviorControl::clockTimerTick(void) {
  timeElapsed = timeElapsed.addMSecs(100);
  emit updateElapsedTime(timeElapsed.toString("h:mm:ss.zzz"));
}

void BehaviorControl::startControl(void) {
  clockTimer->start(100);
  controlEnabled = true;
}

void BehaviorControl::pauseControl(void) {
  clockTimer->stop();
  controlEnabled = false;
}

void BehaviorControl::resetControl(void) {
  clockTimer->stop();
  controlEnabled = false;
  timeElapsed.setHMS(0,0,0,0);
  emit updateElapsedTime(timeElapsed.toString("h:mm:ss.zzz"));
}

void BehaviorControl::triggerOutput(int chan) {
  qDebug() << "BControl triggering output" << chan;
  outputNetworkData->sendTriggerDatagram(OutputMap[chan],0);
}

void BehaviorControl::newDataReceived(char value, QTime t) {
  if (value == CurrentInputState) {
  }
  else {
    char changed = value ^ CurrentInputState;
    for (int i = 0; i < BehavioralInputMap.size(); i++) {
      char mask = 1 << BehavioralInputMap[i];
      if (changed & mask)  {
        if (value & mask) {
          // emit risingEdgeActivation(i);
          if (BehavioralInputDirection[i] == RisingEdge) {
            emit behavioralInputActive(i);
          }
        }
        else {
          // emit fallingEdgeActivation(i);
          if (BehavioralInputDirection[i] == FallingEdge) {
            emit behavioralInputActive(i);
          }
        }
      }
    }
  }
  CurrentInputState = value;

  // Record!!!
}
