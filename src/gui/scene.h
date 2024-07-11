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

    void keyPressEvent(QKeyEvent *) override;
    void InitModel(const QString& filename);
    void ResetModel();

    size_t VertexCount();
    size_t IndexCount();

    void CalculateCamera();
    void RotateModel(float x, float y, float z);
    QList<QLine> GetLines(QPixmap map);

    QVector3D light_pos, light_color, move_object;
    QColor background, vertices_color, lines_color;
    unsigned line_width, vertex_size;
    bool circle_square, dashed_solid, is_none;

    float scale_factor;
    float r_x, r_y, r_z;

    bool projection_type, wireframe, flat_shading;
    bool has_texture, has_normals, is_light_enabled;

    QSettings *settings;

    QOpenGLShaderProgram program, light;
    QOpenGLVertexArrayObject vao, vao_light;
    QOpenGLBuffer vbo, ebo, vbo_light;
    QOpenGLTexture *texture;
    QMatrix4x4 view, projection;

   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

   private:
    void InitLight();
    void SetDisplayType();
    void DrawModel();
    void DrawLight();

    void SaveSettings_();
    void LoadSettings_();

    float x_rot_, y_rot_, start_y_, start_x_;
    bool moving_;

    QVector3D camera_target_, camera_pos_, camera_up_;
    QQuaternion rotation_;

    s21::Mesh* mesh_;
};

#endif  // SCENE_H
