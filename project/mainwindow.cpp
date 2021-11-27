#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->displayScreen->setScene(scene);

    powerStatus = false;
    changePowerStatus();



    //connect(ui->powerButton, SIGNAL(released()), this, SLOT(on_powerButton_clicked()));
    connect(ui->frequencyButton, SIGNAL(released()), this, SLOT(changeFrequency()));
    connect(ui->waveformButton, SIGNAL(released()), this, SLOT(changeWaveform()));
    connect(ui->timeButton, SIGNAL(released()), this, SLOT(changeTime()));
    connect(ui->increaseCurrentButton, SIGNAL(released()), this, SLOT(increaseCurrent()));
    connect(ui->decreaseCurrentButton, SIGNAL(released()), this, SLOT(decreaseCurrent()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_powerButton_clicked()
{
    powerStatus = !powerStatus;
    qInfo("1");
    changePowerStatus();

}

void MainWindow::changePowerStatus() {
    ui->displayScreen->setVisible(powerStatus);
    ui->frequencyLabel->setVisible(powerStatus);
    ui->waveformLabel->setVisible(powerStatus);

    ui->frequencyButton->setEnabled(powerStatus);
    ui->waveformButton->setEnabled(powerStatus);
    ui->timeButton->setEnabled(powerStatus);
    ui->increaseCurrentButton->setEnabled(powerStatus);
    ui->decreaseCurrentButton->setEnabled(powerStatus);



    timeCountDown = QString::number(currentTime) + "0:00";
    scene->addText(timeCountDown);



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
    }
    qInfo("2");

}

void MainWindow::changeFrequency() {
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


}

void MainWindow::increaseCurrent(){
    if(current == 0){
        current += 1;
        ui->current_1->setVisible(true);
        ui->current_2->setVisible(true);
    }else if(current == 1){
        current += 1;
        ui->current_3->setVisible(true);
        ui->current_4->setVisible(true);
    }else if(current == 2){
        current += 1;
        ui->current_5->setVisible(true);
        ui->current_6->setVisible(true);
    }else if(current == 3){
        current += 1;
        ui->current_7->setVisible(true);
        ui->current_8->setVisible(true);
    }else if(current == 4){
        current += 1;
        ui->current_9->setVisible(true);
        ui->current_10->setVisible(true);
    }
}

void MainWindow::decreaseCurrent(){
    if(current == 5){
        current -= 1;
        ui->current_10->setVisible(false);
        ui->current_9->setVisible(false);
    }else if(current == 4){
        current -= 1;
        ui->current_8->setVisible(false);
        ui->current_7->setVisible(false);
    }else if(current == 3){
        current -= 1;
        ui->current_6->setVisible(false);
        ui->current_5->setVisible(false);
    }else if(current == 2){
        current -= 1;
        ui->current_4->setVisible(false);
        ui->current_3->setVisible(false);
    }else if(current == 1){
        current -= 1;
        ui->current_2->setVisible(false);
        ui->current_1->setVisible(false);
    }else if(current > 5){
            current -= 1;
    }
}
