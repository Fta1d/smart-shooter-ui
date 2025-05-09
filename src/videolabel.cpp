#include "inc/videolabel.h"
#include "inc/gstcontrol.h"

void VideoLabel::updateFrame() {
    if (gstreamer) {
        QPixmap frame = gstreamer->getFramePixmap();
        if (!frame.isNull()) {
            vidStreamLabel->setPixmap(frame.scaled(vidStreamLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        this->update();
    } else {
        qDebug() << "gstreamer is null!";
    }
}

VideoLabel::VideoLabel() {
    vidStreamLabel = new QLabel(this);
    vidStreamLabel->setMinimumSize(800, 600);
    vidStreamLabel->setAlignment(Qt::AlignCenter);
    vidStreamLabel->setStyleSheet("background-color: black;");
    vidStreamLabel->setText("Waiting for stream...");
}   

VideoLabel::~VideoLabel()
{
}

void VideoLabel::setGstreamer(GstControl *gst) {
    gstreamer = gst;
}
