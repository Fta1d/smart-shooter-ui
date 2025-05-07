#include "inc/appcontroller.h"

void AppController::run() {
    gst->moveToThread(&gstThread);
    connect(&gstThread, &QThread::finished, gst, &QObject::deleteLater);
    connect(this, &AppController::startGstreamerProcess, gst, &GstControl::connectToStream, Qt::QueuedConnection);
    connect(window, &MainWindow::windowClosed, this, &AppController::stopGstreamer);
    gstThread.start();

    window->initMainWindow();
    
    emit startGstreamerProcess();
}

AppController::AppController() {
    gst = new GstControl();
    window = new MainWindow();

}

AppController::~AppController() {
    if (gst && gst->isRunning()) {
        gst->stopPipeline();
    }

    gstThread.quit();
    gstThread.wait();
}