#ifndef WEATHERCHARTWIDGET_H
#define WEATHERCHARTWIDGET_H
#include <QObject>
#include <QChartView>
#include <QChart>
#include <QtCharts>
#include <QLineSeries>
#include <QValueAxis>

///Виджет для отображения графика
class weatherChartWidget : public QChartView
{
    Q_OBJECT
public:
    weatherChartWidget(QWidget * parent);
    ///функция для создания виджета температуры
    QChart* createLineChart() const;

private:
    ///список значений температуры для графика
    QList<double> listTemperature;

public slots:
    ///слот добавляющий значение в список, и удаляющий первое если значений стало больше 60
    void addNewTemperatureValueSlot(double);

private slots:
    ///перерисовывает график с новыми значениями
    void updateLineChart();
};

#endif // WEATHERCHARTWIDGET_H
