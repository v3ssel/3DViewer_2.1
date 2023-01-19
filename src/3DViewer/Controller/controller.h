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

  QVector<GLfloat>&  GetPolygonsArray();
  QVector<GLuint>& GetIndices();

  QVector<QVector3D>& GetVertices();
  QVector<QVector3D>& GetNormals();
  QVector<QVector2D>& GetUV();

  void clearArrays();
  void ParseVertex_3D(const std::string path_to_file);

 private:
  Controller() {}
  Controller(const Controller&);
  void operator=(Controller&);
};

}  //  namespace s21

#endif
