#include "controller.h"

namespace s21 {
void Controller::ParseVertex_3D(QString path_to_file) {
    return s21::MeshParser::GetInstance().Parse(path_to_file);
}

QVector<GLfloat> &Controller::GetPolygonsArray() {
    return s21::MeshParser::GetInstance().getFacetsArr();
}

QVector<GLuint> &Controller::GetIndices() {
    return s21::MeshParser::GetInstance().getIndicesArr();
}

QVector<QVector3D> &Controller::GetVertices() {
    return s21::MeshParser::GetInstance().getVertexArr();
}

QVector<QVector3D> &Controller::GetNormals() {
    return s21::MeshParser::GetInstance().getNormalsArr();
}

QVector<QVector2D> &Controller::GetUV() {
    return s21::MeshParser::GetInstance().getUVsArr();
}

bool Controller::NormalsUsage() { return !s21::MeshParser::GetInstance().getNormalsArr().empty(); }

bool Controller::TextureUsage() { return !s21::MeshParser::GetInstance().getUVsArr().empty(); }

void Controller::clearArrays() { s21::MeshParser::GetInstance().Clear(); }
}  // namespace s21
