#ifndef _MESH_H_
#define _MESH_H_

#include <qopengl.h>

#include <QVector2D>
#include <QVector3D>
#include <QVector>

namespace s21 {
struct Mesh {
  QVector<GLfloat> facets;
  QVector<GLuint> indices;

  QVector<QVector3D> vertices;
  QVector<QVector3D> normals;
  QVector<QVector2D> uvs;

  void Reset() {
    facets.clear();
    indices.clear();
    vertices.clear();
    normals.clear();
    uvs.clear();
  }
};
}  // namespace s21

#endif  // _MESH_H_
