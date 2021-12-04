#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    batteryTimer = new QTimer;
    currentBattery = 100.00;

    scene = new QGraphicsScene(this);
    ui->displayScreen->setScene(scene);
    powerStatus = false;
    ui->offLabel->setVisible(true);
    ui->onLabel->setVisible(false);
    treatment = false;
    skinContact = false;
    changePowerStatus();

    numRecords = 0;
    QStringList columnHeaders = {"Waveform", "Frequency", "Power Level","Start Time", "Duration"};
    records = new QStandardItemModel;
    records->setColumnCount(5);
    records->setHorizontalHeaderLabels(columnHeaders);
    ui->RecordView->setModel(records);
    recordsLoaded = false;
    ui->RecordView->setVisible(false);

    connect(ui->frequencyButton, SIGNAL(released()), this, SLOT(changeFrequency()));
    connect(ui->waveformButton, SIGNAL(released()), this, SLOT(changeWaveform()));
    connect(ui->timeButton, SIGNAL(released()), this, SLOT(changeTime()));
    connect(ui->increaseCurrentButton, SIGNAL(released()), this, SLOT(increaseCurrent()));
    connect(ui->decreaseCurrentButton, SIGNAL(released()), this, SLOT(decreaseCurrent()));
    connect(ui->recordButton, SIGNAL(released()), this, SLOT(addRecording()));
    connect(batteryTimer, SIGNAL(timeout()), this, SLOT(autoChangeBattery()));
    connect(ui->chargeButton, SIGNAL(released()), this, SLOT(chargeBattery()));
    connect(ui->batteryLineEdit, SIGNAL(returnPressed()), this, SLOT(changeBattery()));
    connect(ui->SaveRecordingButton, SIGNAL(released()), this, SLOT(addRecording()));
    connect(ui->LoadRecordingButton, SIGNAL(released()), this, SLOT(loadRecording()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_powerButton_clicked()
{
    powerStatus = !powerStatus;
    if (powerStatus) {
        autoTimer = new QTimer(this);
        countdownTimer = new QTimer(this);
        circuitTimer = new QTimer(this);
        changeTime();
        currentTimer = new QTimer(this);
        currentTimer->setSingleShot(true);
        connect(currentTimer, &QTimer::timeout, this, &MainWindow::checkCurrent);
        currentTimer->start(1000);
        if (skinContact) startTreatment();
    }
    else {
        delete autoTimer;
        delete countdownTimer;
        delete circuitTimer;

    }
    changePowerStatus();
}

void MainWindow::changePowerStatus() {
    ui->displayScreen->setVisible(powerStatus);
    ui->frequencyLabel->setVisible(powerStatus);
    ui->waveformLabel->setVisible(powerStatus);
    ui->batteryBar->setVisible(powerStatus);

    ui->frequencyButton->setEnabled(powerStatus);
    ui->waveformButton->setEnabled(powerStatus);
    ui->timeButton->setEnabled(powerStatus);
    ui->increaseCurrentButton->setEnabled(powerStatus);
    ui->decreaseCurrentButton->setEnabled(powerStatus);

    ui->batteryBar->setValue(currentBattery);
    ui->batteryLineEdit->setText(QString::number(currentBattery));

    if(powerStatus){
        batteryTimer->start(TIMEOUT);
    }

    timeCountDown = QString::number(currentTime) + ":00";

    if(!powerStatus){
        ui->frequencyButton->setEnabled(powerStatus);
        ui->waveformButton->setEnabled(powerStatus);
        ui->timeButton->setEnabled(powerStatus);
        ui->increaseCurrentButton->setEnabled(powerStatus);
        ui->decreaseCurrentButton->setEnabled(powerStatus);

        currentFrequency = "0.5Hz";
        ui->frequencyLabel->setText(currentFrequency);

        currentWaveform = "Alpha";
        ui->waveformLabel->setText(currentWaveform);

        //Time
        currentTime = 0;
        ui->timeLabel_20min->setVisible(powerStatus);
        ui->timeLabel_40min->setVisible(powerStatus);
        ui->timeLabel_60min->setVisible(powerStatus);

        current = 0;
        //Current Status
        ui->current_1->setVisible(powerStatus);
        ui->current_2->setVisible(powerStatus);
        ui->current_3->setVisible(powerStatus);
        ui->current_4->setVisible(powerStatus);
        ui->current_5->setVisible(powerStatus);
        ui->current_6->setVisible(powerStatus);
        ui->current_7->setVisible(powerStatus);
        ui->current_8->setVisible(powerStatus);
        ui->current_9->setVisible(powerStatus);
        ui->current_10->setVisible(powerStatus);

        //time countdown
        ui->displayScreen->scene()->clear();

        //battery stop reduce
        batteryTimer->stop();

        ui->batteryBar->setVisible(powerStatus);
        ui->lowBatteryWarning_5P->setVisible(powerStatus);
        ui->lowBatteryWarning_5P->setText("");
        ui->lowBatteryWarning_2P->setVisible(powerStatus);
        ui->lowBatteryWarning_2P->setText("");
    }

}

void MainWindow::changeFrequency() {
    restartAutoTimer();
    if(currentFrequency == "0.5Hz"){
        currentFrequency = "77Hz";
    }else if(currentFrequency == "77Hz"){
        currentFrequency = "100Hz";
    }else if(currentFrequency == "100Hz"){
        currentFrequency = "0.5Hz";
    }
    ui->frequencyLabel->setText(currentFrequency);
}

void MainWindow::changeWaveform() {
    restartAutoTimer();
    if(currentWaveform == "Alpha"){
        currentWaveform = "Betta";
    }else if(currentWaveform == "Betta"){
        currentWaveform = "Gamma";
    }else if(currentWaveform == "Gamma"){
        currentWaveform = "Alpha";
    }
    ui->waveformLabel->setText(currentWaveform);
}

void MainWindow::changeTime() {
    restartAutoTimer();
    ui->countdownLabel->setText("");
    if(currentTime == 20){
        currentTime = 40;
        ui->timeLabel_40min->setVisible(true);
        ui->timeLabel_20min->setVisible(false);
        ui->timeLabel_40min->setText(QString::number(currentTime));
    }else if(currentTime == 40){
        currentTime = 60;
        ui->timeLabel_60min->setVisible(true);
        ui->timeLabel_40min->setVisible(false);
        ui->timeLabel_60min->setText(QString::number(currentTime));
    }else if(currentTime == 60){
        currentTime = 20;
        ui->timeLabel_20min->setVisible(true);
        ui->timeLabel_60min->setVisible(false);
        ui->timeLabel_20min->setText(QString::number(currentTime));
    }else{
        currentTime = 20;
        ui->timeLabel_20min->setVisible(true);
        ui->timeLabel_20min->setText(QString::number(currentTime));
    }

    ui->displayScreen->scene()->clear();
    timeCountDown = QString::number(currentTime) + ":00";
    scene->addText(timeCountDown);
    if (skinContact) toggleSkinContact(true);
}

void MainWindow::toggleSkinContact(bool s) {
    skinContact = s;
    tempCountDown = timeCountDown; // reset

    if (skinContact && powerStatus) {
        circuitTimer->stop();
        circuitTimer = new QTimer(this);
        ui->offLabel->setVisible(false);
        ui->onLabel->setVisible(true);
        startTreatment();
        treatment = true;
    }
    else if (!skinContact && powerStatus) {
        ui->offLabel->setVisible(true);
        ui->onLabel->setVisible(false);
        delete circuitTimer;
        circuitTimer = new QTimer(this);
        circuitTimer->setSingleShot(true);
        connect(circuitTimer, &QTimer::timeout, this, &MainWindow::toggleTreatment);
        circuitTimer->start(5000);
        countdownTimer->stop();
    }
}

void MainWindow::increaseCurrent(){
    if(current == 0){
        current += 2;
        ui->current_1->setVisible(true);
        ui->current_2->setVisible(true);
    }else if(current == 2){
        current += 2;
        ui->current_3->setVisible(true);
        ui->current_4->setVisible(true);
    }else if(current == 4){
        current += 2;
        ui->current_5->setVisible(true);
        ui->current_6->setVisible(true);
    }else if(current == 6){
        current += 2;
        ui->current_7->setVisible(true);
        ui->current_8->setVisible(true);
    }else if(current == 8){
        current += 2;
        ui->current_9->setVisible(true);
        ui->current_10->setVisible(true);
    }
}

void MainWindow::decreaseCurrent(){
    if(current == 10){
        current -= 1;
        ui->current_10->setVisible(false);
    }else if(current == 9){
        current -= 1;
        ui->current_9->setVisible(false);
    }else if(current == 8){
        current -= 1;
        ui->current_8->setVisible(false);
    }else if(current == 7){
        current -= 1;
         ui->current_7->setVisible(false);
    }else if(current == 6){
        current -= 1;
        ui->current_6->setVisible(false);
    }else if(current == 5){
        current -= 1;
        ui->current_5->setVisible(false);
    }else if(current == 4){
        current -= 1;
        ui->current_4->setVisible(false);
    }else if(current == 3){
        current -= 1;
        ui->current_3->setVisible(false);
    }else if(current == 2){
        current -= 1;
        ui->current_2->setVisible(false);
    }else if(current == 1){
        current -= 1;
        ui->current_1->setVisible(false);
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1) {
        toggleSkinContact(true);
    }
    else {
        toggleSkinContact(false);
    }
}

void MainWindow::toggleTreatment() {
    treatment = !treatment;
    if (!treatment) {
        ui->countdownLabel->setText("");
        ui->displayScreen->scene()->clear();
        ui->displayScreen->scene()->addText("Treatment Stopped");
        countdownTimer->stop();
    }
    tempCountDown = timeCountDown;
    timeCountDown = QString::number(currentTime) + ":00";
}

void MainWindow::startTreatment() {
    delete countdownTimer;
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    countdownTimer->start(1000);
}

void MainWindow::updateTimer() {
    ui->displayScreen->scene()->clear();
    int m1 = timeCountDown[0].digitValue();
    int m2 = timeCountDown[1].digitValue();
    int s1 = timeCountDown[3].digitValue();
    int s2 = timeCountDown[4].digitValue();

    s2 -= 1;
    if (s2 == -1) {
        s1 -= 1;
        s2 = 9;
    }
    if (s1 == -1) {
        m2 -= 1;
        s1 = 5;
    }
    if (m2 == -1) {
        m1 -= 1;
        m2 = 9;
    }
    timeCountDown = QString::number(m1) + QString::number(m2) + ":" + QString::number(s1) + QString::number(s2);
    if (m1 == -1) {
        timeCountDown = "00:00";
        countdownTimer->stop();
        treatment = false;
        restartAutoTimer();
    }
    ui->countdownLabel->setText(timeCountDown);
}
void MainWindow::restartAutoTimer() {
    delete autoTimer;
    autoTimer = new QTimer(this);
    autoTimer->setSingleShot(true);
    connect(autoTimer, &QTimer::timeout, this, &MainWindow::shutdownCheck);
    autoTimer->start(1800000);
}

void MainWindow::shutdownCheck() {
    if (!skinContact && !treatment) { on_powerButton_clicked(); }
    else { restartAutoTimer(); }
}
void MainWindow::autoChangeBattery() {
    if(currentBattery > 2.0){
        currentBattery -= 0.05;

    }else{
        powerStatus = !powerStatus;
        changePowerStatus();
    }

    if(QString::number(currentBattery) == "2"){
        ui->lowBatteryWarning_2P->setVisible(powerStatus);
        ui->lowBatteryWarning_2P->setText("Warning: 2% battery remaining!!!");
    }else if(QString::number(currentBattery) == "5"){
        ui->lowBatteryWarning_5P->setVisible(powerStatus);
        ui->lowBatteryWarning_5P->setText("Warning: 5% battery remaining!!!");
    }else{
        ui->lowBatteryWarning_2P->setVisible(!powerStatus);
        ui->lowBatteryWarning_5P->setVisible(!powerStatus);
    }

    //display battery
    ui->batteryBar->setValue(currentBattery);
    ui->batteryLineEdit->setText(QString::number(currentBattery));
}

void MainWindow::chargeBattery() {
    currentBattery = 100.0;
    ui->batteryBar->setValue(currentBattery);
    ui->batteryLineEdit->setText(QString::number(currentBattery));
}

void MainWindow::changeBattery() {
    QString text = ui->batteryLineEdit->text();
    currentBattery = text.toDouble();

    ui->batteryBar->setValue(currentBattery);
    ui->batteryLineEdit->setText(QString::number(currentBattery));
}

void MainWindow::loadRecording() {
    if (powerStatus == true) {
    ui->RecordView->setVisible(!recordsLoaded);
    recordsLoaded = !recordsLoaded;
    }
}

void MainWindow::addRecording() {

    if (powerStatus == true) {
    QStandardItem *wave = new QStandardItem(QString(currentWaveform));
    records->setItem(numRecords,0, wave);

    QStandardItem *freq = new QStandardItem(QString(currentFrequency));
    records->setItem(numRecords,1, freq);

    QStandardItem *curr = new QStandardItem(QString::number(current*50));
    records->setItem(numRecords,2, curr);

    QStandardItem *start = new QStandardItem(QString::number(currentTime));
    records->setItem(numRecords,3, start);

    int timeInSec = 0;
    QStringList splt;
    if (!treatment) splt = tempCountDown.split(QLatin1Char(':'));
    else splt = timeCountDown.split(QLatin1Char(':'));

    timeInSec += splt[0].toInt()*60;
    timeInSec += splt[1].toInt();
    int startTimeInSec = currentTime*60;
    int durationInSec = startTimeInSec - timeInSec;
    int seconds = durationInSec % 60;
    int minutes = durationInSec / 60;
    QString minuteString;
    QString secondString;

    if (minutes < 10) {
        minuteString = QString("0%1").arg(minutes);
    } else {
        minuteString = QString("%1").arg(minutes);
    }
    if (seconds < 10) {
        secondString = QString("0%1").arg(seconds);
    } else {
        secondString = QString("%1").arg(seconds);
    }

    QString finalDuration = QString("%1:%2").arg(minuteString, secondString);
    QStandardItem *duration = new QStandardItem(finalDuration);
    records->setItem(numRecords,4, duration);
    numRecords++;
    }
}

void MainWindow::checkCurrent() {
    if (current > 14) {
        powerStatus = false;
        changePowerStatus();
    }
}

void MainWindow::on_currentFaultButton_clicked() {
    // simulate a current over 700 amps
    current = 15;
}
