#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_powerButton_clicked();
    void changeFrequency();
    void changeWaveform();
    void changeTime();
    void increaseCurrent();
    void decreaseCurrent();

private:
    Ui::MainWindow *ui;

    bool powerStatus;
    QString currentFrequency;
    QString currentWaveform;
    QString timeCountDown;
    QGraphicsScene *scene;
    int currentTime;
    int current;

    void changePowerStatus();
};
#endif // MAINWINDOW_H
