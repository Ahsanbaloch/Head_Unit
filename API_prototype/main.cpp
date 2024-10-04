#include "mainwindow.h"

#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QFile>

QString loadHtmlFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open HTML file.");
        return QString();
    }

    QString html = QString::fromUtf8(file.readAll());
    return html;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Google Map Example");
    w.resize(800, 600);
    w.show();
/*
    QWebEngineView view;

    view.settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    //QString html = loadHtmlFile("/home/seame/Google_map_example/map.html");
    // HTML 내용을 C++ 문자열로 직접 포함하기
        QString html = R"(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset="UTF-8">
            <title>Google Map API</title>
            <style type="text/css">
                *{
                    margin: 5px;
                    padding: 5px;
                }
                #map{
                    border: 2px solid blue;
                    width: 500px;
                    height: 300px;
                }
            </style>
            <script src="http://maps.google.com/maps/api/js?key=AIzaSyBaXDM4by1bjTU8XnXAp_i3x9LUm1y6g2k&region=kr"></script>
            </head>

            <body>
                <div id="map"></div>
                <button id="btn1">Change 1</button>
                <button id="btn2">Change 2</button>
                <button id="btn3">Road View</button>

                <script type="text/javascript">

                    // 버튼들 불러와서 변수에 담아주고
                    var btn1 = document.getElementById("btn1");
                    var btn2 = document.getElementById("btn2");
                    var btn3 = document.getElementById("btn3");
                    // 각각 버튼에 이벤트 추가
                    btn1.addEventListener("click", changeMap1);
                    btn2.addEventListener("click", changeMap2);
                    btn3.addEventListener("click", changeMap3);


                    var map;
                    // 지도 출력
                    function initMap(){
                        // 위치데이터 경도, 위도로 구성된 jso 파일은 항상 이런식으로 구성되어있다.
                        var ll = {lat: 52.424427, lng: 10.792172};
                        map = new google.maps.Map(
                                document.getElementById("map"),
                                {zoom: 17, center: ll}
                                );
                        new google.maps.Marker(
                            {position: ll,
                                map: map,
                                label: "current position"}
                        );
                    }
                    initMap(); // 맵 생성

                    // 지도의 중심을 변경하는 작업
                    function changeMap1(){
                        var ll = {lat:52.426744, lng: 10.777209};
                        map.panTo(ll);
                        map.setZoom(16);
                    }
                    // 새로운 지도를 여는 작업 -> 마커 지워짐
                    function changeMap2(){
                        var ll = {lat:-40.339098, lng: 175.609729};
                        map = new google.maps.Map(
                                document.getElementById("map"),
                                {zoom:17, center: ll}
                                );
                    }
                    // 거리뷰를 새창으로 보여주는 작업
                    function changeMap3(){
                        window.open('pano.html','',width=300,height=300);
                    }
                </script>
            </body>
            </html>
        )";

    if (!html.isEmpty()) {
        view.setHtml(html);
        view.setWindowTitle("Google Map Example");
        view.resize(800, 600);
        view.show();
    }
//*/
    return a.exec();
}
