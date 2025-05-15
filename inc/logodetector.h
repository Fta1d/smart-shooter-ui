#ifndef LOGODETECTOR_H
#define LOGODETECTOR_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QDir>
#include <opencv2/opencv.hpp>

class LogoDetector : public QObject {
    Q_OBJECT

private:
    mutable QMutex imageMutex;
    QImage currentFrame;
    cv::Mat logoTemplate;
    bool running;
    double threshold;
    int logoSize;
    int checkFrequency; 

    cv::Mat QImageToCvMat(const QImage &image);

public:
    LogoDetector(QObject *parent = nullptr);
    ~LogoDetector();

public slots:
    void startDetection();
    void stopDetection();
    void processFrame(const QImage &frame);
    void setLogoTemplate(const QString &logoPath);

signals:
    void logoDetected();
    void detectionStopped();
    void message(const QString &text);
};

#endif // LOGODETECTOR_H