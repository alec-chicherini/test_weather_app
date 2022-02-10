#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QWidget;
///Основной виджет приложения.
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    ///Добавить в таблицу слева новый столбец с данными
    void addColumnTable(double temperature, int humidity, int cloudiness, int precipitation, QString time);
    ///Cделать HTTP запрос с данными о погоде на сайт openweathermap.org
    void getOpenWeatherAPICall();
signals:
    void addNewTemperatureValueSignal(double);
private:
    Ui::Widget *ui;
    void initColumnTable();
    ///инкремент значения температуры чтобы посмотреть, как будет менятся график температуры.
    double temperatureModifier;
};
#endif // WIDGET_H
