#include "nodeviewer.hpp"

NodeViewer::NodeViewer(QWidget *parent) : QWidget(parent) //QGraphicsView(parent)
{
   // this->_screen_region = QRect(0,0,400,300);

    this->_screen_region_color = QColor::fromRgb(80, 220, 110);
    this->_grid_color = QColor::fromRgb(118, 118, 118);
    this->_color_point = QColor::fromRgb(255, 50, 0);

    this->_pen_screen = QPen(this->_screen_region_color);
    this->_brush_screen = QBrush(this->_screen_region_color);

    this->_pen_point = QPen(this->_color_point);
    this->_brush_point = QBrush(this->_color_point);

    this->_pen_grid = QPen(this->_grid_color);
    this->_brush_grid = QBrush(this->_grid_color);

    this->_pen_text = QPen(this->_color_text);

    this->_font = QFont();
    this->_font.setPointSize(DEFAULT_FONT_SIZE);
    //this->_font.setBold(true);

    setBackgroundRole(QPalette::Base);
    //setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));

    setAutoFillBackground(true);
}

NodeViewer::~NodeViewer()
{

}

void NodeViewer::paintEvent(QPaintEvent *event)
{

}
