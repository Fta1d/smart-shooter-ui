#include "inc/mainwindow.h"

void MainWindow::initMainWindow() {
    centralWidget = new QWidget(this);

    setupTopLayout();


    connectSignalsAndSlots();
    setCentralWidget(centralWidget);
}

void MainWindow::setupTopLayout() {
    QHBoxLayout *topLayout = new QHBoxLayout(centralWidget);

    topLayout->addWidget(label->vidStreamLabel);
    
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    
    slidersLayout->addLayout(createXSliderLayout());
    slidersLayout->addLayout(createYSliderLayout());
    
    slidersLayout->addStretch();
    
    topLayout->addLayout(slidersLayout);

}

void MainWindow::setupBottomLayout() {
    
}

QHBoxLayout* MainWindow::createXSliderLayout() {
    QHBoxLayout *xLayout = new QHBoxLayout();
    
    xSlider = new QSlider(Qt::Horizontal);
    initializeSlider(xSlider);
    
    xLineEdit = new QLineEdit();
    initializeLineEdit(xLineEdit);
    
    QLabel *xLabel = new QLabel("X:");
    
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xSlider);
    xLayout->addWidget(xLineEdit);
    
    return xLayout;
}

QHBoxLayout* MainWindow::createYSliderLayout() {
    QHBoxLayout *yLayout = new QHBoxLayout();

    ySlider = new QSlider(Qt::Horizontal);
    initializeSlider(ySlider);

    yLineEdit = new QLineEdit();
    initializeLineEdit(yLineEdit);

    QLabel *yLabel = new QLabel("Y:");

    yLayout->addWidget(yLabel);
    yLayout->addWidget(ySlider);
    yLayout->addWidget(yLineEdit);
    
    return yLayout;
}

void MainWindow::initializeSlider(QSlider *slider) {
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(0); 
}

void MainWindow::initializeLineEdit(QLineEdit *lineEdit) {
    lineEdit->setValidator(new QIntValidator(0, 100, this));
    lineEdit->setMaximumWidth(50);
    lineEdit->setText("0"); 
}

void MainWindow::connectSignalsAndSlots() {
    connect(xSlider, &QSlider::valueChanged, this, &MainWindow::updateXLineEdit);
    connect(xLineEdit, &QLineEdit::editingFinished, this, &MainWindow::updateXSlider);
    connect(ySlider, &QSlider::valueChanged, this, &MainWindow::updateYLineEdit);
    connect(yLineEdit, &QLineEdit::editingFinished, this, &MainWindow::updateYSlider);
}

void MainWindow::updateXLineEdit() {
    xLineEdit->setText(QString::number(xSlider->value()));
}

void MainWindow::updateXSlider() {
    xSlider->setValue(xLineEdit->text().toInt());
}

void MainWindow::updateYLineEdit() {
    yLineEdit->setText(QString::number(ySlider->value()));
}

void MainWindow::updateYSlider() {
    ySlider->setValue(yLineEdit->text().toInt());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    xSlider = nullptr;
    ySlider = nullptr;
    xLineEdit = nullptr;
    yLineEdit = nullptr;

    resize(800, 600);
    label = new VideoLabel();
}

MainWindow::~MainWindow() {
}
