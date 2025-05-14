#include "inc/appcontroller.h"

void AppController::run() {
    // Setup GStreamer thread
    gst->moveToThread(&gstThread);
    connect(&gstThread, &QThread::finished, gst, &QObject::deleteLater);
    connect(window, &MainWindow::startGstProcess, gst, &GstControl::connectToStream, Qt::QueuedConnection);
    connect(window, &MainWindow::windowClosed, this, &AppController::stopApp);

    window->label->setGstreamer(gst);
    connect(gst, &GstControl::frameReady, window->label, &VideoLabel::updateFrame, Qt::QueuedConnection);
    
    // Setup UdpCmdSender thread
    cmdSender->moveToThread(&cmdSenderThread);
    connect(&cmdSenderThread, &QThread::finished, cmdSender, &QObject::deleteLater);
    
    // Connect MainWindow signals to UdpCmdSender
    window->setUdpCmdSender(cmdSender);
    
    // Setup LogoDetector thread
    logoDetector->moveToThread(&logoDetectorThread);
    connect(&logoDetectorThread, &QThread::finished, logoDetector, &QObject::deleteLater);
    
    // Set LogoDetector for MainWindow
    window->setLogoDetector(logoDetector);
    
    // Connect frame update to logo detection
    connect(gst, &GstControl::frameReady, this, &AppController::logoDetection, Qt::QueuedConnection);
    
    // Start all threads
    gstThread.start();
    cmdSenderThread.start();
    logoDetectorThread.start();

    window->initMainWindow();
}

void AppController::logoDetection() {
    QPixmap frame = gst->getFramePixmap();
    if (!frame.isNull()) {
        logoDetector->processFrame(frame.toImage());
    }
}


AppController::AppController() {
    logoDetector = new LogoDetector();
    gst = new GstControl();
    cmdSender = new UdpCmdSender();
    window = new MainWindow();
}

AppController::~AppController() {
    // Stop and clean up GStreamer
    if (gst && gst->isRunning()) {
        gst->stopPipeline();
    }
    
    // Stop and clean up UdpCmdSender
    if (cmdSender) {
        cmdSender->stopSending();
    }

    if (logoDetector) {
        logoDetector->stopDetection();
    }

    // Stop and wait for threads
    gstThread.quit();
    gstThread.wait();
    
    cmdSenderThread.quit();
    cmdSenderThread.wait();

    logoDetectorThread.quit();
    logoDetectorThread.wait();
}