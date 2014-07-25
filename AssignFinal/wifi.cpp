#include "wifi.hpp"
#include "ui_wifi.h"

#include "simul.hpp"

#include <algorithm>
#include <memory>
#include <string>

#include "link.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"

#include <qdebug.h>
#include <QListWidgetItem>
#include <QMessageBox>

using namespace MetaSim;

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui{std::unique_ptr<Ui::wifi>(new Ui::wifi)},
    _link{"Channel_1"},
    _routing_table{},
    _random{std::unique_ptr<RandomVar>(new MetaSim::UniformVar(1,100))},
    _connections{}
{
    ui->setupUi(this);

    ui->viewer_layout->addWidget(&_viewer);
    //WList of nodes widget connection
    connect( ui->node_list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(on_item_selected(QListWidgetItem *)));
    //Node Viewer connections
    connect( this, SIGNAL(on_node_created(std::string, QPointF, double)), &this->_viewer, SLOT(node_created(std::string, QPointF, double)));
    connect( this, SIGNAL(on_node_deleted(std::string)), &this->_viewer, SLOT(node_deleted(std::string)));
    connect( this, SIGNAL(on_node_selected(std::string)), &this->_viewer, SLOT(node_selected(std::string)));
    connect( this, SIGNAL(on_connection_created(std::pair<std::string, std::string>)), &this->_viewer, SLOT(connection_created(std::pair<std::string, std::string>)));
    connect( this, SIGNAL(on_connection_erased(std::pair<std::string, std::string>)), &this->_viewer, SLOT(connection_deleted(std::pair<std::string, std::string>)));

}

wifi::~wifi()
{

}

void wifi::on_actionLoad_triggered()
{

}

void wifi::on_actionSave_triggered()
{

}

void wifi::on_actionRun_triggered()
{
    SIMUL.dbg.setStream("log.txt");
    SIMUL.dbg.enable(_WIFILINK_DBG);
    SIMUL.dbg.enable(_WIFIINTER_DBG);
    SIMUL.dbg.enable(_NODE_DBG);

    try {
      //  cout << "U = " << u << " M = " << _m << endl;
       // SIMUL.run(SIM_LEN, 5);
       // output.write(u);
    } catch (BaseExc &e) {
        cout << e.what() << endl;
    }
}

void wifi::on_actionShow_Results_triggered()
{

}

void wifi::updateLists(){
    ui->node_list->selectionModel()->reset();
    on_node_list_itemSelectionChanged();
}

void wifi::on_add_btn_clicked()
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
            _random->get();
            _nodes[name]->setInterval(std::unique_ptr<MetaSim::DeltaVar>(new MetaSim::DeltaVar(_random->get())));

            emit on_node_created( name, pos, radius );
        }else{
            QMessageBox msgBox;
            msgBox.setText("The node already exists");
            msgBox.exec();
        }
    }

    updateLists();
}

void wifi::on_del_btn_clicked()
{
    if(ui->node_list->selectedItems().size() > 0){
        std::string name = ui->node_list->selectedItems().front()->text().toUtf8().data();
        _nodes.erase(_nodes.find((name)));
        _interfaces.erase(_interfaces.find(name));
        delete ui->node_list->selectedItems().front();
        emit on_node_deleted(name);
        //removing connection related to erased node
        auto i = std::begin(_connections);
        while (i!= std::end(_connections)){
            if ( i->first == name || i->second == name){
                emit on_connection_erased(*i);
                i = _connections.erase(i);
            }
            else
                ++i;
        }
    }

    updateLists();
}


void wifi::on_item_selected( QListWidgetItem *item)
{
    std::string name = item->text().toUtf8().data();
    emit on_node_selected(name);
    //qDebug( "selected ");
    //qDebug( item->text().toUtf8().data());
    //const Node *node = _nodes

}

void wifi::on_connect_btn_clicked()
{
    if(ui->node_list->selectedItems().size() > 0){
        std::string to = ui->connect_list->currentText().toUtf8().data();
        if (to == "") return;
        std::string from = ui->node_list->selectedItems().front()->text().toUtf8().data();
        auto conn = make_pair(from, to);
        if (std::find(_connections.begin(), _connections.end(), conn) == _connections.end()){
            _connections.emplace_back(conn);
            emit on_connection_created(conn);
        }
    }

    updateLists();
}

void wifi::on_node_list_itemSelectionChanged()
{
    ui->connect_list->clear();
    if(ui->node_list->selectedItems().size() > 0){
        std::string name = ui->node_list->selectedItems().front()->text().toUtf8().data();
        for (auto& n : _nodes){
            if (n.first != name){
                ui->connect_list->addItem(QString(n.first.c_str()));
            }
        }
    }
}
