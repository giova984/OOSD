#include "wifi.hpp"
#include "ui_wifi.h"

Wifi::Wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Wifi)
{
    ui->setupUi(this);
}

Wifi::~Wifi()
{
    delete ui;
}
