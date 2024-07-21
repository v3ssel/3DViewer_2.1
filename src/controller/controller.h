#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../core/mesh_parser.h"
#include "../core/scene.h"

// Singleton + Facade
namespace s21 {
class Controller {
 public:
  static Controller& Instance() {
    static Controller controller_;
    return controller_;
  }

  Mesh* ParseMeshFromFile(const QString& path);
  Scene* GetScene(QWidget* parent, const QRect& sizes);

 private:
  Controller() = default;
  Controller(const Controller&);
  Controller& operator=(Controller&) = delete;
};
}  //  namespace s21

#endif
