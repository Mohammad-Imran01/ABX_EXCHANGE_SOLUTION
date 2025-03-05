#include <QCoreApplication>
#include "exchangehandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ExchangeHandler exchangeHandler;

    return a.exec();
}
