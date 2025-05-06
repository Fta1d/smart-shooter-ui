#include "inc/videolabel.h"

VideoLabel::VideoLabel() {
    vidStreamLabel = new QLabel(this);
    vidStreamLabel->setMinimumSize(640, 480);
    vidStreamLabel->setAlignment(Qt::AlignCenter);
    vidStreamLabel->setStyleSheet("background-color: black;");
    vidStreamLabel->setText("Waiting for stream...");
}   

VideoLabel::~VideoLabel()
{
}
