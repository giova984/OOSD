#include "nodeviewer.hpp"
#include <QPainter>
#include <QResizeEvent>

NodeViewer::NodeViewer(QWidget *parent) : QWidget(parent),
    _actual_scale(DEFAULT_SCALE),

    _screen_region(0,0,0,0),
    _screen_region_color(25, 150, 25, 100),
    _grid_color(0,0,0,0),

    _color_point(50,50,255,100),
    _color_radius(50,50,255,50),

    _color_point_selected(255,50,50,255),
    _color_radius_selected(255,50,50,200),
    _color_connection(200,0,0,150),
    _pen(),
    _brush(),
    _point_selected(),
    _points{},
    _connections{},
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
    drawConnections();
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

#define PIXEL_TO_WIDTH _screen_region.size().width() / _actual_scale
#define PIXEL_TO_HEIGHT _screen_region.size().height() / _actual_scale

void NodeViewer::drawPoints(){
    QPainter painter(this);

    painter.setPen(this->_pen);

    for (auto& p : _points){
        auto x =  p.second.first.x() * PIXEL_TO_WIDTH;
        auto y =  p.second.first.y() * PIXEL_TO_HEIGHT;

        auto rx =  p.second.second * PIXEL_TO_WIDTH;
        auto ry =  p.second.second * PIXEL_TO_HEIGHT;

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

void NodeViewer::drawConnections()
{
    QPainter painter(this);
    auto pen = QPen(_color_connection);
    pen.setWidthF(4.0);
    painter.setPen(pen);
    painter.setBrush(QBrush(_color_connection));
    for (auto& i : _connections){
        try{
            QPointF p1{  _points.at(i.first).first.x() * PIXEL_TO_WIDTH, _points.at(i.first).first.y() * PIXEL_TO_HEIGHT };
            QPointF p2{  _points.at(i.second).first.x() * PIXEL_TO_WIDTH, _points.at(i.second).first.y() * PIXEL_TO_HEIGHT };
            painter.drawLine(p1, p2);
        }catch(std::out_of_range){}
    }
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

void NodeViewer::node_selected(std::string name)
{
    _point_selected = name;
    this->repaint();
}

void NodeViewer::node_created(std::string name, QPointF pos, double radius)
{
    _points[name] = std::make_pair(pos, radius);
    update_size(pos);
    this->repaint();
}

void NodeViewer::node_deleted(std::string name)
{
    _points.erase(_points.find(name));
    recalculate_size();
    this->repaint();
}

void NodeViewer::connection_created(std::pair<std::string, std::string> conn)
{
    _connections.emplace_back(conn);
    this->repaint();
}

void NodeViewer::connection_deleted(std::pair<std::string, std::string> conn)
{
   // _connections.erase(std::find(std::begin(_connections), std::end(_connections), conn)); // c++14
    auto i = std::begin(_connections);
    while (i!= std::end(_connections))
        if(i->first == conn.first && i->second == conn.second)
            i = _connections.erase(i);
    else
            ++i;

    this->repaint();
}



