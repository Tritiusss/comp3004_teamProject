#include "record.h"

Record::Record(QString wave, int freq, int startT, int dur, int power)
{
waveform = wave;
frequency = freq;
startTime = startT;
duration = dur;
powerLevel = power;
}
