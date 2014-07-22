#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main2(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel label("Hello Qt!");
    label.show();
    return app.exec();
}
