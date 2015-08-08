#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "help.h"
#include "instructions.h"
#include "settings.h"

#include <QProcess>
#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include <QShortcut>
#include <QFont>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QMaemo5InformationBox>
#include <QtMaemo5/QtMaemo5>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow),
      portrait(false),
      about(0),
      help(0),
      instructions(0),
      settingspage(0),
      m_process(new QProcess(this))

{
    ui->setupUi(this);

    qDebug("Set Window");
    //Set Window
    this->setWindowTitle("FlopSwap");
    this->setAttribute(Qt::WA_Maemo5StackedWindow);

    qDebug("Set Button Text");
    //Set Button Text
    ui->pushButtonSwap->setText(tr("Fresh Swap"));
    ui->pushButtonEject->setText(tr("Swap to Device"));
    ui->pushButtonEjectV->setText(tr("Swap to Device"));
    ui->pushButtonGet->setText(tr("Check"));

    qDebug("Set Button Icons");
    // Set Button Icons
    ui->pushButtonSwap->setIcon(QPixmap("/opt/flopswap/icons/refresh_icon&32.png"));
    ui->pushButtonEject->setIcon(QPixmap("/opt/flopswap/icons/eject_icon&32.png"));
    ui->pushButtonEjectV->setIcon(QPixmap("/opt/flopswap/icons/eject_icon&32.png"));
    ui->pushButtonGet->setIcon(QPixmap("/opt/flopswap/icons/info_icon&32.png"));

    qDebug("Setup Statusbar Font Size");
    //Setup Statusbar Font Size
    QFont f = ui->labelStat->font();
    f.setPointSize( f.pointSize() + 2 );
    QFont g = ui->labelLocStat->font();
    g.setPointSize( g.pointSize() + 2 );
    QFont t = ui->labelDT->font();
    t.setPointSize( t.pointSize() - 1 );

    qDebug("Set Font Sizes");
    //Set Font Sizes
    ui->labelStat->setFont(f);
    ui->labelLocStat->setFont(g);
    ui->labelDT->setFont(t);

    qDebug("Set Date Time Text");
    //Set Date Time Text
   if ( QFileInfo("/tmp/flopswapcur").exists() )
    {
       qDebug(">flopswapcur exists");
        QSettings settings("flopswap", "flopswap");
        ssc = settings.value("StartupCheck").toBool();
       if (ssc == false)
       {
           qDebug(">>startcheck is false");
           if ( QFileInfo("/tmp/flopswaploc").exists() )
           {
              qDebug(">>>If flopswaploc exists");
              QFile file("/tmp/flopswaploc");
              file.open(QIODevice::ReadOnly | QIODevice::Text);
              QTextStream in(&file);
              QString line = in.readLine();
              ui->labelLocStat->setText(line);
           }
           else
           {
              qDebug(">>>flopswaploc didn't exist");
              ui->labelLocStat->setText("Initialized...");
           }

           QFile file("/tmp/flopswapcur");
           file.open(QIODevice::ReadOnly | QIODevice::Text);
           QTextStream in(&file);
           QString line = in.readLine();
           ui->labelStat->setText(line);

           QSettings settings("flopswap", "flopswap");
           QString dts = settings.value("LastRefreshDateTime").toString();
           ui->labelDT->setText("Updated: " + dts);
       }
       else
       {
           qDebug(">>startcheck is true");
           ui->pushButtonGet->click();
       }
    }
   else
    {
       qDebug(">flopswapcur didn't exist");
       ui->labelDT->setText("Updated: Never");
    }

    qDebug("Set Action Text");
    //Set Action Text
    ui->actionSettings->setText(tr("Settings"));
    ui->actionReadme->setText(tr("Quick Guide"));
    ui->actionHelp->setText(tr("Help"));
    ui->actionAbout->setText(tr("About"));

    qDebug("Set Actions");
    //Set Actions
    about = new About(this);
    help = new Help(this);
    instructions = new Instructions(this);
    settingspage = new Settings(this);

    qDebug("Keyboard Buttons");
    //For Connecting Keyboard Buttons
    connectSignals();

    qDebug("Initial Text");
    //Initial Text - Show Once
    initialshow();

    qDebug("Initial QSettings");
    //Initial QSettings for Spin Box
    initialpsb();

    qDebug("Button Disable");
    //Check for existance of /tmp/flopswapint for button disable
    disableStatus();

    qDebug("Progress Indication");
    //Check for existance of /tmp/flopswapref for progress indication
    swappingActive();

    qDebug("Connect Slots");
    //GUI Blocking on Swap
    connect(m_process, SIGNAL(started()), this, SLOT(disableButtons()));
    connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(enableButtons()));

    qDebug("Construction Done!");

}

