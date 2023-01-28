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
  void CalculateCamera();

  QVector3D light_pos, light_color;
  QColor background, vertices_color, lines_color;
  unsigned line_width, vertex_size;
  bool circle_square, dashed_solid, is_none;

  float scale_factor;

  bool projection_type, wireframe, flat_shading;
  bool has_texture, has_normals, is_light_enabled;

  QSettings *settings;

  QOpenGLShaderProgram program, light;
  QOpenGLVertexArrayObject vao, vao_light;
  QOpenGLBuffer vbo, ebo;
  QOpenGLTexture *texture;
  QMatrix4x4 view;

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *) override;

 private:
  void LightInit_();
  void CheckDisplayType_();
  void StartDraw_();
  void DrawLight_(QMatrix4x4& projection, QMatrix4x4& lamp);

  void SaveSettings_();
  void LoadSettings_();


  float x_rot_, y_rot_, x_trans_, y_trans_, start_y_, start_x_;
  bool moving_, dragging_;

  QVector3D camera_target_, camera_pos_, camera_up_;
};

#endif  // SCENE_H
