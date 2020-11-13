#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QAudio>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
      Q_OBJECT

   public:

      MainWindow(QWidget *parent = nullptr);
      ~MainWindow();

      QString getSelectedDevice() ;


   public :

      bool isSign() const ;
      bool isRepair() const ;
      bool isStat() const ;
      bool isRaw() const ;
      bool isVerbose() const ;


   signals :

      void sigStartDecode( bool ) ;


   public slots :

      void slotAddMessage( const QString & newMessage ) ;

      void slotEndpoints( const QStringList & endPoints ) ;

      void slotStarted() ;

      void slotAudioStateChanged( QAudio::State state ) ;


   private:

      Ui::MainWindow *ui;

      // Call the decoding function into a thread
      QThread decodeThread ;

};
#endif // MAINWINDOW_H
