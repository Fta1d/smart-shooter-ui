#include <QLabel>

class VideoLabel : public QWidget {
    Q_OBJECT

    private:
        

    public:
        QLabel *vidStreamLabel;

        VideoLabel();
        ~VideoLabel();
};

