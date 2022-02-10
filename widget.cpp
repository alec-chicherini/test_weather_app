#include "widget.h"
#include "ui_widget.h"
#include "QTableWidget"
#include <QTimer>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      temperatureModifier(0.0)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    initColumnTable();
    setLayout(ui->gridLayout);

    QTimer* updateTimer = new QTimer(this);
    connect (updateTimer, &QTimer::timeout, this,[&]{
        emit getOpenWeatherAPICall();
    });
    updateTimer->start(5 * 1000 /* *60*/ );//обновляется каждые 5 секунд

    connect(this, &Widget::addNewTemperatureValueSignal, ui->chart, &weatherChartWidget::addNewTemperatureValueSlot );

    //Кнопки для дебага графика
    ui->btnTempUp->setText(tr("Температура + 1")+QChar(0x2103));
    ui->btnTempDown->setText(tr("Температура - 1")+QChar(0x2103));
    ui->label->setText("0.0");
    connect(ui->btnTempUp,   &QPushButton::clicked, this, [&]{
        temperatureModifier+=1.0;
        ui->label->setText(QString::number(temperatureModifier));
    });
    connect(ui->btnTempDown, &QPushButton::clicked, this, [&]{
        temperatureModifier-=1.0;
        ui->label->setText(QString::number(temperatureModifier));
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addColumnTable(double temperature, int humidity, int cloudiness, int precipitation, QString time)
{
    ui->table->insertColumn(1);

    QTableWidgetItem *dateItem = new QTableWidgetItem(tr("%1").arg(time));
    ui->table->setItem(0, 1, dateItem);
    QTableWidgetItem *temperatureItem = new QTableWidgetItem(tr("%1").arg(QString::number(temperature))+QChar(0x2103));
    ui->table->setItem(1, 1, temperatureItem);
    QTableWidgetItem *humidityItem = new QTableWidgetItem(tr("%1%").arg(QString::number(humidity)));
    ui->table->setItem(2, 1, humidityItem);
    QTableWidgetItem *cloudinessItem = new QTableWidgetItem(tr("%1%").arg(QString::number(cloudiness)));
    ui->table->setItem(3, 1, cloudinessItem);
    QTableWidgetItem *precipitationItem = new QTableWidgetItem(tr("%1mm").arg(QString::number(precipitation)));
    ui->table->setItem(4, 1, precipitationItem);
}

void Widget::getOpenWeatherAPICall()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, [&](QNetworkReply *r){
        const QByteArray reply = r->readAll();
        QJsonParseError *error = new QJsonParseError();
        QJsonDocument doc  = QJsonDocument::fromJson(reply,error);
        if(error->error != QJsonParseError::NoError){
            qDebug()<<"ERROR:"<<error->errorString();
        }
        int temperature   = temperatureModifier + ((QJsonValue)doc.object()["main"])["temp"].toDouble();
        int humidity      = ((QJsonValue)doc.object()["main"])["humidity"].toInt();
        int cloudiness    = ((QJsonValue)doc.object()["clouds"])["all"].toInt();
        int precipitation = ((QJsonValue)doc.object()["snow"])["3h"].toInt() +
                            ((QJsonValue)doc.object()["rain"])["3h"].toInt();
        emit addColumnTable(temperature,  humidity,  cloudiness,  precipitation, QDateTime::currentDateTime().toString("dd.MM.yy hh:mm"));
        emit addNewTemperatureValueSignal(temperature);
    });

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openweathermap.org/data/2.5/weather?q=Krasnoznamensk,ru&APPID=b151af2dc53af567cd9033fa149cf30a&units=metric"));
    manager->get(request);
}

void Widget::initColumnTable()
{
    ui->table->setRowCount(5);
    ui->table->setColumnCount(1);
    QTableWidgetItem *dateItem = new QTableWidgetItem(tr("Время"));
    ui->table->setItem(0, 0, dateItem);
    QTableWidgetItem *temperatureItem = new QTableWidgetItem(tr("Температура:"));
    ui->table->setItem(1, 0, temperatureItem);
    QTableWidgetItem *humidityItem = new QTableWidgetItem(tr("Влажность:"));
    ui->table->setItem(2, 0, humidityItem);
    QTableWidgetItem *cloudinessItem = new QTableWidgetItem(tr("Облачность:"));
    ui->table->setItem(3, 0, cloudinessItem);
    QTableWidgetItem *precipitationItem = new QTableWidgetItem(tr("Осадки:"));
    ui->table->setItem(4, 0, precipitationItem);
}

