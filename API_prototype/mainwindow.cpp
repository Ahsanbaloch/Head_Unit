#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QMap>
#include <QDebug>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
{
    //ui->setupUi(this);

    view = new QWebEngineView(this);
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
                    width: 400px;
                    height: 240px;
                }
            </style>
            <script src="http://maps.google.com/maps/api/js?key={YOUR KEY}&region=kr"></script>
            </head>

            <body>
                <div id="map"></div>
                <button id="btn1">Change 1</button>
                <button id="btn2">Change 2</button>
                <button id="btn3">Road View</button>

                <script type="text/javascript">
                    var btn1 = document.getElementById("btn1");
                    var btn2 = document.getElementById("btn2");
                    var btn3 = document.getElementById("btn3");
                    btn1.addEventListener("click", changeMap1);
                    btn2.addEventListener("click", changeMap2);
                    btn3.addEventListener("click", changeMap3);

                    var map;
                    function initMap(){
                        var ll = {lat: 52.424427, lng: 10.792172};
                        map = new google.maps.Map(
                                document.getElementById("map"),
                                {zoom: 17, center: ll}
                                );
                        new google.maps.Marker(
                            {position: ll, map: map, label: "current position"}
                        );
                    }
                    initMap();
                    function changeMap1(){
                        var ll = {lat:52.426744, lng: 10.777209};
                        map.panTo(ll);
                        map.setZoom(16);
                    }
                    function changeMap2(){
                        var ll = {lat:-40.339098, lng: 175.609729};
                        map = new google.maps.Map(
                                document.getElementById("map"),
                                {zoom:17, center: ll}
                                );
                    }
                    function changeMap3(){
                        window.open('pano.html','',width=300,height=300);
                    }
                </script>
            </body>
            </html>
        )";

    // QWebEngineView에 HTML 설정
    view->setHtml(html);

    cityLineEdit = new QLineEdit(this);
    fetchButton = new QPushButton("Fetch Weather", this);
    weatherLabel = new QLabel("Weather info will be displayed here.", this);
    iconLabel = new QLabel(this);

    // 레이아웃 설정
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *inputLayout = new QHBoxLayout;

    mainLayout->addWidget(view, 3);

    // 입력창과 버튼을 수평으로 배치
    inputLayout->addWidget(cityLineEdit);
    inputLayout->addWidget(fetchButton);

    // 입력 폼과 레이블을 추가
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(weatherLabel, 1);
    mainLayout->addWidget(iconLabel, 1);

    // 중앙 위젯 설정
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 네트워크 요청 처리 연결
    connect(fetchButton, &QPushButton::clicked, this, &MainWindow::onFetchWeather);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onWeatherReply);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::onFetchWeather()
{
    // 입력된 도시 이름을 가져옴
    //QString city = ui->cityLineEdit->text();
    QString city = cityLineEdit->text();
    fetchWeatherData(city);
}

void MainWindow::fetchWeatherData(const QString &city)
{
    // OpenWeatherMap API 요청 URL
    QString apiKey = "{YOUR KEY}";  // OpenWeatherMap에서 발급받은 API 키를 입력
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                    .arg(city, apiKey);

    // 네트워크 요청
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    networkManager->get(request);  // 비동기 요청 실행
}

void MainWindow::onWeatherReply(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError) {
        // 응답 데이터를 받아서 JSON으로 파싱
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();

        // JSON 데이터에서 필요한 정보 추출
        QString cityName = jsonObj["name"].toString();
        double temperature = jsonObj["main"].toObject()["temp"].toDouble();
        QString weatherDescription = jsonObj["weather"].toArray()[0].toObject()["description"].toString();
        QString iconCode = jsonObj["weather"].toArray()[0].toObject()["icon"].toString();

        // 아이콘 코드 검증
        if (iconCode.isEmpty()) {
            qDebug() << "Icon code is empty.";
            QMessageBox::warning(this, "Error", "Icon code is not available.");
            return; // 아이콘 코드가 없으면 함수 종료
        }

        // 아이콘 클래스 매핑
        QMap<QString, QString> weatherIconMap = {
            {"01", "/home/seame/API_prototype/images/sunny.png"}, //"fas fa-sun"},
            {"02", "/home/seame/API_prototype/images/cloud_sun.png"}, //"fas fa-cloud-sun"},
            {"03", "/home/seame/API_prototype/images/cloud.png"}, //"fas fa-cloud"},
            {"04", "/home/seame/API_prototype/images/cloud_meatball.png"}, //"fas fa-cloud-meatball"},
            {"09", "/home/seame/API_prototype/images/cloud_sun_rain.png"}, //"fas fa-cloud-sun-rain"},
            {"10", "/home/seame/API_prototype/images/rain_heavy.png"}, //"fas fa-cloud-showers-heavy"},
            {"11", "/home/seame/API_prototype/images/strom.png"}, //"fas fa-poo-storm"},
            {"13", "/home/seame/API_prototype/images/snow.png"}, //"far fa-snowflake"},
            {"50", "/home/seame/API_prototype/images/smog.png"}, //"fas fa-smog"}
        };

        /*// 아이콘 URL 생성
        QString iconUrl = QString("http://openweathermap.org/img/wn/%1@2x.png").arg(iconCode);
        qDebug() << "Icon Request URL:" << iconUrl;//*/

        // 아이콘 이미지 경로 가져오기
        QString iconPath = weatherIconMap.value(iconCode.left(2), "/home/seame/API_prototype/images/sunny.png"); // 기본 이미지 설정

        //QString iconClass = weatherIconMap.value(iconCode.left(2), ""); // 기본값은 빈 문자열
        //QString iconHtml = QString("<i class='%1'></i>").arg(iconClass);

        // 결과를 레이블에 표시
        QString result = QString("City: %1\nTemperature: %2°C\nDescription: %3")
                        .arg(cityName)
                        .arg(temperature)
                        .arg(weatherDescription);
                        //.arg(iconHtml);

        weatherLabel->setText(result);

        QPixmap icon(iconPath);
        if (!icon.isNull()) {
            iconLabel->setPixmap(icon.scaled(80, 80, Qt::KeepAspectRatio)); // 이미지 크기 조정
        } else {
            qDebug() << "Failed to load icon image from path:" << iconPath;
        }

    } else {
        // 에러 메시지 박스 표시
        qDebug() << "Weather Data Request Error:" << reply->errorString();
        QMessageBox::warning(this, "Error", "Failed to fetch weather data.");
    }

    reply->deleteLater();  // 네트워크 응답 객체 삭제
}

