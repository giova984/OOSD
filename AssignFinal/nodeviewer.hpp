#ifndef NODEVIEWER_HPP
#define NODEVIEWER_HPP

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>

#include <map>
#include <string>
#include <utility>

class QResizeEvent;

class NodeViewer :public QWidget
{
    Q_OBJECT

public:
    explicit NodeViewer( QWidget * parent = 0 );
    virtual ~NodeViewer();
//    void addNode(const std::string& name, const QPointF& pos);
//    void delNode(const std::string& name);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent (QResizeEvent * event );
private:

    double _actual_scale;

    std::string _point_selected;
    std::map<std::string, std::pair<QPointF, double>> _points;

    const static int DEFAULT_SCALE = 10;
    const static int DEFAULT_POINT_SIZE = 100;
    const static int DEFAULT_RADIUS_STROKE_SIZE = 2;

    QColor _screen_region_color;
    QColor _grid_color;
    QColor _color_point;
    QColor _color_radius;
    QColor _color_point_selected;
    QColor _color_radius_selected;

    QPen _pen;
    QBrush _brush;

    QRectF _screen_region;

    QFont _font;

    void drawScreen();
    void drawPoints();
    void drawGrid();

    void update_size(const QPointF& p);
    void recalculate_size();

signals:

public slots:
    void node_selected(const std::string& name);
    void node_created(const std::string& name, const QPointF& pos, double radius);
    void node_deleted(const std::string& name);
};

#endif // NODEVIEWER_HPP
