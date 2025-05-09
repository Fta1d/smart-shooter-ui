#include "inc/mainwindow.h"
#include "inc/udpcommandsender.h"  // Include UdpCmdSender

void MainWindow::initMainWindow() {
    centralWidget = new QWidget(this);
    // centralWidget->setStyleSheet("background-color:rgb(0, 9, 51);");

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QWidget *topWidget = new QWidget();
    QWidget *bottomWidget = new QWidget();
    
    setupTopLayout(topWidget);
    initializeLogWidget();
    setupBottomLayout(bottomWidget);

    mainLayout->addWidget(topWidget, 2); 
    mainLayout->addWidget(bottomWidget, 1);

    connectSignalsAndSlots();
    setCentralWidget(centralWidget);
    
    show();
}

void MainWindow::setupTopLayout(QWidget *parent) {
    QHBoxLayout *topLayout = new QHBoxLayout(parent);
    parent->setLayout(topLayout);

    // label->vidStreamLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topLayout->addWidget(label->vidStreamLabel, 2);
    
    QVBoxLayout *verticalRight = new QVBoxLayout();
    verticalRight->addLayout(createXSliderLayout());
    verticalRight->addLayout(createYSliderLayout());
    verticalRight->addLayout(createStateButtonLayout());
    verticalRight->addWidget(logButton);
    verticalRight->addWidget(saveframeButton);
    verticalRight->addStretch();
    verticalRight->addLayout(createLogoLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createConnectionLayout());
    
    topLayout->addLayout(verticalRight, 1);
}

void MainWindow::setupBottomLayout(QWidget *parent) {
    QHBoxLayout *bottomLayout = new QHBoxLayout(parent);
    
    initializeDesiredFramesViewWidget();

    bottomLayout->addWidget(desiredFramesView);
}

QHBoxLayout* MainWindow::createLogoLayout() {
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QLabel *logoLabel = new QLabel();

    QPixmap pixmap("../logo.jpg");

    pixmap = pixmap.scaled(316, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // pixmap.scaledToHeight(350);

    logoLabel->setPixmap(pixmap);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLayout->addWidget(logoLabel);

    return logoLayout;
}

QHBoxLayout* MainWindow::createStateButtonLayout() {
    QHBoxLayout *stateButtonLayout = new QHBoxLayout();

    initializeButtons();

    stateButtonLayout->addWidget(activeButton);
    stateButtonLayout->addWidget(shotButton);

    return stateButtonLayout;
}

QHBoxLayout* MainWindow::createConnectionLayout() {
    QVBoxLayout *connectionLayout = new QVBoxLayout();

    addressLineEdit = new QLineEdit();
    addressLineEdit->setPlaceholderText("Enter address");
    addressLineEdit->setText("127.0.0.1:5050"); // Default to localhost
    addressLineEdit->setEnabled(false);

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
    initializeXSlider(xSlider);
    
    xLineEdit = new QLineEdit();
    initializeLineEditX(xLineEdit);
    
    QLabel *xLabel = new QLabel("X:");
    
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xSlider);
    xLayout->addWidget(xLineEdit);
    
    return xLayout;
}

QHBoxLayout* MainWindow::createYSliderLayout() {
    QHBoxLayout *yLayout = new QHBoxLayout();

    ySlider = new QSlider(Qt::Horizontal);
    initializeYSlider(ySlider);

    yLineEdit = new QLineEdit();
    initializeLineEditY(yLineEdit);

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
    log->setWindowFlag(Qt::Window);
}

void MainWindow::initializeDesiredFramesViewWidget() {
    desiredFramesView = new QListWidget();
    desiredFramesView->setFlow(QListView::LeftToRight);
    desiredFramesView->setIconSize(QSize(390, 390));
}

void MainWindow::initializeButtons() {
    activeButton = new QPushButton("Activate", this);
    activeButton->setCheckable(true);
    
    shotButton = new QPushButton("Shot", this);
    shotButton->setCheckable(false);

    logButton = new QPushButton("Show Log", this);
    logButton->setCheckable(false);

    saveframeButton = new QPushButton("Save Frame", this);
    saveframeButton->setCheckable(false);
}

