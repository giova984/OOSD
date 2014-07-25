#ifndef NODEVIEWER_HPP
#define NODEVIEWER_HPP

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>

class NodeViewer :public QWidget
{
    Q_OBJECT

public:
    explicit NodeViewer( QWidget * parent = 0 );
    virtual ~NodeViewer();
protected:
    void paintEvent(QPaintEvent *event);
private:

    int _point_selected;

    const static int DEFAULT_FONT_SIZE = 15;


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

signals:

public slots:

};

#endif // NODEVIEWER_HPP
