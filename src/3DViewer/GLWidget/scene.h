#ifndef SCENE_H
#define SCENE_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QSettings>

class scene : public QOpenGLWidget {
  Q_OBJECT

 public:
  scene(QWidget *parent = nullptr);
  void keyPressEvent(QKeyEvent *) override;

  QString filename;
  QSettings *settings;

  bool projection;

  float red_bg, green_bg, blue_bg, alpha_bg;
  float red_vertex, green_vertex, blue_vertex;
  float red_lines, green_lines, blue_lines;

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
  float x_rot_, y_rot_, zoom_scale_, x_trans_, y_trans_, start_y_, start_x_;
  bool moving_, dragging_;

  void SaveSettings_();
  void LoadSettings_();

  void StartDraw_();
};

#endif  // SCENE_H
