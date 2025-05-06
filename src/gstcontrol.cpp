#include "inc/gstcontrol.h"

gboolean GstControl::initPipeline() {
    GstBus *bus;
    GstCaps *caps;

    pipeline = gst_pipeline_new("udp-video-pipeline");
    source = gst_element_factory_make("udpsrc", "udp-source");
    depayloader = gst_element_factory_make("rtph264depay", "depayloader");
    decoder = gst_element_factory_make("avdec_h264", "decoder");
    converter = gst_element_factory_make("videoconvert", "converter");
    sink = gst_element_factory_make("autovideosink", "video-output");

    if (!pipeline || !source || !depayloader || !decoder || !converter || !sink) {
        g_printerr("Could not create pipeline element.\n");
        return -1;
    }

    g_object_set(G_OBJECT(source), "port", 5060, NULL);

    caps = gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264");
    g_object_set(G_OBJECT(source), "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(G_OBJECT(sink), "emit-signals", TRUE, NULL);
    g_signal_connect(sink, "new-sample", NULL /*new sample callback here*/, NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, depayloader, decoder, converter, sink, NULL);

    if (!gst_element_link_many(source, depayloader, decoder, converter, sink, NULL)) {
        g_printerr("Pipeline elements cannot be linked!\n");
        gst_object_unref(GST_OBJECT(pipeline));
        return -1;
    }

    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, NULL /*bus callback here*/, NULL);
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

    this->loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(this->loop);

    gst_element_set_state(this->pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(this->pipeline));
    g_main_loop_unref(this->loop);
}

void GstControl::connectToStream() {
    gst_init(NULL, NULL);

    initPipeline();
    startPipeline();

    
}




GstControl::GstControl() {
    pipeline = nullptr;
    source = nullptr;
    converter = nullptr;
    depayloader = nullptr;
    decoder = nullptr;
    sink = nullptr;
    loop = nullptr;
}

GstControl::~GstControl() {

}