MainWindow::~MainWindow()
{
    initialseen();

    if (about) delete (about);
    if (help) delete (help);
    if (instructions) delete (instructions);
    if (settingspage) delete (settingspage);

    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

   //Keyboard Button Signals - remember to include QShortcut
void MainWindow::connectSignals()
{

    //Keyboard Key Signals
    connect(new QShortcut(QKeySequence(Qt::Key_Q), this), SIGNAL(activated()), this, SLOT(on_pushButtonSwap_clicked()));
    connect(new QShortcut(QKeySequence(Qt::Key_W), this), SIGNAL(activated()), this, SLOT(on_pushButtonEject_clicked()));
    connect(new QShortcut(QKeySequence(Qt::Key_A), this), SIGNAL(activated()), this, SLOT(on_pushButtonGet_clicked()));
}

//Portrait switch
void MainWindow::resizeEvent(QResizeEvent *event)
{
    portrait = event->size().width() <= 480;

    ui->pushButtonEject->setVisible(!portrait);
    ui->pushButtonEjectV->setVisible(portrait);
}

//Setup QSettings for Spin Box on First Run
void MainWindow::initialpsb()
{
     QSettings settings("flopswap", "flopswap");
     if (!settings.contains("PercentForDialog"))
     {
       settings.setValue("PercentForDialog", 94);
       settings.value("PercentForDialog").toInt();
     }
}

//Initial First Run Dialog Text
void MainWindow::initialshow()
{
   QSettings settings("flopswap", "flopswap");

   if (!settings.contains("InitialWarningShown"))
       {
       //Run swapswitch QProcess to draw pushbutton status correctly on first installation
       on_pushButtonGet_clicked();
       //Dialog
        QMaemo5InformationBox::information(
          0,
          " <br> \
            <br> \
          Thank you for installing FlopSwap. <br> \
          For this application to function it requires two linux-swap spaces creating on your uSD. <br> \
          If you have not already done this, please see the Wiki shortcut located in the Help Menu for instructions. <br> \
          <br> ",
          0);
       }
}

//Initial First Run Dialog Text Seen
void MainWindow::initialseen()
{
    //Set Dialog as seen
    QSettings settings("flopswap", "flopswap");

    if (!settings.contains("InitialWarningShown"))
        {
         settings.setValue("InitialWarningShown", true);
        }
}


//Eject First Run Dialog Text
void MainWindow::ejectshow()
{
   //Dialog
   QSettings settings("flopswap", "flopswap");

   if (!settings.contains("EjectWarningShown"))
       {
        QMaemo5InformationBox::information(
          0,
          " <br> \
            <br> \
          This is the Swap to Device Button<br> \
          This button will move your swap to the device to allow you to remove the back cover and/or uSD card. <br> \
          CAUTION:- While swap is on the uSD DO NOT remove the back cover. <br> \
          This will cause the device to reboot and may cause data loss or damage. <br> \
          Please wait for the Safe to Remove Back Cover message. <br> \
            <br> ",
          0);
       }
}

//Eject First Run Dialog Seen
void MainWindow::ejectseen()
{
    QSettings settings("flopswap", "flopswap");

    if (!settings.contains("EjectWarningShown"))
        {
         settings.setValue("EjectWarningShown", true);
        }
}

//Check for existance of /tmp/flopswapint to disable Buttons
void MainWindow::disableStatus()
{
   if ( QFileInfo("/tmp/flopswapint").exists() )
   {
        ui->pushButtonEject->setDisabled(true);
        ui->pushButtonEjectV->setDisabled(true);
        ui->pushButtonGet->setDisabled(true);
        ui->labelStat->setText("Not Available");
        ui->labelLocStat->setText("Safe to Remove Back Cover");
   }
   else
   {
        ui->pushButtonEject->setDisabled(false);
        ui->pushButtonEjectV->setDisabled(false);
        ui->pushButtonGet->setDisabled(false);
   }
}

//Check for existance of /tmp/flopswapref on Initial to disable Buttons
void MainWindow::swappingActive()
{
    if ( QFileInfo("/tmp/flopswapref").exists() )
    {
        disableButtons();
        //Loop this again in 5 seconds
        QTimer::singleShot(5000, this, SLOT(swappingActive()));
    }
    else
    {
        enableButtons();
    }
}

//Use to disable Buttons on button press
void MainWindow::disableButtons()
{
    this->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, true);
    ui->pushButtonSwap->setDisabled(true);
    ui->pushButtonEject->setDisabled(true);
    ui->pushButtonEjectV->setDisabled(true);
    ui->pushButtonGet->setDisabled(true);
}

