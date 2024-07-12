#include "scene_builder.h"

namespace s21 {
Scene* SceneBuilder::Build() {
    Scene* scene = new Scene(parent_);
    scene->setGeometry(size_);

    return scene;
}

SceneBuilder* SceneBuilder::SetParent(QWidget* parent) {
    parent_ = parent;
    return this;
}

SceneBuilder* SceneBuilder::SetSceneSize(const QRect& rect) {
    size_ = rect;
    return this;
}
}  // namespace s21
