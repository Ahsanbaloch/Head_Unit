#ifndef USBSCANNER_H
#define USBSCANNER_H

#include <QObject>
#include <QStringList>
#include <QTimer>

class USBScanner : public QObject
{
    Q_OBJECT

public:
    explicit USBScanner(QObject *parent = nullptr);

    void startAutoScan();
    QStringList findMP3Files(const QString &path, int depth = 2);

signals:
    void mp3FilesFound(const QStringList &files);

private slots:
    void scanForUSBDevices();

private:
    QTimer *scanTimer;
    QString recentUSBPath;
    QStringList scanUSBDrives();
};

#endif // USBSCANNER_H
