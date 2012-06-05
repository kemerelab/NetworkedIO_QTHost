#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QGridLayout>

#include "mazewidget.h"
#include "behaviorcontrol.h"

namespace Ui {
    class SettingsDialog;
}

class InputChannelSettings : public QObject
{
  Q_OBJECT

public:
  InputChannelSettings(QObject *parent, int chan, int IOpin, QString name, EdgeType edge);
  int chan;
  int IOpin;
  QString name;
  EdgeType edge;
  bool changed;

public slots:
  void channelChanged(int p) {changed = true; emit inputChannelChanged(chan, p);};
  void edgeChanged(int e) {changed = true; emit inputEdgeChanged(chan, (EdgeType) e);};

signals:
    void inputChannelChanged(int chan, int pin);
    void inputEdgeChanged(int chan, EdgeType e);
};

class OutputChannelSettings : public QObject
{
  Q_OBJECT

public:
  OutputChannelSettings(QObject *parent, int chan, int IOpin, QString name, int pulseLength);
  QString name;
  int chan;
  int IOpin;
  int pulseLength;
  bool changed;

public slots:
  void channelChanged(int p) {changed = true; emit outputChannelChanged(chan,p);};
  void pulseLengthChanged(int pl) {changed = true; emit outputPulseLengthChanged(chan, pl);};

signals:
    void outputChannelChanged(int chan, int pin);
    void outputPulseLengthChanged(int chan, int pl);
};


class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
  explicit SettingsDialog(MazeInfo *mazeInfo, QWidget *parent = 0);
  ~SettingsDialog();

  QList<InputChannelSettings *> InputSettings;
  QList<OutputChannelSettings *> OutputSettings;

  bool flagInputIPAddressChanged;
  QString newInputIPAddress;
  bool flagOutputIPAddressChanged;
  QString newOutputIPAddress;

private slots:
  void inputIPAddressChanged(QString newAddress) {
    flagInputIPAddressChanged = true;
    newInputIPAddress = newAddress;
  }
  void outputIPAddressChanged(QString newAddress) {
    flagOutputIPAddressChanged = true;
    newOutputIPAddress = newAddress;
  }

private:
    Ui::SettingsDialog *ui;

public slots:
  void settingsRejected();
  void settingsAccepted();

signals:

};

#endif // SETTINGSDIALOG_H
