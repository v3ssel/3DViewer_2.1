#include "controller.h"

namespace s21 {
Mesh* Controller::ParseMeshFromFile(const QString& path) {
  return MeshParser::Instance().Parse(path);
}
}  // namespace s21
