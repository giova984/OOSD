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
    // add two new graphs and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
//    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Send period (Ticks)");
    ui->customPlot->yAxis->setLabel("link level: collisions(blue), lost messages (red)");
}

PlotResults::~PlotResults()
{
    delete ui;
}

void PlotResults::setdata(unsigned int graph, const std::vector<std::pair<double, double> > &data)
{
    if (graph > ui->customPlot->graphCount())
        return;

    _key.clear();
    _value.clear();
    for (auto& v : data){
        _key.push_back(v.first);
        _value.push_back(v.second);
    }
    ui->customPlot->graph(graph)->setData(_key, _value);
    ui->customPlot->graph(graph)->rescaleAxes(true);

    ui->customPlot->replot();
}
