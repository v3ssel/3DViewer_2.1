#include "controller.h"

#include "../core/scene_builder.h"

namespace s21 {
Mesh* Controller::ParseMeshFromFile(const QString& path) {
  return MeshParser::Instance().Parse(path);
}

Scene* Controller::GetScene(QWidget* parent, const QRect& sizes) {
  SceneBuilder sb;
  return sb.SetParent(parent)->SetSceneSize(sizes)->Build();
}
}  // namespace s21
