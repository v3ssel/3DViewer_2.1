#include "controller.h"

namespace s21 {
Mesh* Controller::ParseMeshFromFile(QString path) {
    return MeshParser::Instance().Parse(path);
}
}  // namespace s21
