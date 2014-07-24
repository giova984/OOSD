#include "wifi.hpp"
#include "ui_wifi.h"

wifi::wifi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);
}

wifi::~wifi()
{
    delete ui;
}
