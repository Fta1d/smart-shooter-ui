#ifndef GSTCONTROL_H
#define GSTCONTROL_H

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

#include <QObject>
#include <QMutexLocker>
#include <QMutex>
#include <QPixmap>
#include <QImage>
#include <QDebug>

struct CallbackData {
    GMainLoop *main_loop;
    std::function<void(GstSample*)> sample_handler;
};

class GstControl : public QObject {
    Q_OBJECT

    public slots:
        gboolean startPipeline();
        void connectToStream();
        void stopPipeline();
        bool isRunning() const { return running; }

    signals:
        void frameReady();

    private:
        GstElement* pipeline;
        GstElement* source;
        GstElement* converter;
        GstElement* depayloader;
        GstElement* decoder;
        GstElement* sink;
        GMainLoop*  loop;

        gboolean initPipeline();

        struct CallbackData callback_data;
        
        static GstFlowReturn new_sample(GstAppSink *sink, gpointer data);
        GstFlowReturn procesSample(GstSample *sample);

        bool running;

        QMutex imageMutex;

        QPixmap framePixmap;
    public:
        QPixmap getFramePixmap();

        GstControl();
        ~GstControl();
};




#endif // GSTCONTROL_H