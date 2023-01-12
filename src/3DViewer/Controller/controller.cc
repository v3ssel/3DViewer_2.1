#include "controller.h"

namespace s21 {
void Controller::ParseVertex_3D(std::string path_to_file) {
    return s21::parse::GetInstance().ParseVertex_3D(path_to_file);
}

QVector<GLfloat>& Controller::GetArray() {
    return s21::parse::GetInstance().facetsArray;
}
}  // namespace s21
