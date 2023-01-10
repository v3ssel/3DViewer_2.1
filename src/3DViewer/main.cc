#include <QApplication>

#include "GLWidget/scene.h"
#include "Viewer/viewer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QSurfaceFormat fmt;
  fmt.setVersion(3, 3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  viewer w;
  w.show();
  return a.exec();
}
