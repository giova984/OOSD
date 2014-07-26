#include "plotresults.hpp"
#include "ui_plotresults.h"
#include <vector>

PlotResults::PlotResults(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotResults),
    _key{},
    _value{}
{
    ui->setupUi(this);

    setWindowTitle("Simulation Results");
    // create graph and assign data to it:
    ui->customPlot->addGraph();

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Send period (Ticks)");
    ui->customPlot->yAxis->setLabel("collisions (link level)");
}

PlotResults::~PlotResults()
{
    delete ui;
}

void PlotResults::setdata(const std::vector<double> &key, std::vector<double> &value)
{
    _key = QVector<double>::fromStdVector(key);
    _value = QVector<double>::fromStdVector(value);
    ui->customPlot->graph(0)->setData(_key, _value);
    ui->customPlot->graph(0)->rescaleAxes(true);

    ui->customPlot->replot();
}

void PlotResults::setdata(const std::vector<std::pair<double, double> > &data)
{
    _key.clear();
    _value.clear();
    for (auto& v : data){
        _key.push_back(v.first);
        _value.push_back(v.second);
    }
    ui->customPlot->graph(0)->setData(_key, _value);
    ui->customPlot->graph(0)->rescaleAxes(true);

    ui->customPlot->replot();
}
