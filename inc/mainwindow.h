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
        void initializeSlider(QSlider *slider);
        void initializeLineEdit(QLineEdit *lineEdit);
        void connectSignalsAndSlots();

        QPlainTextEdit *log;
        QPushButton *activeButton;
        QSlider *xSlider;
        QSlider *ySlider;
        QLineEdit *xLineEdit;
        QLineEdit *yLineEdit;
        QWidget *centralWidget;
        QLineEdit *addressLineEdit;
        QPushButton *connectButton;

        bool gstRunning;
        
        mutable QMutex log_mutex;

    public:
        VideoLabel *label;

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    signals:
        void startGstProcess();
        void windowClosed();    

    public slots:
        void initMainWindow();

    private slots:
        void updateXLineEdit();
        void updateXSlider();
        void updateYLineEdit();
        void updateYSlider();
        void stateButtonClicked(bool checked);
        void connectButtonClicked();
        void closeEvent(QCloseEvent *event);
};