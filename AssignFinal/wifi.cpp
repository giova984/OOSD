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

#include<sstream>
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui{std::unique_ptr<Ui::wifi>(new Ui::wifi)},
    _nodes{},
    _connections{}
{
    ui->setupUi(this);

    ui->viewer_layout->addWidget(&_viewer);
    //WList of nodes widget connection
//    connect( ui->node_list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(on_item_selected(QListWidgetItem *)));
    //Node Viewer connections
    connect( this, SIGNAL(on_node_created(std::string, QPointF, double)), &this->_viewer, SLOT(node_created(std::string, QPointF, double)));
    connect( this, SIGNAL(on_node_deleted(std::string)), &this->_viewer, SLOT(node_deleted(std::string)));
    connect( this, SIGNAL(on_node_selected(std::string)), &this->_viewer, SLOT(node_selected(std::string)));
    connect( this, SIGNAL(on_destination_selected(std::string)), &this->_viewer, SLOT(destination_selected(std::string)));
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

void wifi::resetConfiguration(){
    for (auto& n : _nodes)
        emit on_node_deleted(n.first);
    for (auto& c : _connections)
        emit on_connection_erased(c);
    _nodes.clear();
    _connections.clear();
    ui->node_list->clear();
    ui->connect_list->clear();
    resetLists();
}

void wifi::generateConfiguration(int n, int m)
{
    resetConfiguration();
    for (int i=0; i<n; ++i){ //creating nodes and WifiInterfaces
        for (int j=0; j<m; ++j){
            std::string name {"Node_" + to_string(i) + "_" + to_string(j)};
            _nodes[name] = std::make_tuple(i, j, 1.0);
            emit on_node_created( name, QPointF(i, j), 1.0 );
        }
    }
    for (int i=0; i<n; ++i){ //adding destinations
        std::string from{"Node_" + to_string(i) + "_" + to_string(0)};
        std::string to{"Node_" + to_string(i) + "_" + to_string(m-1)};
        _connections.emplace_back(std::make_pair(from, to));
        emit on_connection_created(std::make_pair(from, to));
    }
    for (int i=0; i<m; ++i){ //adding destinations
        std::string from{"Node_" + to_string(0) + "_" + to_string(i)};
        std::string to{"Node_" + to_string(m-1) + "_" + to_string(i)};
        _connections.emplace_back(std::make_pair(from, to));
        emit on_connection_created(std::make_pair(from, to));
    }
}

void wifi::on_actionClear_triggered()
{
    resetConfiguration();
}

void wifi::on_actionLoad_3x3_triggered()
{
    resetConfiguration();
    generateConfiguration(3, 3);
}

void wifi::on_actionLoad_4x4_triggered()
{
    resetConfiguration();
    generateConfiguration(4, 4);
}

void wifi::on_actionLoad_5x5_triggered()
{
    resetConfiguration();
    generateConfiguration(5, 5);
}

void wifi::on_actionRun_triggered()
{
    const int MIN_VAR = 1;
    const int MAX_VAR = 1000;
    const int MAX_PACKET_SIZE = 1500;
    const int MAX_CONTENTION_TIME = 15;
    const int PACKET_TO_SEND = 100;
    const int NUMBER_OF_RUNS = 5;
    WifiLink link{"Channel_1"};
    WifiRoutingTable routing_table{};
    std::unique_ptr<MetaSim::RandomVar> random{std::unique_ptr<RandomVar>(new MetaSim::UniformVar(MIN_VAR, MAX_VAR))};

    //Node-Interface creation
    std::map<std::string, std::unique_ptr<Node>> nodes;
    std::map<std::string,std::unique_ptr<WifiInterface>> interfaces;
    for (auto& n : _nodes){
        std::string name = n.first;
        nodes[name] = std::unique_ptr<Node>(new Node(name));
        interfaces[name] = std::unique_ptr<WifiInterface>(new WifiInterface(
                                                               std::string("interface").append(name),
                                                               *nodes[name],
                                                               make_pair(std::get<0>(n.second), std::get<1>(n.second)),
                                                               std::get<2>(n.second),
                                                               link,
                                                               &routing_table) );
        nodes[name]->setInterval(std::unique_ptr<MetaSim::DeltaVar>(new MetaSim::DeltaVar(random->get())));
    }
    for (auto& c : _connections){
        nodes[c.first]->addDestNode(*nodes[c.second]);
    }

    SIMUL.dbg.setStream("run.txt");
    SIMUL.dbg.enable(_WIFILINK_DBG);
    SIMUL.dbg.enable(_WIFIINTER_DBG);
    SIMUL.dbg.enable(_NODE_DBG);

    try {
        SIMUL.dbg << "aaa";
        SIMUL.dbg << "aaa" << std::endl;
      //  cout << "U = " << u << " M = " << _m << endl;
        SIMUL.run((MAX_VAR + MAX_PACKET_SIZE + MAX_CONTENTION_TIME) * PACKET_TO_SEND , NUMBER_OF_RUNS);
       // output.write(u);
    } catch (BaseExc &e) {
        cout << e.what() << endl;
    }
}

void wifi::on_actionShow_Results_triggered()
{

}

void wifi::resetLists(){
    ui->node_list->selectionModel()->reset();
    on_item_selected(ui->node_list->currentItem());
    on_node_list_itemSelectionChanged();
}

void wifi::on_add_btn_clicked()
{
    if (ui->node_name_text->text() != "" ){
        std::string name = ui->node_name_text->text().toUtf8().data();
        QPointF pos{ui->spin_x->value(), ui->spin_y->value()};
        double radius = ui->spin_radius->value();

        if(_nodes.find(name) == _nodes.end() ){
            _nodes[name] = std::make_tuple(pos.x(), pos.y(), radius);
            new QListWidgetItem(ui->node_name_text->text(), ui->node_list);
            emit on_node_created( name, pos, radius );
        }else{
            QMessageBox msgBox;
            msgBox.setText("The node already exists");
            msgBox.exec();
        }
    }

    resetLists();
}

void wifi::on_del_btn_clicked()
{
    if(ui->node_list->selectedItems().size() > 0){
        std::string name = ui->node_list->selectedItems().front()->text().toUtf8().data();
        _nodes.erase(_nodes.find((name)));
        //_interfaces.erase(_interfaces.find(name));
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

    resetLists();
}


void wifi::on_item_selected( QListWidgetItem *item)
{
    std::string name {(item) ? item->text().toUtf8().data() : ""};
    emit on_node_selected(name);
    //qDebug( "selected ");
    //qDebug( item->text().toUtf8().data());
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

    resetLists();
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
        emit on_node_selected(name);
    }
    emit currentIndexChanged (ui->connect_list->currentText());
}

void wifi::on_connect_list_activated(const QString &text)
{
    emit on_destination_selected(text.toUtf8().data());
}

void wifi::currentIndexChanged(const QString &text)
{
    emit on_destination_selected(text.toUtf8().data());
}
