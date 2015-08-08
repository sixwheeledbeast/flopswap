#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QProcess>


class About;
class Help;
class Instructions;
class Settings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:

    // For Connecting Keyboard buttons
    void connectSignals();

    //Button Slots
    void on_pushButtonSwap_clicked();

    void on_pushButtonEject_clicked();

    void on_pushButtonEjectV_clicked();

    void on_pushButtonGet_clicked();

    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

    void on_actionSettings_triggered();

    void on_actionReadme_triggered();

    void initialshow();

    void initialseen();

    void initialpsb();

    void ejectshow();

    void ejectseen();

    void ejectclick();

    void disableStatus();

    void swappingActive();

    void disableButtons();

    void enableButtons();

    void updatestatus();

private:
    Ui::MainWindow *ui;
    bool portrait;
    bool ssc;
    About *about;
    Help *help;
    Instructions *instructions;
    Settings *settingspage;
    QProcess *m_process;
};

#endif // MAINWINDOW_H
