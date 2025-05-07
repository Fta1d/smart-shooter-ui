#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>

class GstControl;

class VideoLabel : public QWidget {
    Q_OBJECT

    public slots:
        void updateFrame();

    private:
        GstControl *gstreamer;

    public:
        QLabel *vidStreamLabel;
        void setGstreamer(GstControl *gst);

        VideoLabel();
        ~VideoLabel();
};

#endif // VIDEOLABEL_H
