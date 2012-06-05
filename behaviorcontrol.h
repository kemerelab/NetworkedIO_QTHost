#ifndef BEHAVIORCONTROL_H
#define BEHAVIORCONTROL_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QStringList>


enum EdgeType {RisingEdge, FallingEdge};

class BehaviorControl : public QObject
{
    Q_OBJECT
public:
  explicit BehaviorControl(QObject *parent = 0);
  Q_INVOKABLE void countersChanged(QList<int>);
  void initializeChannels(int nInput, int nOutput);

signals:
  void updateElapsedTime(QString);
  //void updateRewardCount(QString);
  void risingEdgeActivation(int channel);
  void fallingEdgeActivation(int channel);
  void behavioralInputActive(int channel);

public slots:
  //void uiFoodWellInput(QString which);
  //void uiControlInput(QString name);
  void startControl(void);
  void pauseControl(void);
  void resetControl(void);

  void clockTimerTick(void);

  void triggerOutput(int mappedChannel);
  void newDataReceived(char, QTime);

  void inputChannelChangedSlot(int chan, int pin) {BehavioralInputMap[chan] = pin;};
  void inputEdgeChangedSlot(int chan, EdgeType e) {BehavioralInputDirection[chan] = e;};
  void outputChannelChangedSlot(int chan, int pin) {OutputMap[chan] = pin;};
  void outputPulseLengthChangedSlot(int chan, int pl) {OutputChannelPulseLength[chan] = pl;};


public:
  QVector<int> BehavioralInputMap;
  QVector<EdgeType> BehavioralInputDirection; // replace with enum
  QVector<int> OutputMap;
  QVector<int> OutputChannelPulseLength;

  QStringList BehavioralInputChannels;
  QStringList OutputChannels;

private:
  QTimer *clockTimer;
  bool controlEnabled;
  QTime timeElapsed;

  char CurrentInputState;
  //int  debounceTime[8];
  //QVector<QTime> TimeLastChange;

};

// Global def!
extern BehaviorControl *bControl;

#endif // BEHAVIORCONTROL_H
