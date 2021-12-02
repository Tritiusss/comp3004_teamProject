#ifndef RECORD_H
#define RECORD_H
#include <QString>


class Record
{
public:
    Record(QString wave, int freq, int startT, int dur, int power);
private:
    QString waveform;
    int frequency;
    int startTime;
    int duration;
    int powerLevel;
};

#endif // RECORD_H
