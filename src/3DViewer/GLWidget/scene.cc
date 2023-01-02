#include "scene.h"

#include "../Viewer/viewer.h"

scene::scene(QWidget* parent) : QOpenGLWidget(parent) {
  settings = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                           QSettings::IniFormat);

  paintTimer = new QTimer(this); // создаю таймер
  connect(paintTimer, SIGNAL(timeout()), this, SLOT(repaint()));
  paintTimer->start();

  zoom_scale_ = -10;
  moving_ = false, dragging_ = false;
  start_x_ = 0, start_y_ = 0;
  x_rot_ = 1, y_rot_ = 1;
  x_trans_ = 0, y_trans_ = 0;

  LoadSettings_();
}

void scene::SaveSettings_() {
  settings->beginGroup("coordinate");
  settings->setValue("dashed_solid", dashed_solid);
  settings->setValue("projection", projection);
  settings->setValue("circle_square", circle_square);
  settings->setValue("is_none", is_none);
  settings->endGroup();

  settings->beginGroup("rgb");
  settings->setValue("red_bg", red_bg);
  settings->setValue("green_bg", green_bg);
  settings->setValue("blue_bg", blue_bg);
  settings->setValue("alpha_bg", alpha_bg);
  settings->setValue("red_vertex", red_vertex);
  settings->setValue("green_vertex", green_vertex);
  settings->setValue("blue_vertex", blue_vertex);
  settings->setValue("red_lines", red_lines);
  settings->setValue("green_lines", green_lines);
  settings->setValue("blue_lines", blue_lines);
  settings->endGroup();

  settings->beginGroup("size");
  settings->setValue("line_width", line_width);
  settings->setValue("vertex_size", vertex_size);
  settings->setValue("zoom_scale_", zoom_scale_);
  settings->endGroup();
}

void scene::LoadSettings_() {
  settings->beginGroup("coordinate");
  dashed_solid = settings->value("dashed_solid", false).toBool();
  projection = settings->value("projection", true).toBool();
  circle_square = settings->value("circle_square", false).toBool();
  is_none = settings->value("is_none", false).toBool();
  settings->endGroup();

  settings->beginGroup("rgb");
  red_bg = settings->value("red_bg", 0.0).toFloat();
  green_bg = settings->value("green_bg", 0.0).toFloat();
  blue_bg = settings->value("blue_bg", 0.0).toFloat();
  alpha_bg = settings->value("alpha_bg", 0.0).toFloat();
  red_vertex = settings->value("red_vertex", 0.0).toFloat();
  green_vertex = settings->value("green_vertex", 0.0).toFloat();
  blue_vertex = settings->value("blue_vertex", 0.0).toFloat();
  red_lines = settings->value("red_lines", 255.).toFloat();
  green_lines = settings->value("green_lines", 0.).toFloat();
  blue_lines = settings->value("blue_lines", 45.).toFloat();
  settings->endGroup();

  settings->beginGroup("size");
  line_width = settings->value("line_width", 5).toUInt();
  zoom_scale_ = settings->value("zoom_scale_", -10).toFloat();
  vertex_size = settings->value("vertex_size", 1).toUInt();
  settings->endGroup();
}

void scene::initializeGL() {
    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, texture);

    // Загрузка картинки
    QImage texture1;
    texture1.load("/opt/goinfre/madamkyl/test 2/bricks.jpg");
    qDebug() << texture1.isNull();
    texture1.convertTo(QImage::Format_RGBA8888); // формат текстуры OpenGL
    glBindTexture(GL_TEXTURE_2D, texture[0]); // привязываю текстуру GL_Texture_2d к участку памяти texture[index]
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // параметры фильтрации
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, GLsizei(texture1.width()), GLsizei(texture1.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());


    glEnable(GL_TEXTURE_2D);
    glClearColor(1,1,1,1);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    GLfloat light_ambient[] = { 0, 0, 0, 0.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.0, -4.0, 2.0, 1.0 };

    /* устанавливаем параметры источника света */
    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);

    /* включаем освещение и источник света */
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    //model[0] = draw
}

