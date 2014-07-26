#include "wifi.hpp"
#include "ui_wifi.h"

#include "simul.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "link.hpp"
#include "netinterface.hpp"
#include "node.hpp"
#include "routing.hpp"
#include "wifistatistics.hpp"

#include <qdebug.h>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QDataStream>

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
    _viewer(this),
    _plotter(this),
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
    QString filename = QFileDialog::getOpenFileName(this);//getting the file name
    if(filename != ""){
        resetConfiguration();
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream in(&file);
            while(!in.atEnd()) {
                QString line = in.readLine();
                QStringList fields = line.split(",");
                if(fields.size()==5 && fields[0]=="Node"){
                    _nodes[fields[1].toStdString()] = std::make_tuple(
                                fields[2].toDouble(),
                            fields[3].toDouble(),
                            fields[4].toDouble());
                }else if(fields.size()==3 && fields[0]=="Connection"){
                    _connections.emplace_back(std::make_pair(
                                                  fields[1].toStdString(),
                                                  fields[2].toStdString()));
                }
            }
            file.close();
        }
        populateUI();
    }
}

void wifi::on_actionSave_triggered()
{
    QString FilePath = QFileDialog::getSaveFileName(this);
    if(FilePath.isEmpty())
        return;
    QFile file(FilePath);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
         QTextStream out(&file);
        for (auto n: _nodes)
            out <<
              QString("Node,")
                   .append(n.first.c_str()).append(",")
                   .append(QString::number(std::get<0>(n.second))).append(",")
                   .append(QString::number(std::get<1>(n.second))).append(",")
                   .append(QString::number(std::get<2>(n.second))).append("\n");                      ;
        for (auto c: _connections)
            out << QString("Connection,")
                   .append(c.first.c_str()).append(",")
                   .append(c.second.c_str()).append("\n");
        file.close();
    }
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
        }
    }
    for (int i=0; i<n; ++i){ //adding destinations
        std::string from{"Node_" + to_string(i) + "_" + to_string(0)};
        std::string to{"Node_" + to_string(i) + "_" + to_string(m-1)};
        _connections.emplace_back(std::make_pair(from, to));

    }
    for (int i=0; i<m; ++i){ //adding destinations
        std::string from{"Node_" + to_string(0) + "_" + to_string(i)};
        std::string to{"Node_" + to_string(m-1) + "_" + to_string(i)};
        _connections.emplace_back(std::make_pair(from, to));
    }
    populateUI();
}

void wifi::populateUI()
{
    for (auto& n : _nodes){
        new QListWidgetItem(QString(n.first.c_str()), ui->node_list);
        emit on_node_created( n.first,
                              QPointF(std::get<0>(n.second), std::get<1>(n.second)),
                              std::get<2>(n.second));
    }
    for (auto& c : _connections){
        emit on_connection_created(c);
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
    const int MAX_PACKET_SIZE = 1500;
    const int MAX_CONTENTION_TIME = 10;
    const int MAX_NUMBER_OF_COLLISION = 1000 * _nodes.size();
    const int PACKET_TO_SEND = 100;

    const int FIXED_GENERATOR_SEED = 12345;
    MetaSim::RandomVar::init(12345);

    const int NUMBER_OF_RUNS = 5;
    WifiLink link{"Channel_1"};
    WifiRoutingTable routing_table{};
    std::vector<double> intervals {10,100,250,500,1000,1500,2000};

    LinkCollisionStat lc_stat("");
    std::vector<std::pair<double, double>> lc_stat_results{};

    lc_stat.attach(&link);

    //Node-Interface creation
    std::map<std::string, std::unique_ptr<Node>> nodes;
    std::map<std::string,std::unique_ptr<WifiInterface>> interfaces;

    RandomGen fixed_generator{FIXED_GENERATOR_SEED};
    UniformVar seeds_gen {1, 1000, &fixed_generator};
    for (auto& n : _nodes){
        std::string name = n.first;
        nodes[name] = std::unique_ptr<Node>(new Node(name));
        auto seed = seeds_gen.get();
        std::cout << "seedgen " << seed << std::endl;
        interfaces[name] = std::unique_ptr<WifiInterface>(new WifiInterface(
                                                              std::string("interface").append(name),
                                                              *nodes[name],
                                                              make_pair(std::get<0>(n.second), std::get<1>(n.second)),
                                                              std::get<2>(n.second),
                                                              link,
                                                              seed,
                                                              &routing_table) );
    }
    for (auto& c : _connections){
        nodes[c.first]->addDestNode(*nodes[c.second]);
    }

//    SIMUL.dbg.setStream("run.txt");
    //    SIMUL.dbg.enable(_WIFILINK_DBG);
    //    SIMUL.dbg.enable(_WIFIINTER_DBG);
    //    SIMUL.dbg.enable(_NODE_DBG);

    for(int pass = 0; pass<intervals.size(); ++pass){
        const int MIN_VAR = 1;
        const int MAX_VAR = intervals[pass];
        MetaSim::UniformVar random{MetaSim::UniformVar(MIN_VAR, MAX_VAR, &fixed_generator)};
        //setting up nodes sending intervals
        for (auto& n : _nodes){
            auto delta = random.get();
            std::cout << "node " << n.first << " unif " << delta << std::endl;
            //nodes[n.first]->setInterval(std::unique_ptr<RandomVar>(new MetaSim::DeltaVar(delta)));
            nodes[n.first]->setInterval(std::unique_ptr<RandomVar>(new MetaSim::DeltaVar(intervals[pass])));
        }
        try {
            Tick estimated_simul_time{(MAX_VAR + MAX_PACKET_SIZE + MAX_CONTENTION_TIME*MAX_NUMBER_OF_COLLISION) * PACKET_TO_SEND};
            SIMUL.run(estimated_simul_time , NUMBER_OF_RUNS);
            //collecting results
            std::cout << "result " << MAX_VAR << " " <<lc_stat.getMean() << std::endl;
            lc_stat_results.emplace_back( std::make_pair(MAX_VAR, lc_stat.getMean()));
        } catch (BaseExc &e) {
            cout << e.what() << endl;
            QMessageBox msgBox;
            msgBox.setText(QString("Simulation Aborted:\n").append(e.what()));
            msgBox.exec();
        }
    }

    //populating graphs
    _plotter.setdata(lc_stat_results);
    on_actionShow_Results_triggered();
}

void wifi::on_actionShow_Results_triggered()
{
    _plotter.show();
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
