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

class scene : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  scene(QWidget *parent = nullptr);
  void keyPressEvent(QKeyEvent *) override;
  void InitModel(GLfloat *vertices, GLuint *indices);
  void calculateCamera();
  QTimer *timer;

  QOpenGLShaderProgram program, light;
  QOpenGLVertexArrayObject vao, vao_light;
  QOpenGLBuffer vbo;
  QOpenGLBuffer ebo;

  QOpenGLTexture *texture;

  QMatrix4x4 view, projection, model, lamp;

  QString filename;
  QSettings *settings;

  bool projection_type, wireframe;

  float red_bg, green_bg, blue_bg, alpha_bg;
  float red_vertex, green_vertex, blue_vertex;
  float red_lines, green_lines, blue_lines;

  unsigned line_width, vertex_size;
  bool circle_square, dashed_solid, is_none;

  GLuint vsize = 0, isize = 0;  // tmp

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
