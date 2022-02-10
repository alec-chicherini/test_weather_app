#include "weatherchartwidget.h"

weatherChartWidget::weatherChartWidget(QWidget * parent):QChartView(parent)
{
    setChart(createLineChart());
    show();
}

QChart *weatherChartWidget::createLineChart() const
 {
     QChart *chart = new QChart();
     chart->setTitle("Temperature");
     return chart;
}

void weatherChartWidget::addNewTemperatureValueSlot(double value)
{
    listTemperature.push_back(value);
    if(listTemperature.size()>60)
    {
        listTemperature.pop_front();
    }
    emit updateLineChart();
}

void weatherChartWidget::updateLineChart()
{
    this->chart()->removeAllSeries();

    QLineSeries * series = new QLineSeries(this->chart());

    int i=0;
    foreach (double t, listTemperature)
    {
        series->append(i++,t);
    }
    this->chart()->addSeries(series);
    this->chart()->createDefaultAxes();

    QValueAxis *axisX = new QValueAxis(series);
    axisX->setLabelFormat("%i");
    this->chart()->setAxisX(axisX, series);
}
