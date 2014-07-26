#ifndef WIFI_HPP
#define WIFI_HPP

#include <QMainWindow>

#include <memory>
#include <map>

#include "link.hpp"
#include "routing.hpp"
#include "nodeviewer.hpp"

class QTableWidgetItem;

namespace Ui {
class wifi;
}

class Node;
class WifiInterface;
class QListWidgetItem;


class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

private slots:
    void on_actionLoad_triggered();
    void on_actionRun_triggered();
    void on_actionSave_triggered();
    void on_actionClear_triggered();
    void on_actionLoad_3x3_triggered();
    void on_actionLoad_4x4_triggered();
    void on_actionLoad_5x5_triggered();
    void on_actionShow_Results_triggered();
    void on_add_btn_clicked();
    void on_del_btn_clicked();
    void on_item_selected(QListWidgetItem *index);
    void on_connect_btn_clicked();
    void on_node_list_itemSelectionChanged();
    void on_connect_list_activated( const QString & text );
    void currentIndexChanged( const QString & text );


private:
    std::unique_ptr<Ui::wifi> ui;
    NodeViewer _viewer;

//    WifiLink _link;
//    WifiRoutingTable _routing_table;
    //std::map<std::string, std::unique_ptr<Node>> _nodes;
    //std::map<std::string,std::unique_ptr<WifiInterface>> _interfaces;
//    std::unique_ptr<MetaSim::RandomVar> _random;

    std::map<std::string, std::tuple<double, double, double>> _nodes;
    std::vector<std::pair<std::string, std::string>> _connections;

    void resetConfiguration();
    void generateConfiguration(int n, int m);
    void populateUI();
    void resetLists();

signals:
    void on_node_selected(const std::string& name);
    void on_destination_selected(const std::string& name);
    void on_node_created(const std::string& name, const QPointF& pos, double radius);
    void on_node_deleted(const std::string& name);
    void on_connection_created(std::pair<std::string, std::string> conn);
    void on_connection_erased(std::pair<std::string, std::string> conn);
};

#endif // WIFI_HPP
