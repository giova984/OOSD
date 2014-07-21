#ifndef WIFI_HPP
#define WIFI_HPP

#include <QMainWindow>

namespace Ui {
class Wifi;
}

class Wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit Wifi(QWidget *parent = 0);
    ~Wifi();

private:
    Ui::Wifi *ui;
};

#endif // WIFI_HPP
