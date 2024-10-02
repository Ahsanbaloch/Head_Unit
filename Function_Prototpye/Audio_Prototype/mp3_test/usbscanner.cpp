#include "usbscanner.h"
#include <QStorageInfo>
#include <QDirIterator>
#include <QDebug>

USBScanner::USBScanner(QObject *parent) : QObject(parent)
{
    scanTimer = new QTimer(this);
    connect(scanTimer, &QTimer::timeout, this, &USBScanner::scanForUSBDevices);
}

void USBScanner::startAutoScan()
{
    scanTimer->start(2000);
}

void USBScanner::scanForUSBDevices()
{
    QStringList usbDrives = scanUSBDrives();
    if (!usbDrives.isEmpty()) {
        // save the latest usb path
        recentUSBPath = usbDrives.last();
        //qDebug() << "Most recent USB detected at:" << recentUSBPath;

        // check mp3 file (depth 2)
        QStringList mp3Files = findMP3Files(recentUSBPath, 2);
        if (!mp3Files.isEmpty()) {
            emit mp3FilesFound(mp3Files);  // if find mp3, send signal to qml
        }
    } else {
        recentUSBPath.clear();  // if no usb
    }
}

QStringList USBScanner::scanUSBDrives()
{
    QStringList usbDrives;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady() && !storage.isReadOnly()) {
            usbDrives << storage.rootPath();
        }
    }
    return usbDrives;
}

QStringList USBScanner::findMP3Files(const QString &path, int depth)
{
    QStringList mp3Files;
    QDirIterator it(path, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        int currentDepth = filePath.count('/') - path.count('/');

        // find mp3 files (Depth 2)
        if (currentDepth <= depth) {
            mp3Files << filePath;
        }
    }
    return mp3Files;
}
