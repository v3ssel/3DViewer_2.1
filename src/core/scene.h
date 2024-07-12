#ifndef SCENE_H
#define SCENE_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QWidget>

#include "mesh.h"

namespace s21 {
class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

   public:
    Scene(QWidget *parent = nullptr);
    ~Scene();

    virtual void InitModel(Mesh* mesh);
    virtual void ResetModel();

    virtual void MoveModel(float x, float y, float z);
    virtual void RotateModel(float x, float y, float z);
    virtual void ScaleModel(float scale);

    virtual void ResetScene();
    virtual void ChangeProjectionType();
    bool projection_type;

    size_t VertexCount();
    size_t IndexCount();

    QColor background, vertices_color, lines_color;
    unsigned line_width, vertex_size;
    bool circle_vertex, dashed_line, no_vertices;

   protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;
    virtual void wheelEvent(QWheelEvent *) override;

    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo, ebo;
    QMatrix4x4 view, projection;

    QVector3D model_pos;
    QQuaternion rotation;
    float scale_factor;

   private:
    void LoadShaders();
    void DrawModel();

    void SetCamera();

    float x_rot_, y_rot_, start_y_, start_x_;
    bool is_moving_;


    QVector3D prev_rotation_;
    QVector3D camera_target_, camera_pos_, camera_up_;

    Mesh* mesh_;
};
}

#endif  // SCENE_H
