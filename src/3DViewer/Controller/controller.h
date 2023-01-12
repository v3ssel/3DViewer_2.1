#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Parsing/singleton.h"

//  Facade
namespace s21 {

class Controller {
 public:
  static Controller& GetInstance() {
    static Controller controller_;
    return controller_;
  }

  void ParseVertex_3D(const std::string path_to_file);
  QVector<GLfloat>&  GetArray();
//  QVector<QVector3D> vertex_;
//  QVector<QVector3D> normals_;
//  QVector<QVector2D> uvs_;

 private:
  Controller() {}
  Controller(const Controller&);
  void operator=(Controller&);
};

}  //  namespace s21

#endif
