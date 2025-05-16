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
    vidStreamLabel->setMouseTracking(true);
    vidStreamLabel->installEventFilter(this);
}   

bool VideoLabel::eventFilter(QObject *watched, QEvent *event) {
    if (watched == vidStreamLabel && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        
        if (!gstreamer || vidStreamLabel->pixmap().isNull()) {
            return false;
        }
        
        QPoint pos = mouseEvent->pos();
        
        QPixmap pixmap = vidStreamLabel->pixmap();
        QRect targetRect = pixmap.rect();
        
        targetRect.setSize(targetRect.size().scaled(vidStreamLabel->size(), Qt::KeepAspectRatio));
        
        QPoint topLeft(
            (vidStreamLabel->width() - targetRect.width()) / 2,
            (vidStreamLabel->height() - targetRect.height()) / 2
        );
        
        targetRect.moveTopLeft(topLeft);
        
        if (!targetRect.contains(pos)) {
            return false;
        }
        
        int scaledX = ((pos.x() - targetRect.left()) * 640) / targetRect.width();
        int scaledY = ((pos.y() - targetRect.top()) * 640) / targetRect.height();
        
        scaledX = qBound(0, scaledX, 639);
        scaledY = qBound(0, scaledY, 639);

        // qDebug() << "Click position: Original = (" << pos.x() << "," << pos.y() 
        //  << "), Final = (" << scaledX << "," << scaledY << ")";
        
        emit videoLabelClicked(scaledX, scaledY);
        return true;
    }
    
    return QWidget::eventFilter(watched, event);
}

VideoLabel::~VideoLabel()
{
}

void VideoLabel::setGstreamer(GstControl *gst) {
    gstreamer = gst;
}
