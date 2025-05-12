#include "inc/logodetector.h"

LogoDetector::LogoDetector(QObject *parent) : QObject(parent) {
    running = false;
    threshold = 0.7;   
    logoSize = 25;       
    checkFrequency = 1;  

    QString appDir = QDir::currentPath();
    QString logoPath = appDir + "/../logo25.jpg";
    emit message("Current directory: " + appDir);
    emit message("Looking for logo at: " + logoPath);   

    QFile file(logoPath);
    if (file.exists()) {
        setLogoTemplate(logoPath);
    } else {
        emit message("Warning: Logo template file not found at " + logoPath);
    }
}

LogoDetector::~LogoDetector() {
    stopDetection();
}

cv::Mat LogoDetector::QImageToCvMat(const QImage &image) {
    QImage imgCopy;
    
    if (image.format() == QImage::Format_ARGB32 || 
        image.format() == QImage::Format_RGB32 ||
        image.format() == QImage::Format_ARGB32_Premultiplied) {
        imgCopy = image.convertToFormat(QImage::Format_RGB888);
    } else if (image.format() == QImage::Format_RGB888) {
        imgCopy = image;
    } else {
        imgCopy = image.convertToFormat(QImage::Format_RGB888);
    }
    
    return cv::Mat(
        imgCopy.height(), 
        imgCopy.width(), 
        CV_8UC3, 
        const_cast<uchar*>(imgCopy.bits()), 
        imgCopy.bytesPerLine()
    );
}

void LogoDetector::setLogoTemplate(const QString &logoPath) {
    cv::Mat logo = cv::imread(logoPath.toStdString());
    
    if (logo.empty()) {
        emit message("Error: Cannot load logo image from " + logoPath);
        return;
    }

    if (logoSize > 0 && (logo.cols != logoSize || logo.rows != logoSize)) {
        cv::resize(logo, logo, cv::Size(logoSize, logoSize));
    }

    cv::cvtColor(logo, logoTemplate, cv::COLOR_BGR2GRAY);
    
    emit message("Logo template loaded successfully. Size: " + 
                 QString::number(logoTemplate.cols) + "x" + 
                 QString::number(logoTemplate.rows));
}

void LogoDetector::startDetection() {
    if (logoTemplate.empty()) {
        emit message("Error: Logo template is not set");
        return;
    }
    
    running = true;
    emit message("Logo detection started");
}

void LogoDetector::stopDetection() {
    running = false;
    emit detectionStopped();
    emit message("Logo detection stopped");
}

