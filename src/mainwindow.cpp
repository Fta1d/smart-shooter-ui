#include "inc/mainwindow.h"
#include "inc/udpcommandsender.h"  // Include UdpCmdSender
#include "inc/logodetector.h"

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
    
    applyDeltaStyle();

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
    verticalRight->addLayout(createLogoDetectionLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createLogoLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createConnectionLayout());
    
    topLayout->addLayout(verticalRight, 1);
}

QVBoxLayout* MainWindow::createLogoDetectionLayout() {
    QVBoxLayout *logoDetectionLayout = new QVBoxLayout();

    logoDetectionEnabled = new QCheckBox("Enable successful shot save");
    logoDetectionEnabled->setChecked(false);

    clearFramesButton = new QPushButton("Clear Cache");

    logoDetectionLayout->addWidget(clearFramesButton);
    logoDetectionLayout->addWidget(logoDetectionEnabled);
    
    return logoDetectionLayout;
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
    desiredFramesView->setIconSize(QSize(350, 350));
}

void MainWindow::initializeButtons() {
    activeButton = new QPushButton("Activate", this);
    activeButton->setCheckable(true);
    
    shotButton = new QPushButton("Shot", this);
    shotButton->setCheckable(false);

    logButton = new QPushButton("Show Log", this);
    logButton->setCheckable(false);
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

    // Connect signal values to cache them locally
    connect(xSlider, &QSlider::valueChanged, [this](int value) {
        currentXValue = value;
        emit updateXValue(value);
    });
    
    connect(ySlider, &QSlider::valueChanged, [this](int value) {
        currentYValue = value;
        emit updateYValue(value);
    });

    connect(logoDetectionEnabled, &QCheckBox::toggled, this, &MainWindow::logoDetectionToggled);
    connect(clearFramesButton, &QPushButton::clicked, this, &MainWindow::clearFramesButtonClicked);
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

void MainWindow::setLogoDetector(LogoDetector *detector) {
    logoDetector = detector;

    connect(this, &MainWindow::startLogoDetection, logoDetector, &LogoDetector::startDetection, Qt::QueuedConnection);
    connect(this, &MainWindow::stopLogoDetection, logoDetector, &LogoDetector::stopDetection, Qt::QueuedConnection);

    connect(logoDetector, &LogoDetector::logoDetected, this, &MainWindow::autoSaveFrame, Qt::QueuedConnection);
    connect(logoDetector, &LogoDetector::message, this, &MainWindow::logMessage, Qt::QueuedConnection);
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
    // Update active state
    currentActiveValue = checked;
    emit updateActiveValue(checked);
    
    if (checked) {
        logMessage("State: activated");
    } else {
        logMessage("State: deactivated");
    }
}

void MainWindow::shotButtonClicked() { 
    // Set shot value to true
    currentShotValue = true;
    emit updateShotValue(true);
    logMessage("Shot fired!");
    
    // Reset shot value to false after a short delay
    QTimer::singleShot(10, [this]() {
        currentShotValue = false;
        emit updateShotValue(false);
        logMessage("Shot reset");
    });
}

void MainWindow::connectButtonClicked() {
    // QMutexLocker locker(&log_mutex);
    QString address = addressLineEdit->text();
    QStringList addressPortList = address.split(':');

    if (!address.isEmpty()) {
        if (udpConnected) {
            logMessage("UDP already connected!");
            return;
        }

        logMessage("Connecting UDP to: " + address);
        
        // Set destination and start sending
        emit updateXValue(320);
        emit updateYValue(320);
        emit startUdpSending(addressPortList[0], addressPortList[1].toInt());
        
        udpConnected = true;
        
        // Start GStreamer if not already running
        if (!gstRunning) {
            logMessage("Starting video stream from: " + QStringLiteral("5060"));
            gstRunning = true;
            emit startGstProcess();
        }
    } else {
        logMessage("Error: Address field is empty");
    }
}

void MainWindow::logoDetectionToggled(bool checked) {
    if (checked) {
        detection = true;
        emit startLogoDetection();
        logMessage("Logo detection enabled");
    } else {
        detection = false;
        emit stopLogoDetection();
        logMessage("Logo detection disabled");
    }
}

void MainWindow::clearFramesButtonClicked() {
    QListWidgetItem *item;

    while ((item = desiredFramesView->item(0)) != nullptr) {
        QIcon icon = item->icon();
        icon = QIcon(); 
        
        delete desiredFramesView->takeItem(0); 
    }

    QApplication::processEvents();
    QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    
    frameIndex = 0;
    malloc_trim(0);
    logMessage("All saved frames cleared and memory freed");
}

void MainWindow::autoSaveFrame() {
    const int MAX_FRAMES = 50;

    QMutexLocker locker(&label_mutex);

    if (desiredFramesView->count() >= MAX_FRAMES) {
        delete desiredFramesView->takeItem(0);
        frameIndex--; 
    }

    QPixmap labelPixmap = label->vidStreamLabel->pixmap();

    if (!labelPixmap.isNull()) {
        QPixmap optimizedPixmap = labelPixmap.scaled(350, 350, Qt::KeepAspectRatio, 
                                                   Qt::SmoothTransformation);
        
        QListWidgetItem *item = new QListWidgetItem();
        QIcon icon(optimizedPixmap);
        item->setIcon(icon);

        desiredFramesView->insertItem(frameIndex++, item);
        desiredFramesView->scrollToItem(item);

        labelPixmap = QPixmap();
    }
}

void MainWindow::showLog() {
    if (!log->isVisible()) {
        log->resize(400, 400);
        log->show();
    } else {
        log->close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Stop UDP sending
    if (udpConnected) {
        emit stopUdpSending();
    }
    
    emit windowClosed();
    QMainWindow::closeEvent(event);
}

// Log slots
void MainWindow::logMessage(const QString &message) {
    QMutexLocker locker(&log_mutex);
    log->appendPlainText(message);
}

void MainWindow::logError(const QString &errorMessage) {
    QMutexLocker locker(&log_mutex);
    log->appendPlainText("ERROR: " + errorMessage);
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (!isFullScreen && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F) {
            enterFullscreen();
            return true;  
        }
    }

    if (isFullScreen && watched == label->vidStreamLabel && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F || keyEvent->key() == Qt::Key_Escape) {
            exitFullscreen();
            return true; 
        }
    }
    
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::enterFullscreen() {
    if (isFullScreen) return;
    
    label->vidStreamLabel->removeEventFilter(this);
    label->vidStreamLabel->setParent(nullptr);
    label->vidStreamLabel->setWindowFlags(Qt::Window);
    label->vidStreamLabel->installEventFilter(this);
    
    label->vidStreamLabel->showFullScreen();
    label->vidStreamLabel->activateWindow();
    label->vidStreamLabel->setFocus(Qt::ActiveWindowFocusReason);
    
    isFullScreen = true;
}

void MainWindow::exitFullscreen() {
    if (!isFullScreen) return;
    
    label->vidStreamLabel->hide();
    label->vidStreamLabel->setWindowFlags(Qt::Widget);
    
    QWidget *topWidget = centralWidget->findChildren<QWidget*>().at(0);
    QLayout *topLayout = topWidget->layout();
    if (topLayout) {
        QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(topLayout);
        if (hLayout) {
            hLayout->insertWidget(0, label->vidStreamLabel, 2);
        }
    }
    
    label->vidStreamLabel->setMinimumSize(800, 600);
    label->vidStreamLabel->setAlignment(Qt::AlignCenter);
    label->vidStreamLabel->show();
    
    isFullScreen = false;
    this->activateWindow();
}

void MainWindow::setupMemoryManagement() {
    QTimer *cacheCleanTimer = new QTimer(this);
    connect(cacheCleanTimer, &QTimer::timeout, this, &MainWindow::clearFramesButtonClicked);

    cacheCleanTimer->start(120000);
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
    isCleaningMemory = false;
    detection = false;

    frameHeight = 640;
    frameWidth = 640;
    frameIndex = 0;

    resize(1160, 1000);
    label = new VideoLabel();

    setupMemoryManagement();

    qApp->installEventFilter(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MainWindow::~MainWindow() {
    // cmdSender is not owned by MainWindow, so we don't delete it here
}

void MainWindow::applyDeltaStyle() {
    // Special styling for video label
    if (label && label->vidStreamLabel) {
        label->vidStreamLabel->setStyleSheet(R"(
            QLabel {
                background-color: #000000;
                border: 1px solid #4A5660;
                border-radius: 3px;
                color: #FFFFFF;
            }
        )");
    }
    
    // Make containers look like cards
    QList<QWidget*> topLevelWidgets = centralWidget->findChildren<QWidget*>();
    for (QWidget* widget : topLevelWidgets) {
        if (widget->parent() == centralWidget && widget->objectName() != "") {
            widget->setStyleSheet(widget->styleSheet() + R"(
                QWidget {
                    background-color: #1F2529;
                    border: 1px solid #4A5660;
                    border-radius: 4px;
                    padding: 10px;
                }
            )");
        }
    }
}
