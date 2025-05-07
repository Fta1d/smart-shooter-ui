#include "inc/appcontroller.h"

void AppController::run() {
    // Setup GStreamer thread
    gst->moveToThread(&gstThread);
    connect(&gstThread, &QThread::finished, gst, &QObject::deleteLater);
    connect(window, &MainWindow::startGstProcess, gst, &GstControl::connectToStream, Qt::QueuedConnection);
    connect(window, &MainWindow::windowClosed, this, &AppController::stopGstreamer);

    window->label->setGstreamer(gst);
    connect(gst, &GstControl::frameReady, window->label, &VideoLabel::updateFrame, Qt::QueuedConnection);
    
    // Setup UdpCmdSender thread
    cmdSender->moveToThread(&cmdSenderThread);
    connect(&cmdSenderThread, &QThread::finished, cmdSender, &QObject::deleteLater);
    
    // Connect MainWindow signals to UdpCmdSender
    window->setUdpCmdSender(cmdSender);
    
    // Start threads
    gstThread.start();
    cmdSenderThread.start();

    window->initMainWindow();
}

AppController::AppController() {
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

    // Stop and wait for threads
    gstThread.quit();
    gstThread.wait();
    
    cmdSenderThread.quit();
    cmdSenderThread.wait();
}