QT += core gui widgets network

CONFIG += c++17

TARGET = smart-shooter-ui
TEMPLATE = app

# Шлях до GStreamer з пробілами потребує лапок
GSTREAMER_DIR = "C:\\Program Files\\gstreamer\\1.0\\mingw_x86_64"
MSGPACK_DIR = "C:\\msys64\\mingw64"


# Шляхи до заголовків GStreamer
INCLUDEPATH += "$${MSGPACK_DIR}\\include"
INCLUDEPATH += "$${MSGPACK_DIR}\\include\\msgpack"
INCLUDEPATH += "$${GSTREAMER_DIR}\\include"
INCLUDEPATH += "$${GSTREAMER_DIR}\\include\\gstreamer-1.0"
INCLUDEPATH += "$${GSTREAMER_DIR}\\include\\glib-2.0"
INCLUDEPATH += "$${GSTREAMER_DIR}\\lib\\glib-2.0\\include"

# Виведення для відладки
message("GSTREAMER_DIR: $${GSTREAMER_DIR}")
message("Include paths:")
for(path, INCLUDEPATH) {
    message(" - $${path}")
}

# Вказуємо шляхи до вихідних файлів
SOURCES += \
    src\\appcontroller.cpp \
    src\\gstcontrol.cpp \
    src\\main.cpp \
    src\\mainwindow.cpp \
    src\\udpcommandsender.cpp \
    src\\videolabel.cpp

HEADERS += \
    inc\\appcontroller.h \
    inc\\gstcontrol.h \
    inc\\mainwindow.h \
    inc\\udpcommandsender.h \
    inc\\videolabel.h

# Шлях до власних заголовків
INCLUDEPATH += inc\\

# Шляхи до бібліотек GStreamer
LIBS += -L"$${GSTREAMER_DIR}\\lib" \
    -lgstreamer-1.0 \
    -lgstapp-1.0 \
    -lgstvideo-1.0 \
    -lglib-2.0 \
    -lgobject-2.0 \

# Додаткові директорії для проміжних файлів
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui
RCC_DIR = rcc

# Додайте це для кращого виявлення помилок
CONFIG += console