void LogoDetector::processFrame(const QImage &frame) {
    if (!running || logoTemplate.empty()) {
        return;
    }
    
    QMutexLocker locker(&imageMutex);
    
    // static int frameCounter = 0;
    // frameCounter++;

    // if (frameCounter % checkFrequency != 0) {
    //     return;
    // }

    cv::Mat cvFrame = QImageToCvMat(frame);

    if (cvFrame.empty() || cvFrame.cols < logoSize || cvFrame.rows < logoSize) {
        return;
    }
    
    int roiWidth = logoSize * 2;
    int roiHeight = logoSize * 2;

    roiWidth = std::min(roiWidth, cvFrame.cols);
    roiHeight = std::min(roiHeight, cvFrame.rows);
    
    cv::Mat roi = cvFrame(cv::Rect(0, 0, roiWidth, roiHeight));

    cv::Mat grayRoi;
    cv::cvtColor(roi, grayRoi, cv::COLOR_BGR2GRAY);

    cv::Mat result;
    cv::matchTemplate(grayRoi, logoTemplate, result, cv::TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    if (maxVal >= threshold) {
        emit logoDetected();
    }
}


// #include "inc/logodetector.h"
// #include <QDebug>
// #include <QDir>

// LogoDetector::LogoDetector(QObject *parent) : QObject(parent) {
//     running = false;
//     threshold = 20.0;  
//     logoSize = 100;   
//     checkFrequency = 3;  
    
    
//     QTimer::singleShot(500, [this]() {
//         emit message("Logo detector running in MOTION DETECTION MODE");
//         emit message("Will detect motion in top-left corner");
//     });
// }

// LogoDetector::~LogoDetector() {
//     stopDetection();
// }

// cv::Mat LogoDetector::QImageToCvMat(const QImage &image) {
//     QImage imgCopy;
    
//     if (image.format() == QImage::Format_ARGB32 || 
//         image.format() == QImage::Format_RGB32 ||
//         image.format() == QImage::Format_ARGB32_Premultiplied) {
//         imgCopy = image.convertToFormat(QImage::Format_RGB888);
//     } else if (image.format() == QImage::Format_RGB888) {
//         imgCopy = image;
//     } else {
//         imgCopy = image.convertToFormat(QImage::Format_RGB888);
//     }
    
//     return cv::Mat(
//         imgCopy.height(), 
//         imgCopy.width(), 
//         CV_8UC3, 
//         const_cast<uchar*>(imgCopy.bits()), 
//         imgCopy.bytesPerLine()
//     );
// }

// void LogoDetector::startDetection() {
//     running = true;
//     emit message("Logo detection started (MOTION DETECTION MODE)");
// }

// void LogoDetector::stopDetection() {
//     running = false;
//     emit detectionStopped();
//     emit message("Logo detection stopped");
// }

// void LogoDetector::processFrame(const QImage &frame) {
//     if (!running) {
//         return;
//     }
    
//     QMutexLocker locker(&imageMutex);
    
//     static int frameCounter = 0;
//     frameCounter++;
    
    
//     if (frameCounter % checkFrequency != 0) {
//         return;
//     }
    
    
//     cv::Mat cvFrame = QImageToCvMat(frame);
    
    
//     if (cvFrame.empty() || cvFrame.cols < logoSize || cvFrame.rows < logoSize) {
//         return;
//     }
    
    
//     cv::Mat roi = cvFrame(cv::Rect(0, 0, logoSize, logoSize));
    
    
//     cv::Mat grayRoi;
//     cv::cvtColor(roi, grayRoi, cv::COLOR_BGR2GRAY);
    
    
//     cv::GaussianBlur(grayRoi, grayRoi, cv::Size(21, 21), 0);
    
    
//     static cv::Mat firstFrame;
//     if (firstFrame.empty()) {
//         grayRoi.copyTo(firstFrame);
//         return;
//     }
    
    
//     cv::Mat frameDelta;
//     cv::absdiff(firstFrame, grayRoi, frameDelta);
    
    
//     cv::Mat thresh;
//     cv::threshold(frameDelta, thresh, 25, 255, cv::THRESH_BINARY);
    
    
//     cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1,-1), 2);
    
    
//     std::vector<std::vector<cv::Point>> contours;
//     cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    
//     bool motionDetected = false;
//     for (const auto& contour : contours) {
        
//         double area = cv::contourArea(contour);
        
        
//         if (area < 500) continue;
        
//         motionDetected = true;
//         break;
//     }
    
   
//     double meanDiff = cv::mean(frameDelta)[0];
    
   
//     if (motionDetected || meanDiff > threshold) {
//         emit message(QString("Motion detected! Avg diff: %1, Contours: %2")
//                      .arg(meanDiff, 0, 'f', 1)
//                      .arg(contours.size()));
//         emit logoDetected();
        
        
//         grayRoi.copyTo(firstFrame);
//     }
    
    
    
//     static int frameUpdateCounter = 0;
//     frameUpdateCounter++;
//     if (frameUpdateCounter > 10) {
//         grayRoi.copyTo(firstFrame);
//         frameUpdateCounter = 0;
//     }
// }

// void LogoDetector::setLogoTemplate(const QString &logoPath) {
    
//     emit message("Motion detection mode active - template matching disabled");
// }