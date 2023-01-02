#ifndef SRC_CPART_PARSER_H
#define SRC_CPART_PARSER_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <QVector3D>
#include <QtOpenGLWidgets/qopenglwidget.h> // для типа GLuint

//  Singleton
namespace s21 {
class Parser {
 public:
  static Parser& GetInstance() {
    static Parser instance;
    return instance;
  }

  void ParseVertex(const std::string path_to_file);
  void ParseIndices(const std::string path_to_file);

  std::vector<double>& GetVertex();
  std::vector<int>& GetIndices();

  struct face // грань
  {
      struct vertex { // вершина
          GLuint v_i; // индекс вершины
          GLuint vt_i; // индекс текстурной вершины
          GLuint vn_i; // индекс вершины нормали
      };

      vertex v[3]; // три вершины у треугольника - полигона

      face(vertex v1,vertex v2,vertex v3) // полигон (грань)
      {
          v[0]=v1;
          v[1]=v2;
          v[2]=v3;
      }
  };

//  std::vector<std::string> coord;
//  std::vector<QVector3D> vertex;
//  std::vector<QVector2D> uvs;
//  std::vector<QVector3D> normals;
//  std::vector<face> faces;

 private:
  Parser() {}
  Parser(const Parser&);
  void operator=(Parser&);

  std::vector<double> vertex_;
  std::vector<int> indices_;

  std::vector<double> normals_; // vn
  std::vector<double> uvs_; // vt

};

class Transform {
 public:
  static Transform& GetInstance() {
    static Transform instance;
    return instance;
  }

  void MoveXYZ(double x1, double y1, double z1);
  void Scale(double c);
  void RotateXYZ(char coordinate, double angle);

 private:
  Transform() {}
  Transform(const Transform&);
  void operator=(const Transform&);
};
}  // namespace s21

#endif
