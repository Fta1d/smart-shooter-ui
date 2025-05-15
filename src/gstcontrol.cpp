#include "inc/gstcontrol.h"

static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer data) {
    GstControl *control = static_cast<GstControl*>(data);
    
    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR: {
            GError *err;
            gchar *debug;
            gst_message_parse_error(message, &err, &debug);
            qDebug() << "Error:" << err->message;
            qDebug() << "Debug:" << debug;
            g_error_free(err);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_WARNING: {
            GError *err;
            gchar *debug;
            gst_message_parse_warning(message, &err, &debug);
            qDebug() << "Warning:" << err->message;
            qDebug() << "Debug:" << debug;
            g_error_free(err);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_INFO: {
            GError *err;
            gchar *debug;
            gst_message_parse_info(message, &err, &debug);
            qDebug() << "Info:" << err->message;
            qDebug() << "Debug:" << debug;
            g_error_free(err);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_EOS:
            qDebug() << "End of stream";
            break;
        default:
            break;
    }
    
    return TRUE;
}

gboolean GstControl::initPipeline() {
    gst_debug_set_default_threshold(GST_LEVEL_WARNING);
    // GstBus *bus;

    // g_print("Initializing webcam pipeline...\n");

    // pipeline = gst_pipeline_new("webcam-pipeline");
    // source = gst_element_factory_make("v4l2src", "webcam-source");  // v4l2src для Linux
    // converter = gst_element_factory_make("videoconvert", "converter");
    // sink = gst_element_factory_make("appsink", "video-output");

    // if (!pipeline || !source || !converter || !sink) {
    //     g_printerr("Could not create webcam pipeline elements.\n");
    //     return -1;
    // }

    // // Налаштування для веб-камери
    // // Можете змінити розширення і частоту кадрів, якщо потрібно
    // g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

    // // Налаштування для appsink
    // g_object_set(G_OBJECT(sink), "emit-signals", TRUE, NULL);
    // GstCaps *caps = gst_caps_from_string("video/x-raw, format=(string)RGB, width=640, height=480, framerate=30/1");
    // g_object_set(G_OBJECT(sink), "caps", caps, NULL);
    // gst_caps_unref(caps);
    // g_signal_connect(sink, "new-sample", G_CALLBACK(new_sample), &callback_data);

    // gst_bin_add_many(GST_BIN(pipeline), source, converter, sink, NULL);

    // if (!gst_element_link_many(source, converter, sink, NULL)) {
    //     g_printerr("Webcam pipeline elements cannot be linked!\n");
    //     gst_object_unref(GST_OBJECT(pipeline));
    //     return -1;
    // }

    // bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    // gst_bus_add_watch(bus, NULL /*bus callback here*/, NULL);
    // gst_object_unref(bus);


    GstBus *bus;
    GstCaps *caps;
    GstElement *queue1, *queue2, *queue3;

    pipeline = gst_pipeline_new("udp-video-pipeline");
    source = gst_element_factory_make("udpsrc", "udp-source");
    queue1 = gst_element_factory_make("queue", "queue1");
    depayloader = gst_element_factory_make("rtph264depay", "depayloader");
    queue2 = gst_element_factory_make("queue", "queue2");
    decoder = gst_element_factory_make("avdec_h264", "decoder");
    queue3 = gst_element_factory_make("queue", "queue3");
    converter = gst_element_factory_make("videoconvert", "converter");
    sink = gst_element_factory_make("appsink", "video-output");

    if (!pipeline || !source || !queue1 || !depayloader || !queue2 || !decoder || !queue3 || !converter || !sink) {
        g_printerr("Could not create pipeline element.\n");
        return -1;
    }

    // Configure source element
    g_object_set(G_OBJECT(source), "port", 5060, NULL);
    caps = gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264");
    g_object_set(G_OBJECT(source), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Configure queues elements
    g_object_set(G_OBJECT(queue1), "max-size-buffers", 10, "max-size-time", 0, "max-size-bytes", 0, NULL);
    g_object_set(G_OBJECT(queue2), "max-size-buffers", 10, "max-size-time", 0, "max-size-bytes", 0, NULL);
    g_object_set(G_OBJECT(queue3), "max-size-buffers", 10, "max-size-time", 0, "max-size-bytes", 0, NULL);

    // Configure sink element
    g_object_set(G_OBJECT(sink), "emit-signals", TRUE, NULL);
    caps = gst_caps_from_string("video/x-raw, format=(string)RGBA");
    g_object_set(G_OBJECT(sink), "caps", caps, NULL);
    gst_caps_unref(caps);
    g_signal_connect(sink, "new-sample", G_CALLBACK(new_sample), &callback_data);

    gst_bin_add_many(GST_BIN(pipeline), source, queue1, depayloader, queue2, decoder, queue3, converter, sink, NULL);

    if (!gst_element_link_many(source, queue1, depayloader, queue2, decoder, queue3, converter, sink, NULL)) {
        g_printerr("Pipeline elements cannot be linked!\n");
        gst_object_unref(GST_OBJECT(pipeline));
        return -1;
    }

    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_callback, this);
    gst_object_unref(bus);

    return 1;
}

gboolean GstControl::startPipeline() {
    GstStateChangeReturn ret;

    ret = gst_element_set_state(this->pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (this->pipeline);
        return -1;
    }

    g_print("Started pipeline!\n");

    running = true;

    this->loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(this->loop);

    g_main_loop_unref(this->loop);

    return GST_FLOW_OK;
}

GstFlowReturn GstControl::new_sample(GstAppSink *sink, gpointer data) {
    CallbackData *cb_data = static_cast<CallbackData*>(data);
    GstSample *sample = gst_app_sink_pull_sample(sink);

    if (sample) {
        cb_data->sample_handler(sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}

GstFlowReturn GstControl::procesSample(GstSample *sample) {
    QMutexLocker locker(&imageMutex);

    GstBuffer *buffer;
    GstMapInfo map;

    if (sample) {
        buffer = gst_sample_get_buffer(sample);
        GstCaps *caps = gst_sample_get_caps(sample);
        if (!caps) {
            gst_sample_unref(sample);
            return GST_FLOW_ERROR;
        }

        GstVideoInfo video_info;
        gst_video_info_init(&video_info);
        if (!gst_video_info_from_caps(&video_info, caps)) {
            gst_sample_unref(sample);
            return GST_FLOW_ERROR;
        }

        if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
            QImage::Format format;
            switch (GST_VIDEO_INFO_FORMAT(&video_info)) {
                case GST_VIDEO_FORMAT_RGB:
                    format = QImage::Format_RGB888;
                    break;
                case GST_VIDEO_FORMAT_RGBA:
                    format = QImage::Format_RGBA8888;
                    break;
                case GST_VIDEO_FORMAT_BGRA:
                    format = QImage::Format_ARGB32;
                    break;
                case GST_VIDEO_FORMAT_BGR:
                    format = QImage::Format_RGB888;
                    break;
                default:
                    g_printerr("Wrong video format!\n");
                    gst_buffer_unmap(buffer, &map);
                    gst_sample_unref(sample);
                    return GST_FLOW_ERROR;
            }

            QImage image(
                map.data,
                GST_VIDEO_INFO_WIDTH(&video_info),
                GST_VIDEO_INFO_HEIGHT(&video_info),
                GST_VIDEO_INFO_PLANE_STRIDE(&video_info, 0),
                format
            );

            if (GST_VIDEO_INFO_FORMAT(&video_info) == GST_VIDEO_FORMAT_BGR) {
                image = image.rgbSwapped();
            }

            QImage imageCopy = image.copy();

            gst_buffer_unmap(buffer, &map);

            framePixmap = QPixmap::fromImage(imageCopy);

            emit frameReady();

            gst_sample_unref(sample);
            return GST_FLOW_OK;
        }

        gst_sample_unref(sample);
        return GST_FLOW_OK;
    }

    return GST_FLOW_ERROR;
}

void GstControl::connectToStream() {
    gst_init(NULL, NULL);

    initPipeline();
    startPipeline();
}


void GstControl::stopPipeline() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
    }

    if (loop && g_main_loop_is_running(loop)) {
        g_main_loop_quit(loop);
        running = false;
    }
}

GstControl::GstControl() {
    pipeline = nullptr;
    source = nullptr;
    converter = nullptr;
    depayloader = nullptr;
    decoder = nullptr;
    sink = nullptr;
    loop = nullptr;
    running = false;

    callback_data.main_loop = nullptr;
        
    callback_data.sample_handler = [this](GstSample *sample) {
        this->procesSample(sample);
    };

    g_print("Initializzed gst!\n");
}

GstControl::~GstControl() {

}

QPixmap GstControl::getFramePixmap() {
    QMutexLocker locker(&imageMutex);
    return framePixmap;
}
