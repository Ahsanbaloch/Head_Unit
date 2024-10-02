import QtQuick 2.12
import QtQuick.Controls 1.4
import QtMultimedia 5.12

ApplicationWindow {
    visible: true
    width: 400
    height: 400
    title: "MP3 Player"

    Column {
        anchors.centerIn: parent
        spacing: 20

        // mp3 file list
        ListView {
            id: mp3ListView
            width: 300
            height: 200
            model: ListModel {}

            delegate: Item {
                width: mp3ListView.width
                height: 50

                Row {
                    spacing: 10
                    Text {
                        text: modelData  // mp3 file path
                    }
                    Button {
                        text: "Play-"
                        onClicked: {
                            console.debug("MP3 File selected: " + modelData)
                            mp3Player.loadMP3File(modelData)
                            console.debug("loadMP3File")
                            //audioPlayer.source = modelData
                            audioPlayer.source = "file://" + modelData;

                            console.log("Audio source is: " + audioPlayer.source)
                            console.debug("source complete")
                            audioPlayer.play()
                        }
                    }
                }
            }
        }


        // cover image
        Image {
            id: coverImage
            width: 200
            height: 200
            cache: false
            fillMode: Image.PreserveAspectFit
            source: mp3Player.coverImagePath
            visible: mp3Player.coverImagePath !== ""
        }

        // audio play control
        Audio {
            id: audioPlayer
            autoPlay: false
            source: ""

            onStatusChanged: {
                if (status === Audio.EndOfMedia) {
                    audioPlayer.stop();
                }
            }
        }
        Connections {
              target: audioPlayer
              onSourceChanged: {
                  console.log("Audio source changed to: " + audioPlayer.source)
              }
        }

        // play and control
        Row {
            spacing: 10
            Button {
                text: "Play"
                onClicked: audioPlayer.play()
            }
            Button {
                text: "Pause"
                onClicked: audioPlayer.pause()
            }
            Button {
                text: "Stop"
                onClicked: audioPlayer.stop()
            }
        }
    }

    // update mp3 list from usb
    Connections {
        target: usbScanner
        onMp3FilesFound: {
            mp3ListView.model.clear()
            for (var i = 0; i < files.length; i++) {
                mp3ListView.model.append({"text": files[i]})
            }
        }
    }
}
