#include "AudioStream.h"

#include <QDebug>

AudioStream::AudioStream
(
   QString input,
   double bRate
)
: AudioFile( bRate )
{
   // Configure input
   QAudioFormat format;

   // TODO get this info from main
   sample_rate = 44100 ;
   samplesPerBit = sample_rate / baudRate;
   channels = 1 ;
   bits_sample = 16 ;
}


AudioStream::~AudioStream()
{
}


int
AudioStream::read_wav_header()
{
   return 0;
}


int
AudioStream::readSignedSample()
{
   // Read from collection of buffers
   // Block if no values yet
   int byte, i, sample = 0, s = 0; // EOF -> 0x1000000

   for (i = 0; i < channels; i++) {
      // i = 0: links bzw. mono
      byte = getBufferElement();

      if (i == targetedChannel)
         sample = byte;

      if (bits_sample == 16) {
         byte = getBufferElement();

         if (i == targetedChannel)
            sample += byte << 8;
      }

   }

   if (bits_sample == 8) s = sample - 128; // 8bit: 00..FF, centerpoint 0x80=128
   if (bits_sample == 16) s = (short) sample;

   return s;
}


void AudioStream::pushBuffer( const QByteArray & bytes )
{
   // Sometimes we are fed an empty buffer, skip it
   if ( bytes.size() == 0 )
   {
      return ;
   }
   // Add to collection of buffers
   listMutex_.lock() ;
   buffers_.push_back( bytes ) ;
   listMutex_.unlock() ;
}


int AudioStream::getBufferElement()
{
   // There is a buffer currently read
   if (valueIndex_ < currentBuffer_.size())
   {
       return currentBuffer_[valueIndex_++];
   }

   // Wait untill there are new full buffers
   listMutex_.lock() ;
   int nbBuffers = buffers_.size() ;
   listMutex_.unlock() ;

   while ( nbBuffers == 0 )
   {
      QThread::msleep( 10 ) ;
      listMutex_.lock() ;
      nbBuffers = buffers_.size() ;
      listMutex_.unlock() ;
   }

   listMutex_.lock() ;
   currentBuffer_ = buffers_.front();
   buffers_.pop_front();
   valueIndex_ = 0;
   listMutex_.unlock() ;

   return currentBuffer_[valueIndex_++];
}



