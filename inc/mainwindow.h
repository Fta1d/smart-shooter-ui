#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>

#include "inc/videolabel.h"

// Forward declaration
class UdpCmdSender;

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        void setupTopLayout(QWidget *parent);
        void setupBottomLayout(QWidget *parent);

        QHBoxLayout* createXSliderLayout();
        QHBoxLayout* createYSliderLayout();
        QHBoxLayout* createStateButtonLayout();
        QHBoxLayout* createConnectionLayout();
        void initializeLogWidget();
        void initializeButtons();
        void initializeXSlider(QSlider *slider);
        void initializeYSlider(QSlider *slider);
        void initializeLineEdit(QLineEdit *lineEdit);
        void connectSignalsAndSlots();

        // QWidget *topWidget = nullptr;
        // QHBoxLayout *topLayout = nullptr;

        QPlainTextEdit *log;
        QPushButton *activeButton;
        QPushButton *shotButton;
        QSlider *xSlider;
        QSlider *ySlider;
        QLineEdit *xLineEdit;
        QLineEdit *yLineEdit;
        QWidget *centralWidget;
        QLineEdit *addressLineEdit;
        QPushButton *connectButton;

        bool gstRunning;
        bool udpConnected;
        
        // UdpCmdSender pointer - not owned by MainWindow
        UdpCmdSender *cmdSender;
        
        mutable QMutex log_mutex;
        
        // Current values
        int currentXValue;
        int currentYValue;
        bool currentShotValue;
        bool currentActiveValue;
        bool isFullScreen;

    public:
        VideoLabel *label;

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        
        // Set the UdpCmdSender from AppController
        void setUdpCmdSender(UdpCmdSender *sender);

    signals:
        void startGstProcess();
        void windowClosed();    
        
        // New signals for UdpCmdSender
        void startUdpSending(const QString &address, quint16 port);
        void stopUdpSending();
        void updateXValue(int value);
        void updateYValue(int value);
        void updateShotValue(bool value);
        void updateActiveValue(bool value);

    public slots:
        void initMainWindow();

    private slots:
        void updateXLineEdit();
        void updateXSlider();
        void updateYLineEdit();
        void updateYSlider();
        void stateButtonClicked(bool checked);
        void shotButtonClicked();
        void connectButtonClicked();
        void closeEvent(QCloseEvent *event);
        //void fullscreenMode();
        
        // New slots for command sender
        void logMessage(const QString &message);
        void logError(const QString &errorMessage);

    protected:
        void keyPressEvent(QKeyEvent *event) override;
        bool eventFilter(QObject *watched, QEvent *event) override;
};