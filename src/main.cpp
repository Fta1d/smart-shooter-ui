#include "inc/appcontroller.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    
    AppController appController;
    
    appController.run();

    return app.exec();
}