#ifndef PARSE_H
#define PARSE_H

#include <QFile>
#include <QVector3D>
#include <QVector>
#include <QWidget>
using GLfloat = float;
using GLuint = unsigned int;

namespace s21 {
class parse {
 public:
  static parse& GetInstance() {
    static parse instance;
    return instance;
  }
  void ParseVertex_3D(QString path_to_file);
  void ParseF(QStringList str);
  void pushArr(const char** tmp);
  void CheckFlags(QString path_to_file);
  void clear();

  QVector<QVector3D>& getVertexArr() { return vertex_; }
  QVector<QVector3D>& getNormalsArr() { return normals_; }
  QVector<QVector2D>& getUVsArr() { return uvs_; }
  QVector<GLfloat>& getFacetsArr() { return facetsArray; }
  QVector<GLuint>& getIndicesArr() { return indices; }

  bool vn_used;
  bool vt_used;

 private:
  parse() { clear(); }
  parse(const parse&);
  void operator=(parse&);

  void add_pseudo_str();

  QVector<GLfloat> facetsArray;
  QVector<GLuint> indices;
  QVector<QVector3D> vertex_;
  QVector<QVector3D> normals_;
  QVector<QVector2D> uvs_;
};
}  // namespace s21

#endif  // PARSE_H