void scene::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 1000000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void scene::paintGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection) {
    glFrustum(-1, 1, -1, 1, 1, 1000000);
  } else {
    glOrtho(-1, 1, -1, 1, -1, 1000000);
  }

  if (projection)
    glTranslatef(x_trans_, y_trans_, zoom_scale_);
  else
    glScaled(zoom_scale_, zoom_scale_, zoom_scale_);

  glClearColor(red_bg / 255.0f, green_bg / 255.0f, blue_bg / 255.0f, alpha_bg);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(x_rot_, 1, 0, 0);
  glRotatef(y_rot_, 0, 1, 0);

  StartDraw_();

//  glBindTexture(GL_TEXTURE_2D, texture[0]);
//  glCallList(model[0]);
//  angle += 0.3f;
}

void scene::mousePressEvent(QMouseEvent* mouse) {
  switch (mouse->button()) {
    case Qt::LeftButton:
      moving_ = true;
      dragging_ = false;
      break;
    case Qt::MiddleButton:
      dragging_ = true;
      moving_ = false;
      break;
    default:
      moving_ = false;
      dragging_ = false;
      break;
  }
  start_x_ = mouse->pos().x();
  start_y_ = mouse->pos().y();
}

void scene::mouseMoveEvent(QMouseEvent* mouse) {
  if (moving_) {
    y_rot_ = y_rot_ + (mouse->pos().x() - start_x_);
    x_rot_ = x_rot_ + (mouse->pos().y() - start_y_);
  }
  if (dragging_) {
    x_trans_ = x_trans_ + (mouse->pos().x() - start_x_) / 4.f;
    y_trans_ = y_trans_ - (mouse->pos().y() - start_y_) / 4.f;
  }
  start_x_ = mouse->pos().x();
  start_y_ = mouse->pos().y();
  update();
}

void scene::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_R:
      x_trans_ = 0, y_trans_ = 0;
      zoom_scale_ = -10;
      break;
    case Qt::Key_O:
      x_trans_ = 0, y_trans_ = 0, zoom_scale_ = 0.1;
      projection = false;
      break;
    case Qt::Key_P:
      x_trans_ = 0, y_trans_ = 0, zoom_scale_ = -10;
      projection = true;
      break;
  }
  update();
}

void scene::wheelEvent(QWheelEvent* event) {
  QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() < 0)
    zoom_scale_ = projection ? zoom_scale_ * 1.1 : zoom_scale_ / 1.1;
  if (numDegrees.y() > 0)
    zoom_scale_ = projection ? zoom_scale_ / 1.1 : zoom_scale_ * 1.1;
  update();
}

void scene::StartDraw_() {
//  glBindTexture(GL_TEXTURE_2D, );
//  GLuint num = glGenLists(1);
//  glNewList(num,GL_COMPILE);

  glVertexPointer(3, GL_DOUBLE, 0,
                  s21::Controller::GetInstance().GetVertex().data());
  glEnableClientState(GL_VERTEX_ARRAY);

  glColor3d(red_vertex / 255.0f, green_vertex / 255.0f, blue_vertex / 255.0f);
  glPointSize(vertex_size);
  if (!circle_square) glEnable(GL_POINT_SMOOTH);
  if (!is_none)
    glDrawArrays(GL_POINTS, 1,
                 ((s21::Controller::GetInstance().GetVertex().size()
                       ? s21::Controller::GetInstance().GetVertex().size() + 1
                       : 3) -
                  3) /
                     3);

  glColor3d(red_lines / 255.0f, green_lines / 255.0f, blue_lines / 255.0f);
  glLineWidth(line_width);
  if (dashed_solid) {
    glLineStipple(1, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
  }

  glDrawElements(GL_LINES, s21::Controller::GetInstance().GetIndices().size(),
                 GL_UNSIGNED_INT,
                 s21::Controller::GetInstance().GetIndices().data());
  SaveSettings_();
  glDisable(GL_LINE_STIPPLE);
  glDisable(GL_POINT_SMOOTH);
  glDisableClientState(GL_VERTEX_ARRAY);

//  glEndList();
}
