#ifndef APPCONTROLER_H
#define APPCONTROLER_H

#include <QThread>
#include <QApplication>

#include "inc/gstcontrol.h"
#include "inc/mainwindow.h"

class AppController : public QObject {
    Q_OBJECT

    private:
        GstControl *gst;
        MainWindow *window;
        QThread gstThread;

    public slots:
        void stopGstreamer() {
            if (gst && gst->isRunning()) {
                gst->stopPipeline();
            }
            qApp->quit();
        }
        
    public:
        void run();

        AppController();
        ~AppController();
    
};


#endif // APPCONTROLER_H