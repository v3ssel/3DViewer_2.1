#ifndef _SCENE_BUILDER_H_
#define _SCENE_BUILDER_H_

#include "scene.h"

namespace s21 {
    class SceneBuilder {
       public:
        Scene* Build();

        SceneBuilder* SetParent(QWidget* parent = nullptr);
        SceneBuilder* SetSceneSize(const QRect& rect);
    
       private:
        QWidget* parent_;
        QRect size_;
    };
} // namespace s21


#endif  // _SCENE_BUILDER_H_
