/*
 * File:   AudioFile.h
 * Author: Viproz
 * Used code from rs1729
 * Created on January 6, 2019, 10:23 AM
 */

#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#define EOF_INT  0x1000000
#define AVG_NUM 5.

#include <stdio.h>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

#include <QObject>

class AudioFile : public QObject {
      Q_OBJECT

public:
    AudioFile(double baudRate);
    AudioFile(std::string filename, double baudRate, int* errors);
    virtual ~AudioFile();

    virtual int read_wav_header();

    virtual int readSignedSample();

    virtual void pushBuffer( const QByteArray & ) {}

    int readSignedSampleAveraged();
    int readSignedSampleNormalized();
    int readSignedSampleAveragedNormalized();

    int averageSample(int sample);
    int normalizeSample(int sample);
    int averageNormalizeSample(int sample);

    void resetActiveSum();

    // Getters
    int getSampleRate() { return sample_rate; }
    double getSamplesPerBit() { return samplesPerBit; }

    // Setters
    void setTargetedChannel(int channel) { targetedChannel = channel; }
    void setBaudRate(int baudv) { baudRate = baudv; }

protected:

    int findstr(char* buf, const char* str, int pos);

    double activeSum = 0;
    int targetedChannel = 0;
    int sample_rate = 0;
    int bits_sample = 0;
    int channels = 0;
    double baudRate;
    double samplesPerBit = 0;

    FILE *fp = Q_NULLPTR ;
};

#endif /* AUDIOFILE_H */

