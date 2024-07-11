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
#include <QSettings>
#include <QTimer>
#include <QWidget>

#include "../controller/controller.h"

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

   public:
    Scene(QWidget *parent = nullptr);
    ~Scene();

    void InitModel(const QString& filename);
    void ResetModel();
    void RotateModel(float x, float y, float z);

    size_t VertexCount();
    size_t IndexCount();

    void keyPressEvent(QKeyEvent *) override;

    QVector3D model_pos, prev_rotation;
    QColor background, vertices_color, lines_color;

    unsigned line_width, vertex_size;
    float scale_factor;

    bool circle_square, dashed_solid, no_vertices;
    bool projection_type;

    QSettings *settings;

   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo, ebo;
    QMatrix4x4 view, projection;

   private:
    void LoadShaders();
    void DrawModel();

    void CalculateCamera();

    void SaveSettings();
    void LoadSettings();

    float x_rot_, y_rot_, start_y_, start_x_;
    bool is_moving_;

    QVector3D camera_target_, camera_pos_, camera_up_;
    QQuaternion rotation_;

    s21::Mesh* mesh_;
};

#endif  // SCENE_H
