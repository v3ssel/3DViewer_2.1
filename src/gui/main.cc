#include <QApplication>

#include "viewer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Viewer w;
  w.show();
  return a.exec();
}
