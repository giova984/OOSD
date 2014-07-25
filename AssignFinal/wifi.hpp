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
    void on_actionShow_Results_triggered();
    void on_add_btn_clicked();
    void on_del_btn_clicked();
    void on_item_selected(QListWidgetItem *index);
    void on_connect_btn_clicked();
    void on_node_list_itemSelectionChanged();

private:
    std::unique_ptr<Ui::wifi> ui;
    NodeViewer _viewer;

    WifiLink _link;
    WifiRoutingTable _routing_table;
    std::map<std::string, std::unique_ptr<Node>> _nodes;
    std::map<std::string,std::unique_ptr<WifiInterface>> _interfaces;
    std::unique_ptr<MetaSim::RandomVar> _random;

    std::vector<std::pair<std::string, std::string>> _connections;

    void updateLists();

signals:
    void on_node_selected(const std::string& name);
    void on_node_created(const std::string& name, const QPointF& pos, double radius);
    void on_node_deleted(const std::string& name);
    void on_connection_created(std::pair<std::string, std::string> conn);
    void on_connection_erased(std::pair<std::string, std::string> conn);
};

#endif // WIFI_HPP
