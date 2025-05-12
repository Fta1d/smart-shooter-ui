#ifndef APPCONTROLER_H
#define APPCONTROLER_H

#include <QThread>
#include <QApplication>

#include "inc/gstcontrol.h"
#include "inc/mainwindow.h"
#include "inc/udpcommandsender.h"

class AppController : public QObject {
    Q_OBJECT

    private:
        GstControl *gst;
        UdpCmdSender *cmdSender;  // Added UdpCmdSender
        MainWindow *window;
        QThread gstThread;
        QThread cmdSenderThread;  // Added thread for UdpCmdSender

    public slots:
        void stopApp() {
            if (gst && gst->isRunning()) {
                gst->stopPipeline();
            }
            
            // Stop the command sender before quitting
            if (cmdSender) {
                cmdSender->stopSending();
            }
            
            qApp->quit();
        }
        
    public:
        void run();

        AppController();
        ~AppController();
    
};


#endif // APPCONTROLER_H