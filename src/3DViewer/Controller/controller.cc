#include "controller.h"

namespace s21 {
void Controller::ParseVertex_3D(std::string path_to_file) {
    return s21::parse::GetInstance().ParseVertex_3D(path_to_file);
}

QVector<GLfloat>& Controller::GetPolygonsArray() {
    return s21::parse::GetInstance().getFacetsArr();
}

QVector<GLuint> &Controller::GetIndices() {
    return s21::parse::GetInstance().getIndicesArr();
}

QVector<QVector3D> &Controller::GetVertices() {
    return s21::parse::GetInstance().getVertexArr();
}

QVector<QVector3D> &Controller::GetNormals() {
    return s21::parse::GetInstance().getNormalsArr();
}

QVector<QVector2D> &Controller::GetUV() {
    return s21::parse::GetInstance().getUVsArr();
}

void Controller::clearArrays() {
    s21::parse::GetInstance().clear();
}
}  // namespace s21
