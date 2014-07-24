#ifndef WIFI_HPP
#define WIFI_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>

#include <memory>
#include <vector>

#include "link.hpp"
#include "routing.hpp"

#define DEFAULT_FONT_SIZE 15

class QTableWidgetItem;

namespace Ui {
class wifi;
}

class Node;
class WifiInterface;

class NodeViewer :public QGraphicsView
{
    Q_OBJECT

public:
    explicit NodeViewer( QWidget * parent = 0 );
    virtual ~NodeViewer();
protected:
    void paintEvent(QPaintEvent *event);
private:

    int _point_selected;

    QPen _pen_text;
    QPen _pen_screen;
    QPen _pen_point;
    QPen _pen_grid;

    QBrush _brush_screen;
    QBrush _brush_point;
    QBrush _brush_grid;

    std::vector<QPointF> _points;
    QRectF _screen_region;

    QColor _screen_region_color;
    QColor _grid_color;
    QColor _color_point;
    QColor _color_text;

    QFont _font;

    void drawScreen();
    void drawPoints();
    void drawGrid();

};

class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

private slots:
    void on_actionLoad_triggered();

    void on_actionRun_triggered();

    void on_actionSave_triggered();

    void on_actionShow_Results_triggered();

    void on_abb_btn_clicked();

    void on_del_btn_clicked();

private:
    Ui::wifi *ui;
    QGraphicsScene _scene;
    NodeViewer _viewer;

    WifiLink link;
    WifiRoutingTable routing_table;
    std::vector<std::unique_ptr<Node>> _nodes;
    std::unique_ptr<WifiInterface> _interfaces;

    std::vector<std::unique_ptr<QTableWidgetItem>> __items;
};

#endif // WIFI_HPP
