#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class GstControl;

class VideoLabel : public QWidget {
    Q_OBJECT

    public slots:
        void updateFrame();

    signals:
        void videoLabelClicked(int x, int y);

    private:
        GstControl *gstreamer;

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

    public:
        QLabel *vidStreamLabel;
        void setGstreamer(GstControl *gst);

        VideoLabel();
        ~VideoLabel();
};

#endif // VIDEOLABEL_H
