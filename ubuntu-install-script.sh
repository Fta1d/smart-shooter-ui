#!/bin/bash

# Скрипт для встановлення runtime пакетів GStreamer, Qt та msgpack для Ubuntu

echo "Оновлення пакетного менеджера..."
sudo apt update

echo "Встановлення runtime пакетів GStreamer..."
sudo apt install -y \
    libgstreamer1.0-0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    gstreamer1.0-qt6

echo "Встановлення runtime пакетів Qt6..."
sudo apt install -y \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6network6 \
    libqt6dbus6

echo "Встановлення runtime пакетів msgpack..."
sudo apt install -y \
    libmsgpackc2

echo "Встановлення runtime пакетів OpenCV..."
sudo apt install -y \
    libopencv-core4.5 \
    libopencv-imgproc4.5 \
    libopencv-highgui4.5 \
    libopencv-videoio4.5

echo "Готово! Всі необхідні runtime пакети було встановлено."
