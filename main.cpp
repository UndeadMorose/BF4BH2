#include "bf4bh2.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  BF4BH2 w;
  w.show();
  return a.exec();
}
