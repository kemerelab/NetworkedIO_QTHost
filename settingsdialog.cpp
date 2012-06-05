#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "behaviorcontrol.h"
#include "networkdatainterface.h"

#include <QComboBox>
#include <QButtonGroup>
#include <QToolButton>
#include <QSpinBox>

InputChannelSettings::InputChannelSettings(QObject *parent, int chan, int IOpin, QString name, EdgeType edge) :
  QObject(parent), chan(chan), name(name), IOpin(IOpin), edge(edge)
{
}

OutputChannelSettings::OutputChannelSettings(QObject *parent, int chan, int IOpin, QString name, int pulseLength) :
  QObject(parent), chan(chan), name(name), IOpin(IOpin), pulseLength(pulseLength)
{
}


SettingsDialog::SettingsDialog(MazeInfo *mazeInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    /* ---------------------------------------------------------------------- */
    /* Input Settings */
    ui->textInputIPAddress->setText(inputNetworkData->host.toString());
    flagInputIPAddressChanged = false;
    QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    ui->textInputIPAddress->setValidator(new QRegExpValidator(
             QRegExp("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$"), this));

    connect(ui->textInputIPAddress,SIGNAL(textChanged(QString)),this,SLOT(inputIPAddressChanged(QString)));

    QGridLayout *inputLayout = (QGridLayout *)ui->inputSettingsTab->layout();
    for (int i = 0; i < mazeInfo->BehavioralInputLabels.length(); i++) {
      InputChannelSettings *ics = new InputChannelSettings(this,i,bControl->BehavioralInputMap[i],
             mazeInfo->BehavioralInputLabels[i], bControl->BehavioralInputDirection[i]);
      InputSettings.append(ics);

      QLabel *label = new QLabel(this);
      label->setText(ics->name);

      int gridRow = inputLayout->rowCount();
      inputLayout->addWidget(label,gridRow+1,0);

      QComboBox *channelComboBox = new QComboBox(this);
      channelComboBox->addItems(bControl->BehavioralInputChannels);
      channelComboBox->setCurrentIndex(bControl->BehavioralInputMap[i]);
      connect(channelComboBox, SIGNAL(currentIndexChanged(int)), ics, SLOT(channelChanged(int)));
      connect(ics,SIGNAL(inputChannelChanged(int,int)), bControl, SLOT(inputChannelChangedSlot(int,int)));
      inputLayout->addWidget(channelComboBox,gridRow+1,1);

      QToolButton *risingEdge = new QToolButton(this);
      risingEdge->setText("R");
      risingEdge->setCheckable(true);
      risingEdge->setIcon(QIcon(":/icons/images/rising-edge.svg"));
      QToolButton *fallingEdge = new QToolButton(this);
      fallingEdge->setText("F");
      fallingEdge->setCheckable(true);
      fallingEdge->setIcon(QIcon(":/icons/images/falling-edge.svg"));
      QButtonGroup *selectEdge = new QButtonGroup(this);
      selectEdge->addButton(risingEdge,RisingEdge);
      selectEdge->addButton(fallingEdge,FallingEdge);
      if (ics->edge == RisingEdge)
        risingEdge->setChecked(true);
      else
        fallingEdge->setChecked(true);
      connect(selectEdge, SIGNAL(buttonClicked(int)), ics, SLOT(edgeChanged(int)));
      connect(ics,SIGNAL(inputEdgeChanged(int,EdgeType)), bControl, SLOT(inputEdgeChangedSlot(int,EdgeType)));

      QHBoxLayout *hlayout1 = new QHBoxLayout();
      hlayout1->addWidget(risingEdge,1);
      hlayout1->addWidget(fallingEdge,1);
      hlayout1->setMargin(0);
      inputLayout->addLayout(hlayout1,gridRow+1,2);
    }

    inputLayout->setRowStretch(1,2);
    inputLayout->setRowStretch(inputLayout->rowCount()+1,2);

    /* ---------------------------------------------------------------------- */
    /* Output Settings */
    ui->textOutputIPAddress->setText(outputNetworkData->host.toString());
    ui->textOutputIPAddress->setValidator(new QRegExpValidator(
             QRegExp("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$"), this));
    flagOutputIPAddressChanged = false;
    connect(ui->textOutputIPAddress,SIGNAL(textChanged(QString)),this,SLOT(outputIPAddressChanged(QString)));

    QGridLayout *outputLayout = (QGridLayout *)ui->outputSettingsTab->layout();
    for (int i = 0; i < mazeInfo->OutputChannelLabels.length(); i++) {
      OutputChannelSettings *ocs = new OutputChannelSettings(this,i,
             bControl->OutputMap[i],mazeInfo->OutputChannelLabels[i],
             bControl->OutputChannelPulseLength[i]);
      OutputSettings.append(ocs);
      QLabel *label = new QLabel();
      label->setText(ocs->name);

      int gridRow = outputLayout->rowCount();
      outputLayout->addWidget(label,gridRow+1,0);

      QComboBox *channelComboBox = new QComboBox();
      channelComboBox->addItems(bControl->OutputChannels);
      channelComboBox->setCurrentIndex(bControl->OutputMap[i]);
      connect(channelComboBox, SIGNAL(activated(int)), ocs, SLOT(channelChanged(int)));
      connect(ocs,SIGNAL(outputChannelChanged(int,int)),bControl,SLOT(outputChannelChangedSlot(int,int)));
      outputLayout->addWidget(channelComboBox,gridRow+1,1);

      QSpinBox *pulseDuration = new QSpinBox(this);
      pulseDuration->setRange(10,1000);
      pulseDuration->setValue(ocs->pulseLength);
      pulseDuration->setSuffix(" ms");
      pulseDuration->setAlignment(Qt::AlignRight);
      outputLayout->addWidget(pulseDuration,gridRow+1,2);
      connect(pulseDuration,SIGNAL(valueChanged(int)),ocs,SLOT(pulseLengthChanged(int)));
      connect(ocs,SIGNAL(outputPulseLengthChanged(int,int)),bControl,SLOT(outputPulseLengthChangedSlot(int,int)));
    }

    outputLayout->setRowStretch(1,2);
    outputLayout->setRowStretch(outputLayout->rowCount()+1,2);

    connect(this,SIGNAL(rejected()),this,SLOT(settingsRejected()));

    connect(this,SIGNAL(accepted()),this,SLOT(settingsAccepted()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::settingsAccepted() {
  if (flagInputIPAddressChanged) {
    inputNetworkData->host = QHostAddress(newInputIPAddress);
    inputNetworkData->initializeInterface();
  }
  if (flagOutputIPAddressChanged) {
    outputNetworkData->host = QHostAddress(newOutputIPAddress);
    outputNetworkData->initializeInterface();
  }
}

void SettingsDialog::settingsRejected() {
  for (int i = 0; i < InputSettings.size(); i++) {
    if (InputSettings[i]->changed) {
      bControl->inputChannelChangedSlot(i,InputSettings[i]->IOpin);
      bControl->inputEdgeChangedSlot(i,InputSettings[i]->edge);
    }
  }
  for (int i = 0; i < OutputSettings.size(); i++) {
    if (OutputSettings[i]->changed) {
      bControl->outputChannelChangedSlot(i,OutputSettings[i]->IOpin);
      bControl->outputPulseLengthChangedSlot(i,OutputSettings[i]->pulseLength);
    }
  }

}
