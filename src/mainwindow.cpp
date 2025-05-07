#include "inc/mainwindow.h"

void MainWindow::initMainWindow() {
    centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QWidget *topWidget = new QWidget();
    QWidget *bottomWidget = new QWidget();
    
    setupTopLayout(topWidget);

    setupBottomLayout(bottomWidget);

    mainLayout->addWidget(topWidget, 1); 
    mainLayout->addWidget(bottomWidget, 2); 

    connectSignalsAndSlots();
    setCentralWidget(centralWidget);

    show();
}

void MainWindow::setupTopLayout(QWidget *parent) {
    QHBoxLayout *topLayout = new QHBoxLayout(parent);

    label->vidStreamLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topLayout->addWidget(label->vidStreamLabel);
    
    QVBoxLayout *verticalRight = new QVBoxLayout();
    verticalRight->addLayout(createXSliderLayout());
    verticalRight->addLayout(createYSliderLayout());
    verticalRight->addLayout(createStateButtonLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createConnectionLayout());
    
    topLayout->addLayout(verticalRight, 1);
}

void MainWindow::setupBottomLayout(QWidget *parent) {
    QHBoxLayout *bottomLayout = new QHBoxLayout(parent);
    initializeLogWidget();
    bottomLayout->addWidget(log);
}

QHBoxLayout* MainWindow::createStateButtonLayout() {
    QHBoxLayout *stateButtonLayout = new QHBoxLayout();

    initializeButtons();

    stateButtonLayout->addWidget(activeButton);

    return stateButtonLayout;
}

QHBoxLayout* MainWindow::createConnectionLayout() {
    QVBoxLayout *connectionLayout = new QVBoxLayout();

    addressLineEdit = new QLineEdit();
    addressLineEdit->setPlaceholderText("Enter address");

    QRegularExpression regExp("^[0-9.:]+$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);
    addressLineEdit->setValidator(validator);

    connectButton = new QPushButton("Connect");

    connectionLayout->addWidget(new QLabel("Address:"));
    connectionLayout->addWidget(addressLineEdit);
    connectionLayout->addWidget(connectButton);
    
    QHBoxLayout *wrapperLayout = new QHBoxLayout();
    wrapperLayout->addLayout(connectionLayout);
    
    return wrapperLayout;
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

void MainWindow::initializeLogWidget() {
    log = new QPlainTextEdit();
    log->setReadOnly(true);
    log->setPlaceholderText("Event log...");
}

void MainWindow::initializeButtons() {
    activeButton = new QPushButton("Activate", this);
    activeButton->setCheckable(true);
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
    connect(activeButton, &QPushButton::clicked, this, &MainWindow::stateButtonClicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
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

void MainWindow::stateButtonClicked(bool checked) {
    QMutexLocker locker(&log_mutex);

    if (checked) {
        log->appendPlainText("State: activated");
    } else {
        log->appendPlainText("State: deactivated");
    }
    
}

void MainWindow::connectButtonClicked() {
    QMutexLocker locker(&log_mutex);
    QString address = addressLineEdit->text();
    
    if (!address.isEmpty()) {
        log->appendPlainText("Connecting to: " + address);
        // Connection code here
    } else {
        log->appendPlainText("Error: Address field is empty");
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit windowClosed();
    QMainWindow::closeEvent(event);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    xSlider = nullptr;
    ySlider = nullptr;
    xLineEdit = nullptr;
    yLineEdit = nullptr;
    addressLineEdit = nullptr;
    connectButton = nullptr;

    resize(1000, 600);
    label = new VideoLabel();
}

MainWindow::~MainWindow() {
}