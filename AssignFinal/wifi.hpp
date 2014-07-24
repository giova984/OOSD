#ifndef WIFI_HPP
#define WIFI_HPP

#include <QMainWindow>

namespace Ui {
class wifi;
}

class wifi : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

private:
    Ui::wifi *ui;
};

#endif // WIFI_HPP
