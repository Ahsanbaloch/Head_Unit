#include "mp3player.h"
#include <QImage>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

MP3Player::MP3Player(QObject *parent) : QObject(parent) {}

QString MP3Player::coverImagePath() const {
    return m_coverImagePath;
}

void MP3Player::loadMP3File(const QString &filePath) {
    extractCoverArt(filePath);
}


void MP3Player::extractCoverArt(const QString &filePath) {
    qDebug() << "Starting Python script to extract cover art";

    QString scriptPath = QCoreApplication::applicationDirPath() + "/extract_cover.py";
    qDebug() << "Python script path:" << scriptPath;
    qDebug() << "MP3 file path:" << filePath;

    // Python start
    QProcess process;
    process.start("python3", QStringList() << scriptPath << filePath);
    process.waitForFinished();  // wait until python end

    // check for the cover_image.jpg
    QString imagePath = QCoreApplication::applicationDirPath() + "/cover_image.jpg";
    if (QFile::exists(imagePath)) {
        qDebug() << "Cover image found and saved to:" << imagePath;

        // image load
        QImage coverImage(imagePath);
        if (!coverImage.isNull()) {
            QString previousImagePath = m_coverImagePath;

            // empty the path for qml to renew
            m_coverImagePath = "";
            qDebug()<<"empty previous Image path"<<m_coverImagePath<<" this";
            emit coverImagePathChanged();  // send signal

            m_coverImagePath = "file:///" + imagePath;
            qDebug() << "Cover image successfully loaded";
            emit coverImagePathChanged();
        } else {
            qDebug() << "Failed to load the cover image";
            m_coverImagePath = "qrc:/x.png";  // base path
            emit coverImagePathChanged();
        }
    } else {
        qDebug() << "Cover image not found, setting default image";
        m_coverImagePath = "qrc:/x.png";
        emit coverImagePathChanged();
    }


    emit coverImagePathChanged();
}
