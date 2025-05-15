#include "inc/mainwindow.h"
#include "inc/udpcommandsender.h"  // Include UdpCmdSender
#include "inc/logodetector.h"

void MainWindow::initMainWindow() {
    centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QWidget *topWidget = new QWidget();
    QWidget *bottomWidget = new QWidget();
    
    setupTopLayout(topWidget);
    initializeLogWidget();
    setupBottomLayout(bottomWidget);

    QLabel *companyName = new QLabel();
    companyName->setAlignment(Qt::AlignCenter);

    QPixmap textPixmap = createOutlinedTextPixmap(
        "UKR SMART TECH",    // Text
        22,                  // Font size
        QColor("#0057b7"),   // Text color
        Qt::white,           // Edge color
        5                    // Edge thickness
    );
    
    companyName->setPixmap(textPixmap);
    companyName->setContentsMargins(0, -20, 0, -30);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(4);
    shadowEffect->setColor(QColor(0, 0, 0, 160));
    shadowEffect->setOffset(2, 2);
    companyName->setGraphicsEffect(shadowEffect);

    mainLayout->addWidget(companyName);
    mainLayout->addWidget(topWidget, 2); 
    mainLayout->addWidget(bottomWidget, 1);

    connectSignalsAndSlots();

    loadSettings();

    setCentralWidget(centralWidget);
    
    applyDeltaStyle();

    show();
}

void MainWindow::setupTopLayout(QWidget *parent) {
    topWidgetRef = parent; 

    QHBoxLayout *topLayout = new QHBoxLayout(parent);
    parent->setLayout(topLayout);

    topLayout->addWidget(label->vidStreamLabel, 2);
    
    QVBoxLayout *verticalRight = new QVBoxLayout();
    verticalRight->addLayout(createXSliderLayout());
    verticalRight->addLayout(createYSliderLayout());
    verticalRight->addLayout(createStateButtonLayout());
    verticalRight->addWidget(logButton);
    verticalRight->addLayout(createLogoDetectionLayout());
    verticalRight->addLayout(createModeSelectionLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createLogoLayout());
    verticalRight->addStretch();
    verticalRight->addLayout(createConnectionLayout());
    
    topLayout->addLayout(verticalRight, 1);
}

