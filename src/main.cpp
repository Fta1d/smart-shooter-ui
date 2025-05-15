#include "inc/appcontroller.h"

void setStylesAndSheets(QApplication *app) {
    app->setStyle("Fusion");

    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#2B3339"));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor("#1F2529"));
    palette.setColor(QPalette::AlternateBase, QColor("#2B3339"));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor("#4A5660"));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor("#3B82F6"));
    palette.setColor(QPalette::Highlight, QColor("#3B82F6"));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    
    app->setPalette(palette);

    QString globalStyle = R"(
        QWidget {
            font-family: Arial, sans-serif;
            font-size: 14px;
        }
        
        QMainWindow {
            background-color: #2B3339;
        }
        
        /* Buttons */
        QPushButton {
            background-color: #4A5660;
            color: #FFFFFF;
            border: none;
            padding: 8px 16px;
            border-radius: 3px;
            min-height: 25px;
            font-weight: normal;
        }
        
        QPushButton:hover {
            background-color: #5A6770;
        }
        
        QPushButton:pressed {
            background-color: #3A4550;
        }
        
        QPushButton:checked {
            background-color: #3B82F6;
        }
        
        /* Line edits */
        QLineEdit {
            background-color: #4A5660;
            color: #FFFFFF;
            border: 1px solid #5A6770;
            padding: 6px;
            border-radius: 3px;
        }
        
        QLineEdit:focus {
            border: 1px solid #3B82F6;
        }
        
        QLineEdit:disabled {
            background-color: #3A4550;
            color: #888888;
        }
        
        /* Sliders */
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: #1F2529;
            margin: 0px;
            border-radius: 2px;
        }

        QSlider::sub-page:horizontal {
            background: #3B82F6;
            border: none;
            height: 4px;  
            border-radius: 2px;
        }

        QSlider::add-page:horizontal { 
            background: #1F2529;
            border: none;
            height: 4px;  
            border-radius: 2px;
        }

        QSlider::handle:horizontal {
            background: #3B82F6;
            border: none;
            width: 16px;
            height: 8px;
            margin: -4px 0; 
            border-radius: 2px;
        }

        QSlider::handle:horizontal:hover {
            background: #5B9AFF;
        }

        QSlider::handle:horizontal:pressed {
            background: #2872E6;
        }
        
        /* Text areas */
        QPlainTextEdit {
            background-color: #1F2529;
            color: #FFFFFF;
            border: 1px solid #4A5660;
            padding: 8px;
            font-family: 'Consolas', 'Monaco', monospace;
            selection-background-color: #3B82F6;
        }
        
        /* List widgets */
        QListWidget {
            background-color: #1F2529;
            color: #FFFFFF;
            border: 1px solid #4A5660;
            padding: 4px;
            outline: none;
        }
        
        QListWidget::item {
            padding: 4px;
            border-radius: 3px;
        }
        
        QListWidget::item:selected {
            background-color: #3B82F6;
        }
        
        QListWidget::item:hover {
            background-color: #4A5660;
        }
        
        /* Labels */
        QLabel {
            color: #FFFFFF;
            background-color: transparent;
        }
        
        /* Scroll bars */
        QScrollBar:vertical {
            background-color: #1F2529;
            width: 12px;
            margin: 0;
        }
        
        QScrollBar::handle:vertical {
            background-color: #4A5660;
            min-height: 20px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #5A6770;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            border: none;
            background: none;
        }
        
        QScrollBar:horizontal {
            background-color: #1F2529;
            height: 12px;
            margin: 0;
        }
        
        QScrollBar::handle:horizontal {
            background-color: #4A5660;
            min-width: 20px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #5A6770;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            border: none;
            background: none;
        }
    )";
    app->setStyleSheet(globalStyle);
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    setStylesAndSheets(&app);

    AppController appController;
    
    appController.run();

    return app.exec();
}