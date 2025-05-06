#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLineEdit>
#include <QIntValidator>

#include "inc/videolabel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        void setupTopLayout();
        void setupBottomLayout();

        QHBoxLayout* createXSliderLayout();
        QHBoxLayout* createYSliderLayout();
        void initializeSlider(QSlider *slider);
        void initializeLineEdit(QLineEdit *lineEdit);
        void connectSignalsAndSlots();

        QSlider *xSlider;
        QSlider *ySlider;
        QLineEdit *xLineEdit;
        QLineEdit *yLineEdit;
        QWidget *centralWidget;

    public:
        void initMainWindow();

        VideoLabel *label;

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void updateXLineEdit();
        void updateXSlider();
        void updateYLineEdit();
        void updateYSlider();
};