//Use to enable Buttons on QProcess complete
void MainWindow::enableButtons()
{
    this->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, false);
    ui->pushButtonSwap->setDisabled(false);
    ui->pushButtonEject->setDisabled(false);
    ui->pushButtonEjectV->setDisabled(false);
    ui->pushButtonGet->setDisabled(false);
    disableStatus();
}

void MainWindow::ejectclick()
{
    QMessageBox msgBox;
     msgBox.setText("Continue?");
     msgBox.setWindowTitle("Move Swap to Device");
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
     msgBox.setDefaultButton(QMessageBox::Yes);
     int ret = msgBox.exec();
     switch (ret) {
       case QMessageBox::Yes:
             ejectshow();
             m_process->start("sudo /opt/flopswap/script/swapswitchejectsd.sh");
             ejectseen();
           break;
       case QMessageBox::No:
           // Cancel was clicked
           break;
       default:
           // should never be reached
           break;
     }
}

void MainWindow::updatestatus()
{
    if ( QFileInfo("/tmp/flopswaploc").exists() )
    {
       QFile file("/tmp/flopswaploc");
       file.open(QIODevice::ReadOnly | QIODevice::Text);
       QTextStream in(&file);
       QString line = in.readLine();
       ui->labelLocStat->setText(line);
    }
    else
    {
       ui->labelLocStat->setText("Initialized...");
    }

     if ( QFileInfo("/tmp/flopswapcur").exists() )
     {
        QFile file("/tmp/flopswapcur");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QString line = in.readLine();
        ui->labelStat->setText(line);
     }
     else
     {
        ui->labelStat->setText("Initialized...");
     }

    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString();
    ui->labelDT->setText("Updated: " + dateTimeString);
    QSettings settings("flopswap", "flopswap");
    settings.setValue("LastRefreshDateTime", dateTimeString);
}

//Buttons
void MainWindow::on_pushButtonSwap_clicked()
{ 
    m_process->start("sudo /opt/flopswap/script/swapswitch.sh");

    m_process->waitForFinished();

    updatestatus();
}

void MainWindow::on_pushButtonEject_clicked()
{
    ejectclick();
}

void MainWindow::on_pushButtonEjectV_clicked()
{
    ejectclick();
}

void MainWindow::on_pushButtonGet_clicked()
{
    m_process->start("sudo /opt/flopswap/script/swapused.sh");

    m_process->waitForFinished();

    updatestatus();
}

//Actions
void MainWindow::on_actionAbout_triggered()
{
    about->show();
}

void MainWindow::on_actionHelp_triggered()
{
    help->show();
}

void MainWindow::on_actionSettings_triggered()
{
    settingspage->show();
}

void MainWindow::on_actionReadme_triggered()
{
    instructions->show();
}
