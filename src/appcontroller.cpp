#include "inc/appcontroller.h"

void AppController::run() {
    gst->moveToThread(&gstThread);
    connect(&gstThread, &QThread::finished, gst, &QObject::deleteLater);
    connect(window, &MainWindow::startGstProcess, gst, &GstControl::connectToStream, Qt::QueuedConnection);
    connect(window, &MainWindow::windowClosed, this, &AppController::stopGstreamer);

    window->label->setGstreamer(gst);

    connect(gst, &GstControl::frameReady, window->label, &VideoLabel::updateFrame, Qt::QueuedConnection);

    gstThread.start();

    window->initMainWindow();
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