#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "AudioFile.h"

#include <QAudioInput>
#include <QMutex>
#include <QThread>

class SampleGetter ;

class AudioStream : public AudioFile
{
      Q_OBJECT

   public:
      AudioStream(QString input, double baudRate );

      ~AudioStream() override ;

      int read_wav_header() override ;

      int readSignedSample() override ;

      // Thread safe addition of samples to the collection
      void pushBuffer( const QByteArray & bytes ) override ;


   protected :

      int getBufferElement() ;

   public:

      QAudioInput * audioInput_ = Q_NULLPTR ;


   protected:

      // List of buffers
      QList< QByteArray > buffers_ ;

      // Buffer being currently read
      QByteArray currentBuffer_ ;

      // Index in current buffer
      int valueIndex_ = 0 ;

      QThread audioInputThread_ ;
      QMutex listMutex_ ;

      SampleGetter * sampleGetter_ ;
};

#endif // AUDIOSTREAM_H
