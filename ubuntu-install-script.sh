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
    gstreamer1.0-libav

echo "Встановлення runtime пакетів Qt..."
sudo apt install -y \
    libqt5core5a \
    libqt5gui5 \
    libqt5widgets5 \
    libqt5network5 \
    libqt5dbus5

echo "Встановлення runtime пакетів msgpack..."
sudo apt install -y \
    libmsgpackc2

echo "Готово! Всі необхідні runtime пакети було встановлено."
