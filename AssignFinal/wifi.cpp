#include "wifi.hpp"
#include "ui_wifi.h"

#include <algorithm>
#include <simul.hpp>
#include <memory>
#include <string>

#include "link.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"

#include <qdebug.h>
#include <QListWidgetItem>
#include <QMessageBox>

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi),
    _link{"Channel_1"},
    _routing_table{}
{
    ui->setupUi(this);

    ui->viewer_layout->addWidget(&_viewer);
    connect( ui->node_list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(on_item_selected(QListWidgetItem *)));
    //Node Viewer connections
    connect( this, SIGNAL(on_node_created(std::string, QPointF, double)), &this->_viewer, SLOT(node_created(const std::string&, const QPointF&, double)));
    connect( this, SIGNAL(on_node_deleted(std::string)), &this->_viewer, SLOT(node_deleted(const std::string&)));
    connect( this, SIGNAL(on_node_selected(std::string)), &this->_viewer, SLOT(node_selected(std::string)));


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
        std::string name = ui->node_name_text->text().toUtf8().data();
        QPointF pos{ui->spin_x->value(), ui->spin_y->value()};
        double radius = ui->spin_radius->value();

        if(_nodes.find(name) == _nodes.end() ){
            new QListWidgetItem(ui->node_name_text->text(), ui->node_list);
            //Node-Interface creation
            _nodes[name] = std::unique_ptr<Node>(new Node(name));
            _interfaces[name] = std::unique_ptr<WifiInterface>(new WifiInterface(
                                                                   std::string("interface").append(name),
                                                                   *_nodes[name],
                                                                   make_pair(pos.x(),pos.y()),
                                                                   radius,
                                                                   _link,
                                                                   &_routing_table) );

            emit on_node_created( name, pos, radius );
        }else{
            QMessageBox msgBox;
            msgBox.setText("The node already exists");
            msgBox.exec();
        }
    }
}

void wifi::on_del_btn_clicked()
{
    if(ui->node_list->selectedItems().size() > 0){
        std::string name = ui->node_list->selectedItems().front()->text().toUtf8().data();
        _nodes.erase(_nodes.find((name)));
        delete ui->node_list->selectedItems().front();
        emit on_node_deleted(name);
    }
}


void wifi::on_item_selected( QListWidgetItem *item)
{
    emit on_node_selected(item->text().toUtf8().data());
    //qDebug( "selected ");
    //qDebug( item->text().toUtf8().data());
    //const Node *node = _nodes
}
