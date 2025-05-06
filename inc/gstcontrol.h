#ifndef GSTCONTROL_H
#define GSTCONTROL_H

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

#include <QObject>

class GstControl {
    Q_OBJECT

    private:
        GstElement* pipeline;
        GstElement* source;
        GstElement* converter;
        GstElement* depayloader;
        GstElement* decoder;
        GstElement* sink;
        static GMainLoop *loop;

        gboolean initPipeline();
        gboolean startPipeline();
        
        static GstFlowReturn new_sample(GstAppSink *sink, gpointer data);

    public:
        void connectToStream();

        GstControl();
        ~GstControl();
};




#endif // GSTCONTROL_H