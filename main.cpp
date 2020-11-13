#include "MainWindow.h"
#include "M10Decoder.h"
#include "AudioFile.h"

#include <QApplication>
#include <QAudioFormat>
#include <QAudioInput>
#include <QBuffer>
#include <QDebug>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();

   M10Decoder decoder ;

   // Configure input
   QAudioFormat format;

   int sample_rate = 44100 ;
   int channels = 1 ;
   int bits_sample = 16 ;

   // Set up the desired format
   format.setSampleRate(sample_rate);
   format.setChannelCount(channels);
   format.setSampleSize(bits_sample);
   format.setCodec("audio/pcm");
   format.setByteOrder(QAudioFormat::LittleEndian);
   format.setSampleType(QAudioFormat::SignedInt);

   // Enumerate audio endpoints
   QStringList endPoints ;
   auto deviceInfos = QAudioDeviceInfo::availableDevices(QAudio::AudioInput) ;
   foreach(const QAudioDeviceInfo &deviceInfo, deviceInfos )
   {
      endPoints.append( deviceInfo.deviceName() ) ;
   }
   w.slotEndpoints( endPoints ) ;

   // Call the decoding function into a thread
   QThread decodeThread ;

   // Connect message signal to view add message slot
   QObject::connect( &decoder, &M10Decoder::sigFrame,
                     &w, &MainWindow::slotAddMessage ) ;

   // Assign decoder to its dedicated thread
   decoder.moveToThread( & decodeThread ) ;

   // When thread starts, it executes slotStartDecoding
   QObject::connect( &decodeThread, &QThread::started,
                     &decoder, &M10Decoder::slotStartDecoding ) ;
   QAudioInput * audioInput = Q_NULLPTR ;

   // When button start is clicked, start thread
   QObject::connect( &w, &MainWindow::sigStartDecode,
                     [&decoder, &audioInput, &decodeThread, format, &w, &deviceInfos]( bool )
   {
      // Use selected device
      auto stringDevice = w.getSelectedDevice() ;
      QAudioDeviceInfo * dInfo = Q_NULLPTR ;
      for ( auto & deviceInfo : deviceInfos )
      {
         if ( deviceInfo.deviceName() == stringDevice )
         {
            dInfo = &deviceInfo ;
         }
      }

      // Internal consistency error, pointer cannot be null
      if ( dInfo == Q_NULLPTR )
      {
         w.slotAddMessage( "<span style=\"color:red;\">Audio device has disapeared between the moment the application was started and the decoder was started.</span>" ) ;
         return ;
      }

      // Inform user in case the audio endpoint does not support the requested audio format
      if (!dInfo->isFormatSupported(format)) {
         w.slotAddMessage( "<span style=\"color:red;\">Audio device does not support recording format.</span>" ) ;
         return ;
      }

      // Forward GUI options to the decoder
      if ( w.isSign() )
      {
         decoder.setTryMethodSign(true) ;
      }
      if ( w.isRepair() )
      {
         decoder.setTryMethodRepair(true) ;
      }
      if ( w.isStat() )
      {
         decoder.setTryStats(true) ;
      }
      if ( w.isRaw() )
      {
         decoder.setRaw(true) ;
      }
      if ( w.isVerbose() )
      {
         decoder.setVerboseLevel(1) ;
      }

      // Start audio input
      audioInput = new QAudioInput( *dInfo, format ) ;
      audioInput->setNotifyInterval( 50 ) ;
      auto ioDevice = new QBuffer() ;
      ioDevice->open( QBuffer::ReadWrite ) ;
      audioInput->start( ioDevice ) ;

      // Push received audio samples to the decoder
      QObject::connect( audioInput, &QAudioInput::notify,
                        [&decoder, ioDevice](){ if ( decoder.audioFile ) { decoder.audioFile->pushBuffer( ioDevice->buffer() ) ; ioDevice->seek(0) ; ioDevice->buffer().clear() ; } } ) ;

      // Inform the main window of audio input state transitions
      QObject::connect( audioInput, &QAudioInput::stateChanged,
                        &w, &MainWindow::slotAudioStateChanged ) ;

      // Start decoder thread
      decodeThread.start() ;

      // Check audio input state
      if ( audioInput->state() == QAudio::ActiveState && audioInput->error() == QAudio::NoError )
      {
         w.slotAddMessage( "<span style=\"color:green;\">Audio stream has started.</span>" ) ;
      }
      else
      {
         w.slotAddMessage(
                  QString( "<span style=\"color:red;\">Cannot start audio stream: state %1, error %2.</span>" )
                  .arg( audioInput->state() )
                  .arg( audioInput->error() ) ) ;
      }

      // Visual feedback that we have started
      w.slotStarted() ;
   } ) ;

   return a.exec();
}
