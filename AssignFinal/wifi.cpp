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

    //_viewer.setScene(&_scene);
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

