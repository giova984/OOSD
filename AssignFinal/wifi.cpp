#include "wifi.hpp"
#include "ui_wifi.h"

#include <algorithm>
#include <simul.hpp>
#include <memory>

#include "link.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"

#include <qdebug.h>

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi),
    link{"Channel_1"}
{
    ui->setupUi(this);

    ui->node_list->setColumnWidth(0,60);

    _viewer.setScene(&_scene);
    ui->viewer_layout->addWidget(&_viewer);

}

wifi::~wifi()
{
    delete ui;
}

void wifi::on_actionLoad_triggered()
{

}

void wifi::on_actionSave_triggered()
{

}

void wifi::on_actionRun_triggered()
{

}

void wifi::on_actionShow_Results_triggered()
{

}

void wifi::on_abb_btn_clicked()
{
    if (ui->node_name_text->text() != "" ){
            for (int i=0; i<ui->node_list->rowCount(); ++i){
                auto a{ui->node_list->itemAt(0,i)};
                //<< " - " << ui->node_name_text->text().toUtf8().data() << std::endl;
                if(ui->node_list->itemAt(0,i)->text() == ui->node_name_text->text())
                    return;
            }
            _nodes.emplace_back(new Node(ui->node_name_text->text().toUtf8().data()));
            ui->node_list->insertRow(ui->node_list->rowCount() );
            int row = ui->node_list->rowCount();
            __items.emplace_back(new QTableWidgetItem( ui->node_name_text->text()));
            ui->node_list->setItem(row,0,__items.back().get());
            __items.emplace_back(new QTableWidgetItem( ui->spin_x->text()));
            ui->node_list->setItem(row,1,__items.back().get());
            __items.emplace_back(new QTableWidgetItem( ui->spin_x->text()));
            ui->node_list->setItem(row,2,__items.back().get());
            __items.emplace_back(new QTableWidgetItem( ui->spin_radius->text() ));
            ui->node_list->setItem(row,3,__items.back().get());
    }


}

void wifi::on_del_btn_clicked()
{

}


NodeViewer::NodeViewer(QWidget *parent) : QGraphicsView(parent)
{
    this->_screen_region = QRect(0,0,0,0);

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
    setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));
}

NodeViewer::~NodeViewer()
{

}

void NodeViewer::paintEvent(QPaintEvent *event)
{

}
