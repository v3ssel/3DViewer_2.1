#ifndef SCENE_H
#define SCENE_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>

#include <QMouseEvent>
#include <QSettings>

#include <QWidget>
#include <QTimer>
#include <QDebug>

class scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  scene(QWidget *parent = nullptr);
  ~scene();

  void keyPressEvent(QKeyEvent *) override;
  void InitModel(QVector<GLfloat>& vertices, QVector<GLuint>& indices);
  void calculateCamera();

  QOpenGLShaderProgram program, light;
  QOpenGLVertexArrayObject vao, vao_light;
  QOpenGLBuffer vbo, ebo;

  QOpenGLTexture *texture = nullptr;

  QMatrix4x4 view, projection, model, lamp;

  QString filename;
  QSettings *settings;

  bool projection_type, wireframe, flat_shading;

  bool has_texture, has_normals; // tmp?

  QVector3D light_pos, light_color;
  QColor background, vertices_color, lines_color;
  unsigned line_width, vertex_size;
  bool circle_square, dashed_solid, is_none;

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *) override;

 private:
  QVector3D cameraTarget, cameraPos, cameraUp;

  float x_rot_, y_rot_, zoom_scale_, x_trans_, y_trans_, start_y_, start_x_;
  bool moving_, dragging_;

  void SaveSettings_();
  void LoadSettings_();

  void StartDraw_();
};

#endif  // SCENE_H