void MainWindow::initializeXSlider(QSlider *slider) {
    slider->setMinimum(0);
    slider->setMaximum(frameWidth);
    slider->setValue(frameWidth / 2); 
}

void MainWindow::initializeYSlider(QSlider *slider) {
    slider->setMinimum(0);
    slider->setMaximum(frameHeight);
    slider->setValue(frameHeight / 2); 
}

void MainWindow::initializeLineEditX(QLineEdit *lineEdit) {
    lineEdit->setValidator(new QIntValidator(0, frameWidth, this));
    lineEdit->setMaximumWidth(50);
    lineEdit->setText(QString::number(frameWidth / 2)); 
}

void MainWindow::initializeLineEditY(QLineEdit *lineEdit) {
    lineEdit->setValidator(new QIntValidator(0, frameHeight, this));
    lineEdit->setMaximumWidth(50);
    lineEdit->setText(QString::number(frameHeight / 2)); 
}

void MainWindow::connectSignalsAndSlots() {
    // Connect sliders and line edits for UI updates
    connect(xSlider, &QSlider::valueChanged, this, &MainWindow::updateXLineEdit);
    connect(xLineEdit, &QLineEdit::editingFinished, this, &MainWindow::updateXSlider);
    connect(ySlider, &QSlider::valueChanged, this, &MainWindow::updateYLineEdit);
    connect(yLineEdit, &QLineEdit::editingFinished, this, &MainWindow::updateYSlider);
    
    // Connect buttons
    connect(activeButton, &QPushButton::clicked, this, &MainWindow::stateButtonClicked);
    connect(shotButton, &QPushButton::clicked, this, &MainWindow::shotButtonClicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
    connect(logButton, &QPushButton::clicked, this, &MainWindow::showLog);
    connect(saveframeButton, &QPushButton::clicked, this, &MainWindow::saveFrame);

    // Connect signal values to cache them locally
    connect(xSlider, &QSlider::valueChanged, [this](int value) {
        currentXValue = value;
        emit updateXValue(value);
    });
    
    connect(ySlider, &QSlider::valueChanged, [this](int value) {
        currentYValue = value;
        emit updateYValue(value);
    });
}

void MainWindow::setUdpCmdSender(UdpCmdSender *sender) {
    cmdSender = sender;
    
    // Connect UdpCmdSender signals to MainWindow slots
    connect(cmdSender, &UdpCmdSender::messageSent, this, &MainWindow::logMessage, Qt::QueuedConnection);
    connect(cmdSender, &UdpCmdSender::errorOccurred, this, &MainWindow::logError, Qt::QueuedConnection);
    
    // Connect MainWindow signals to UdpCmdSender slots
    connect(this, &MainWindow::startUdpSending, cmdSender, &UdpCmdSender::startSending, Qt::QueuedConnection);
    connect(this, &MainWindow::stopUdpSending, cmdSender, &UdpCmdSender::stopSending, Qt::QueuedConnection);
    connect(this, &MainWindow::updateXValue, cmdSender, &UdpCmdSender::setXValue, Qt::QueuedConnection);
    connect(this, &MainWindow::updateYValue, cmdSender, &UdpCmdSender::setYValue, Qt::QueuedConnection);
    connect(this, &MainWindow::updateShotValue, cmdSender, &UdpCmdSender::setShotValue, Qt::QueuedConnection);
    connect(this, &MainWindow::updateActiveValue, cmdSender, &UdpCmdSender::setActiveValue, Qt::QueuedConnection);
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

    // Update active state
    currentActiveValue = checked;
    emit updateActiveValue(checked);
    
    if (checked) {
        log->appendPlainText("State: activated");
    } else {
        log->appendPlainText("State: deactivated");
    }
}

void MainWindow::shotButtonClicked() {
    QMutexLocker locker(&log_mutex);
    
    // Set shot value to true
    currentShotValue = true;
    emit updateShotValue(true);
    log->appendPlainText("Shot fired!");
    
    // Reset shot value to false after a short delay
    QTimer::singleShot(10, [this]() {
        currentShotValue = false;
        emit updateShotValue(false);
        QMutexLocker locker(&log_mutex);
        log->appendPlainText("Shot reset");
    });
}

void MainWindow::connectButtonClicked() {
    QMutexLocker locker(&log_mutex);
    QString address = addressLineEdit->text();
    QStringList addressPortList = address.split(':');

    if (!address.isEmpty()) {
        if (udpConnected) {
            log->appendPlainText("UDP already connected!");
            return;
        }

        log->appendPlainText("Connecting UDP to: " + address);
        
        // Set destination and start sending
        emit updateXValue(320);
        emit updateYValue(320);
        emit startUdpSending(addressPortList[0], addressPortList[1].toInt());
        
        udpConnected = true;
        
        // Start GStreamer if not already running
        if (!gstRunning) {
            log->appendPlainText("Starting video stream from: " + QStringLiteral("5060"));
            gstRunning = true;
            emit startGstProcess();
        }
    } else {
        log->appendPlainText("Error: Address field is empty");
    }
}

void MainWindow::showLog() {
    if (!log->isVisible()) {
        log->show();
    } else {
        log->close();
    }
}

void MainWindow::saveFrame() {
    QMutexLocker locker(&label_mutex);
    QListWidgetItem *item = new QListWidgetItem();

    QPixmap labelPixmap = label->vidStreamLabel->pixmap();

    QIcon icon(labelPixmap);
    item->setIcon(icon);

    desiredFramesView->insertItem(frameIndex++, item);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Stop UDP sending
    if (udpConnected) {
        emit stopUdpSending();
    }
    
    emit windowClosed();
    QMainWindow::closeEvent(event);
}

// UdpCmdSender log slots
void MainWindow::logMessage(const QString &message) {
    QMutexLocker locker(&log_mutex);
    log->appendPlainText(message);
}

void MainWindow::logError(const QString &errorMessage) {
    QMutexLocker locker(&log_mutex);
    log->appendPlainText("ERROR: " + errorMessage);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_F) {
        if (!isFullScreen) {
            qDebug() << "Full screen called";

            label->vidStreamLabel->removeEventFilter(this);
            label->vidStreamLabel->setParent(nullptr);
            label->vidStreamLabel->setWindowFlags(Qt::Window);
            label->vidStreamLabel->setStyleSheet("background-color: black;");
            label->vidStreamLabel->installEventFilter(this);

            label->vidStreamLabel->showFullScreen();
            label->vidStreamLabel->setFocus(Qt::OtherFocusReason);

            isFullScreen = true;
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == label->vidStreamLabel && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F || keyEvent->key() == Qt::Key_Escape) {
            qDebug() << "Normal screen called";

            label->vidStreamLabel->setWindowFlags(Qt::Widget);
            label->vidStreamLabel->hide();
            label->vidStreamLabel->setParent(nullptr); 
            label->vidStreamLabel->setParent(label);   

            QWidget *topWidget = centralWidget->findChildren<QWidget*>().at(0);
            QLayout *topLayout = topWidget->layout();
            if (topLayout) {
                QHBoxLayout *hLayout = qobject_cast<QHBoxLayout *>(topLayout);
                if (hLayout) {
                    hLayout->insertWidget(0, label->vidStreamLabel);
                }
            }

            label->vidStreamLabel->setMinimumSize(800, 600);
            label->vidStreamLabel->setAlignment(Qt::AlignCenter);
            label->vidStreamLabel->show();
            
            isFullScreen = false;
            
            return true;
        }
    }
    
    return QMainWindow::eventFilter(watched, event);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    xSlider = nullptr;
    ySlider = nullptr;
    xLineEdit = nullptr;
    yLineEdit = nullptr;
    addressLineEdit = nullptr;
    connectButton = nullptr;
    shotButton = nullptr;
    gstRunning = false;
    udpConnected = false;
    cmdSender = nullptr;
    
    // Initialize current values
    currentXValue = 0;
    currentYValue = 0;
    currentShotValue = false;
    currentActiveValue = false;
    isFullScreen = false;

    frameHeight = 640;
    frameWidth = 640;
    frameIndex = 0;

    resize(1160, 830);
    label = new VideoLabel();

    
    // setFixedWidth(1160);
    // setFixedHeight(830); 
    
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MainWindow::~MainWindow() {
    // cmdSender is not owned by MainWindow, so we don't delete it here
}