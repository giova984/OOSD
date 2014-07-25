#ifndef WIFI_HPP
#define WIFI_HPP

#include <QMainWindow>

#include <memory>
#include <vector>

#include "link.hpp"
#include "routing.hpp"
#include "nodeviewer.hpp"

class QTableWidgetItem;

namespace Ui {
class wifi;
}

class Node;
class WifiInterface;



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

    void on_abb_btn_clicked();

    void on_del_btn_clicked();

private:
    Ui::wifi *ui;
    NodeViewer _viewer;

    WifiLink link;
    WifiRoutingTable routing_table;
    std::vector<std::unique_ptr<Node>> _nodes;
    std::unique_ptr<WifiInterface> _interfaces;

    std::vector<std::unique_ptr<QTableWidgetItem>> __items;
};

#endif // WIFI_HPP
