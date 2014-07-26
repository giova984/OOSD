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

private:
    Ui::PlotResults *ui;
};

#endif // PLOTRESULTS_HPP