QHBoxLayout* MainWindow::createModeSelectionLayout() {
    QHBoxLayout *modeSelectionLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Select mode:");

    modeSelectBox = new QComboBox();
    modeSelectBox->addItem("Manual", QVariant(1));
    modeSelectBox->addItem("Semi auto", QVariant(2));
    modeSelectBox->addItem("Auto", QVariant(3));

    modeSelectionLayout->addWidget(label);
    modeSelectionLayout->addWidget(modeSelectBox, 1);
    
    return modeSelectionLayout;
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

    QString logoPath = ":/pic/logo.jpg";

    QPixmap pixmap(logoPath);

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
    addressLineEdit->setEnabled(true);

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
    desiredFramesView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(desiredFramesView, &QListWidget::customContextMenuRequested, 
            this, [this](const QPoint &pos) {
        QListWidgetItem *item = desiredFramesView->itemAt(pos);
        if (item) {
            QMenu contextMenu(tr("Frame Context Menu"), this);
            
            QAction *openAction = new QAction(tr("Open Image"), this);
            QAction *deleteAction = new QAction(tr("Delete Image"), this);
            QAction *copyPathAction = new QAction(tr("Copy File Path"), this);
            
            connect(openAction, &QAction::triggered, [this, item]() {
                onFrameItemDoubleClicked(item);
            });
            
            connect(deleteAction, &QAction::triggered, [this, item]() {
                QString filePath = item->data(Qt::UserRole).toString();
                if (!filePath.isEmpty() && QFile::exists(filePath)) {
                    if (QFile::remove(filePath)) {
                        delete desiredFramesView->takeItem(desiredFramesView->row(item));
                        logMessage("File deleted: " + filePath);
                    } else {
                        logError("Failed to delete file: " + filePath);
                    }
                }
            });
            
            connect(copyPathAction, &QAction::triggered, [this, item]() {
                QString filePath = item->data(Qt::UserRole).toString();
                if (!filePath.isEmpty()) {
                    QApplication::clipboard()->setText(filePath);
                    logMessage("File path copied to clipboard");
                }
            });
            
            contextMenu.addAction(openAction);
            contextMenu.addAction(deleteAction);
            contextMenu.addAction(copyPathAction);
            
            contextMenu.exec(desiredFramesView->mapToGlobal(pos));
        }
    });
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

    connect(desiredFramesView, &QListWidget::itemDoubleClicked, this, &MainWindow::onFrameItemDoubleClicked);

    connect(modeSelectBox, &QComboBox::currentIndexChanged, this, [this]() {
        QVariant data = modeSelectBox->currentData();
        emit updateMode(data.toInt());
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
    connect(this, &MainWindow::updateMode, cmdSender, &UdpCmdSender::setMode, Qt::QueuedConnection);
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
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Clear Frames",
        "Do you want to clear only thumbnails or delete all saved files as well?",
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes
    );
    
    if (reply == QMessageBox::Cancel) {
        return;
    }
    
    bool deleteFiles = (reply == QMessageBox::Yes);
    
    if (deleteFiles) {
        QString shootsDir = QDir::currentPath() + "/shoots";
        QDir dir(shootsDir);
        
        if (dir.exists()) {
            QStringList filters;
            filters << "*.png";
            QStringList files = dir.entryList(filters, QDir::Files);
            
            int deletedCount = 0;
            int failedCount = 0;

            foreach (const QString &file, files) {
                QString fullPath = shootsDir + "/" + file;
                bool removed = QFile::remove(fullPath);
                
                if (removed) {
                    deletedCount++;
                } else {
                    failedCount++;
                    logError("Failed to delete file: " + fullPath);
                }
            }
            
            logMessage(QString("Deleted %1 files from shoots folder. Failed: %2")
                      .arg(deletedCount)
                      .arg(failedCount));
        } else {
            logMessage("Shoots directory does not exist.");
        }
    }

    desiredFramesView->clear();

    QPixmapCache::clear();
    QApplication::processEvents();
    QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    
#ifdef Q_OS_LINUX
    malloc_trim(0);
#endif
    
    frameIndex = 0;
    logMessage("All thumbnails cleared and memory freed");
}

void MainWindow::autoSaveFrame() {
    const int MAX_FRAMES_DISPLAYED = 50;
    
    QMutexLocker locker(&label_mutex);
    QPixmap labelPixmap = label->vidStreamLabel->pixmap();

    if (!labelPixmap.isNull()) {
        QString shootsDir = QDir::currentPath() + "/shoots";
        QDir dir(shootsDir);
        if (!dir.exists()) {
            bool created = dir.mkpath(".");
            if (created) {
                logMessage("Created directory for frames: " + shootsDir);
            } else {
                logError("Failed to create directory for frames: " + shootsDir);
                return;
            }
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
        QString filename = QString("shot_%1_%2.png").arg(frameIndex).arg(timestamp);
        QString fullPath = shootsDir + "/" + filename;

        if (labelPixmap.save(fullPath, "PNG")) {
            logMessage("Frame saved to: " + fullPath);

            QPixmap thumbnailPixmap = labelPixmap.scaled(350, 350, Qt::KeepAspectRatio, 
                                                      Qt::SmoothTransformation);

            if (desiredFramesView->count() >= MAX_FRAMES_DISPLAYED) {
                QListWidgetItem *oldItem = desiredFramesView->takeItem(0);
                delete oldItem;
            }

            QListWidgetItem *item = new QListWidgetItem();
            QIcon icon(thumbnailPixmap);
            item->setIcon(icon);

            item->setData(Qt::UserRole, fullPath);
            item->setToolTip(fullPath); 

            desiredFramesView->insertItem(frameIndex++, item);
            desiredFramesView->scrollToItem(item);
        } else {
            logError("Failed to save frame to: " + fullPath);
        }

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
    saveSettings();

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
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        if (keyEvent->key() == Qt::Key_F) {
            if (isFullScreen) {
                exitFullscreen();
            } else {
                enterFullscreen();
            }
            return true;
        } else if (isFullScreen && keyEvent->key() == Qt::Key_Escape) {
            exitFullscreen();
            return true;
        }
    }

    if (event->type() == QEvent::Resize) {
        QLabel *imageLabel = qobject_cast<QLabel*>(watched);
        if (imageLabel && imageLabel->property("isImageViewer").toBool()) {
            QPixmap originalPixmap = imageLabel->property("originalPixmap").value<QPixmap>();
            if (!originalPixmap.isNull()) {
                imageLabel->setPixmap(originalPixmap.scaled(
                    imageLabel->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                ));
                return true;
            }
        }
    }
    
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::enterFullscreen() {
    if (isFullScreen) return;
    
    label->vidStreamLabel->removeEventFilter(this);
    label->vidStreamLabel->setParent(nullptr);
    label->vidStreamLabel->setWindowFlags(Qt::Window);
    label->vidStreamLabel->setAttribute(Qt::WA_DeleteOnClose);
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
    
    if (topWidgetRef && topWidgetRef->layout()) {
        QHBoxLayout *topLayout = qobject_cast<QHBoxLayout*>(topWidgetRef->layout());
        if (topLayout) {
            topLayout->insertWidget(0, label->vidStreamLabel, 2);
        }
    }
    
    label->vidStreamLabel->setMinimumSize(800, 600);
    label->vidStreamLabel->setAlignment(Qt::AlignCenter);
    label->vidStreamLabel->show();
    
    isFullScreen = false;
    this->activateWindow();
}

void MainWindow::onFrameItemDoubleClicked(QListWidgetItem *item) {
    if (item) {
        QString filePath = item->data(Qt::UserRole).toString();
        if (!filePath.isEmpty() && QFile::exists(filePath)) {

            QPixmap pixmap(filePath);
            if (pixmap.isNull()) {
                logError("Failed to load image: " + filePath);
                return;
            }

            QLabel *imageLabel = new QLabel();
            imageLabel->setMinimumSize(800, 600);
            imageLabel->setAlignment(Qt::AlignCenter);

            imageLabel->setProperty("originalPixmap", QVariant::fromValue(pixmap));
            imageLabel->setProperty("isImageViewer", true);

            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            imageLabel->setWindowFlags(Qt::Window);
            imageLabel->setWindowTitle(QFileInfo(filePath).fileName());
            imageLabel->setAttribute(Qt::WA_DeleteOnClose);

            imageLabel->installEventFilter(this);

            imageLabel->show();
            imageLabel->activateWindow();
            
            QObject::connect(imageLabel, &QLabel::destroyed, [=]() {
                // Cleanup
            });
        } else {
            logError("Cannot open file (doesn't exist): " + filePath);
        }
    }
}

void MainWindow::setupMemoryManagement() {
    QTimer *cacheCleanTimer = new QTimer(this);

    connect(cacheCleanTimer, &QTimer::timeout, this, [this]() {
        // desiredFramesView->clear();
        QPixmapCache::clear();
        QApplication::processEvents();
        
#ifdef Q_OS_LINUX
        malloc_trim(0);
#endif
        
        logMessage("Auto memory cleanup: thumbnails cleared and memory freed");
    });

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
    modeSelectBox = nullptr;
    gstRunning = false;
    udpConnected = false;
    cmdSender = nullptr;
    topWidgetRef = nullptr;

    frameHeight = 640;
    frameWidth = 640;
    frameIndex = 0;
    
    // Initialize current values
    currentXValue = frameWidth / 2;
    currentYValue = frameHeight / 2;
    currentShotValue = false;
    currentActiveValue = false;
    isFullScreen = false;
    isCleaningMemory = false;
    detection = false;

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

void MainWindow::loadSettings() {
    QSettings settings("UKR SMART TECH", "smart-shooter");

    int savedX = settings.value("position/x", frameWidth / 2).toInt();
    int savedY = settings.value("position/y", frameHeight / 2).toInt();

    savedX = qBound(0, savedX, frameWidth);
    savedY = qBound(0, savedY, frameHeight);

    currentXValue = savedX;
    currentYValue = savedY;

    xSlider->setValue(savedX);
    ySlider->setValue(savedY);

    updateXLineEdit();
    updateYLineEdit();

    emit updateXValue(currentXValue);
    emit updateYValue(currentYValue);
}

void MainWindow::saveSettings() {
    QSettings settings("UKR SMART TECH", "smart-shooter");

    settings.setValue("position/x", currentXValue);
    settings.setValue("position/y", currentYValue);
    
    settings.sync();
}

QPixmap MainWindow::createOutlinedTextPixmap(const QString &text, int fontSize, const QColor &textColor, 
    const QColor &outlineColor, qreal outlineWidth) {
    QPixmap pixmap(600, 80);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font("Monaco", fontSize, QFont::Bold);
    painter.setFont(font);

    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(0, 0, pixmap.width(), pixmap.height(), Qt::AlignCenter, text);

    QPainterPath path;
    path.addText(pixmap.width()/2 - textRect.width()/2, 
    pixmap.height()/2 + fm.ascent()/2 - fm.descent(), 
    font, text);

    painter.setPen(QPen(outlineColor, outlineWidth));
    painter.drawPath(path);

    painter.setPen(Qt::NoPen);
    painter.setBrush(textColor);
    painter.drawPath(path);

    return pixmap;
}
