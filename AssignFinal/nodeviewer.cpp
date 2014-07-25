#include "nodeviewer.hpp"
#include <QPainter>
#include <QResizeEvent>

NodeViewer::NodeViewer(QWidget *parent) : QWidget(parent),
    _actual_scale(DEFAULT_SCALE),

    _screen_region(0,0,0,0),
    _screen_region_color(Qt::lightGray),
    _grid_color(0,0,0,0),

    _color_point(50,50,255,100),
    _color_radius(50,50,255,50),

    _color_point_selected(255,50,50,255),
    _color_radius_selected(255,50,50,200),
    _pen(),
    _brush(),
    _point_selected(),
    _font()
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    _brush.setStyle(Qt::SolidPattern);
}

NodeViewer::~NodeViewer()
{

}

void NodeViewer::paintEvent(QPaintEvent *event)
{
    drawScreen();
    drawPoints();
}

void NodeViewer::resizeEvent(QResizeEvent *event)
{
    this->_screen_region.setSize(event->size());
}

void NodeViewer::drawScreen(){
    QPainter painter(this);
    painter.setBrush(QBrush(_screen_region_color));
    painter.drawRect(this->_screen_region);
}

void NodeViewer::drawPoints(){
    QPainter painter(this);

    painter.setPen(this->_pen);

    for (auto p : _points){
        auto x =  p.second.first.x() / _actual_scale * _screen_region.size().width();
        auto y =  p.second.first.y() / _actual_scale * _screen_region.size().height();

        auto rx =  p.second.second / _actual_scale * _screen_region.size().width();
        auto ry =  p.second.second / _actual_scale * _screen_region.size().height();

        if(p.first == _point_selected){
            this->_brush.setColor(_color_point_selected);
            painter.setBrush(QBrush(_color_point_selected));
            painter.drawEllipse(QPointF(x,y), DEFAULT_POINT_SIZE/ _actual_scale, DEFAULT_POINT_SIZE/ _actual_scale);
            painter.setBrush(QBrush(_color_radius_selected));
            painter.drawEllipse(QPointF(x,y), rx, ry);
        }else{
            painter.setBrush(QBrush(_color_point));
            painter.drawEllipse(QPointF(x,y), DEFAULT_POINT_SIZE/ _actual_scale, DEFAULT_POINT_SIZE/ _actual_scale);
            painter.setBrush(QBrush(_color_radius));
            painter.drawEllipse(QPointF(x,y), rx, ry);
        }
    }
}

void NodeViewer::drawGrid(){
//    QPainter grid_painter(this);

//    grid_painter.setPen(this->_pen_grid);

//    for(int i = 0; i < NUM_CALIBRATION_QUADS+1; i++){
//        for(int j = 0; j < NUM_CALIBRATION_QUADS+1; j++){
//            //draw_line_horiz
//            if(j < NUM_CALIBRATION_QUADS)
//                grid_painter.drawLine(this->_points[(i* (NUM_CALIBRATION_QUADS+1))+j], this->_points[(i* (NUM_CALIBRATION_QUADS+1))+j+1]);

//            //draw_line_vert
//            if(i < NUM_CALIBRATION_QUADS)
//                grid_painter.drawLine(this->_points[(i* (NUM_CALIBRATION_QUADS+1))+j], this->_points[((i+1)* (NUM_CALIBRATION_QUADS+1))+j]);
//        }
    //    }
}

void NodeViewer::update_size(const QPointF &p)
{
    if (p.x() > _actual_scale)
        _actual_scale = p.x();
    if (p.y() > _actual_scale)
        _actual_scale = p.y();
}

void NodeViewer::recalculate_size()
{
    _actual_scale = DEFAULT_SCALE;
    for (auto p : _points){
       update_size(p.second.first);
    }
}

void NodeViewer::node_selected(const std::string &name)
{
    _point_selected = name;
    this->repaint();
}

void NodeViewer::node_created(const std::string &name, const QPointF &pos, double radius)
{
    _points[name] = std::make_pair(pos, radius);
    update_size(pos);
    this->repaint();
}

void NodeViewer::node_deleted(const std::string &name)
{
    _points.erase(_points.find(name));
    recalculate_size();
    this->repaint();
}



