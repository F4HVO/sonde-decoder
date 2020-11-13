#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   // Forward starr decode command
   connect( ui->pushButtonStart, &QPushButton::clicked,
            this, &MainWindow::sigStartDecode ) ;



   QVariantMap parameters;
   QStringList args(QCoreApplication::arguments());

   // Fetch tokens from the environment, if present
   const QByteArray mapboxMapID = qgetenv("MAPBOX_MAP_ID");
   const QByteArray mapboxAccessToken = qgetenv("MAPBOX_ACCESS_TOKEN");
   const QByteArray hereAppID = qgetenv("HERE_APP_ID");
   const QByteArray hereToken = qgetenv("HERE_TOKEN");
   const QByteArray esriToken = qgetenv("ESRI_TOKEN");

   if (!mapboxMapID.isEmpty())
      parameters["mapbox.map_id"] = QString::fromLocal8Bit(mapboxMapID);
   if (!mapboxAccessToken.isEmpty()) {
      parameters["mapbox.access_token"] = QString::fromLocal8Bit(mapboxAccessToken);
      parameters["mapboxgl.access_token"] = QString::fromLocal8Bit(mapboxAccessToken);
   }
   if (!hereAppID.isEmpty())
      parameters["here.app_id"] = QString::fromLocal8Bit(hereAppID);
   if (!hereToken.isEmpty())
      parameters["here.token"] = QString::fromLocal8Bit(hereToken);
   if (!esriToken.isEmpty())
      parameters["esri.token"] = QString::fromLocal8Bit(esriToken);

   ui->quickWidget->setSource( QUrl( "./Map.qml" ) ) ;
   ui->quickWidget->show() ;
   QObject *item = ui->quickWidget->rootObject() ;
   Q_ASSERT(item);
   QMetaObject::invokeMethod(item, "initializeProviders",
                             Q_ARG(QVariant, QVariant::fromValue(parameters)));

   ui->textBrowserOutput->setOpenExternalLinks(true);
}

MainWindow::~MainWindow()
{
   delete ui;
}


QString
MainWindow::getSelectedDevice()
{
   return ui->comboBoxAudioInput->currentText() ;
}


bool
MainWindow::isSign() const
{
   return ui->checkBoxSign->isChecked() ;
}


bool
MainWindow::isRepair() const
{
   return ui->checkBoxRepair->isChecked() ;
}


bool
MainWindow::isStat() const
{
   return ui->checkBoxStat->isChecked() ;
}

bool
MainWindow::isRaw() const
{
   return ui->checkBoxRaw->isChecked() ;
}

bool
MainWindow::isVerbose() const
{
   return ui->checkBoxVerbose->isChecked() ;
}



void
MainWindow::slotAddMessage
(
   const QString & newMessage
)
{
   QTextCursor cursor( ui->textBrowserOutput->document() ) ;
   cursor.insertHtml( QString( "%1<br>" ).arg( newMessage ) ) ;

   // Extract coordinates if there are some, then display them
   QRegularExpression coordinatesExp( ".*>(?<latitude>[0-9.]*),(?<longitude>[0-9.]*)<" ) ;
   auto match = coordinatesExp.match( newMessage ) ;
   if ( match.hasMatch() )
   {
      QVariantMap parameters;
      parameters["latitude"] = match.captured( "latitude" ).toDouble() ;
      parameters["longitude"] = match.captured( "longitude" ).toDouble() ;

      // Invoke addMarker method
      QObject *item = ui->quickWidget->rootObject() ;
      Q_ASSERT(item);
      QMetaObject::invokeMethod(item, "addMarkerAtCoordinates",
                                Q_ARG(QVariant, QVariant::fromValue(parameters)));
   }
}


void
MainWindow::slotEndpoints
(
   const QStringList & endPoints
)
{
   for ( auto & endPoint : endPoints )
   {
      ui->comboBoxAudioInput->addItem( endPoint ) ;
   }
}


void
MainWindow::slotStarted()
{
   ui->pushButtonStart->setEnabled( false ) ;
   ui->pushButtonStart->setText( "Stop" ) ;
   ui->comboBoxAudioInput->setEnabled( false ) ;
   ui->checkBoxRepair->setEnabled( false ) ;
   ui->checkBoxSign->setEnabled( false ) ;
   ui->checkBoxStat->setEnabled( false ) ;
   ui->labelMethod->setEnabled( false ) ;
   ui->checkBoxRaw->setEnabled( false ) ;
   ui->checkBoxVerbose->setEnabled( false ) ;
}

void
MainWindow::slotAudioStateChanged
(
   QAudio::State state
)
{
   if ( state != QAudio::ActiveState )
   {
      slotAddMessage( QString( "<span style=\"color:red;\">Audio stream interrupted ! State is %1.</span>" )
                      .arg( state ) ) ;
   }
}

