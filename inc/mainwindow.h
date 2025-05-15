#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QListWidget>
#include <QCheckBox>
#include <QPixmapCache>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>
#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QSettings>

#include <malloc.h>
#include "inc/videolabel.h"

// Forward declaration
class UdpCmdSender;
class LogoDetector;

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        void setupTopLayout(QWidget *parent);
        void setupBottomLayout(QWidget *parent);
        void setupMemoryManagement();
        void applyDeltaStyle();

        QHBoxLayout* createXSliderLayout();
        QHBoxLayout* createYSliderLayout();
        QHBoxLayout* createStateButtonLayout();
        QHBoxLayout* createConnectionLayout();
        QHBoxLayout* createLogoLayout();
        QVBoxLayout* createLogoDetectionLayout();
        QHBoxLayout* createModeSelectionLayout();

        void initializeLogWidget();
        void initializeDesiredFramesViewWidget();
        void initializeButtons();
        void initializeXSlider(QSlider *slider);
        void initializeYSlider(QSlider *slider);
        void initializeLineEditX(QLineEdit *lineEdit);
        void initializeLineEditY(QLineEdit *lineEdit);
        void initializeLineEdit(QLineEdit *lineEdit);

        QPixmap createOutlinedTextPixmap(const QString &text, int fontSize, const QColor &textColor, 
            const QColor &outlineColor, qreal outlineWidth = 1.5);

        void connectSignalsAndSlots();
        void setupFrameViewerConnections();
        void saveSettings();
        void loadSettings();

        QWidget *topWidgetRef;

        QPlainTextEdit *log;
        QPushButton *activeButton;
        QPushButton *shotButton;
        QPushButton *logButton;
        QSlider *xSlider;
        QSlider *ySlider;
        QLineEdit *xLineEdit;
        QLineEdit *yLineEdit;
        QWidget *centralWidget;
        QLineEdit *addressLineEdit;
        QPushButton *connectButton;
        QListWidget *desiredFramesView;
        QCheckBox *logoDetectionEnabled;
        QPushButton *clearFramesButton;
        QComboBox *modeSelectBox;

        bool gstRunning;
        bool udpConnected;
        
        // UdpCmdSender pointer - not owned by MainWindow
        UdpCmdSender *cmdSender;
        LogoDetector *logoDetector;
        
        mutable QMutex log_mutex;
        mutable QMutex label_mutex;

        int frameHeight;
        int frameWidth;
        int frameIndex;
        
        // Current values
        int currentXValue;
        int currentYValue;
        bool currentShotValue;
        bool currentActiveValue;
        bool isFullScreen;
        bool isCleaningMemory;
        bool detection;

    public:
        VideoLabel *label;

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        
        // Set the UdpCmdSender from AppController
        void setUdpCmdSender(UdpCmdSender *sender);
        void setLogoDetector(LogoDetector *detector);

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
        void updateMode(int value);

        void startLogoDetection();
        void stopLogoDetection();

    public slots:
        void initMainWindow();
        void logMessage(const QString &message);
        void logError(const QString &errorMessage);

        void autoSaveFrame();

    private slots:
        void updateXLineEdit();
        void updateXSlider();
        void updateYLineEdit();
        void updateYSlider();
        void stateButtonClicked(bool checked);
        void shotButtonClicked();
        void connectButtonClicked();
        void showLog();
        void closeEvent(QCloseEvent *event);
        void logoDetectionToggled(bool checked);
        void clearFramesButtonClicked();
        void enterFullscreen();
        void exitFullscreen();
        void onFrameItemDoubleClicked(QListWidgetItem *item);
        

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MAINWINDOW_H