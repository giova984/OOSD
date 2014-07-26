#ifndef PLOTRESULTS_HPP
#define PLOTRESULTS_HPP

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class PlotResults;
}

class PlotResults : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotResults(QWidget *parent = 0);
    ~PlotResults();

    void setdata(const std::vector<double>& key, std::vector<double>& value);
    void setdata(const std::vector<std::pair<double, double>>& data);

private:
    Ui::PlotResults *ui;

    QVector<double> _key;
    QVector<double> _value;
};

#endif // PLOTRESULTS_HPP
