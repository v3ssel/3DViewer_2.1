#include "scene.h"

#include "../Viewer/viewer.h"

scene::scene(QWidget* parent) : QOpenGLWidget(parent) {
  settings = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                           QSettings::IniFormat);

  cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);

  isize = 0, vsize = 0;
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
//  projection = settings->value("projection", true).toBool();
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

void scene::InitModel(GLfloat *vertices, GLuint *indices) {

}

static GLfloat vertices[] = {
    1.000000, 1.000000, -1.000000,   0.000245, 0.500000,   0.0000, 1.0000, 0.0000,
    -1.000000, 1.000000, -1.000000,  0.333089, 0.500000,   0.0000, 1.0000, 0.0000,
    -1.000000, 1.000000, 1.000000,   0.333089, 0.999266,   0.0000, 1.0000, 0.0000,

    1.000000, 1.000000, -1.000000,   0.000245, 0.500000,   0.0000, 1.0000, 0.0000,
    -1.000000, 1.000000, 1.000000,   0.333089, 0.999266,   0.0000, 1.0000, 0.0000,
    1.000000, 1.000000, 1.000000,    0.000245, 0.999266,   0.0000, 1.0000, 0.0000,


    1.000000, -1.000000, 1.000000,   0.666911, 0.499511,   0.0000, 0.0000, 1.0000,
    1.000000, 1.000000, 1.000000,    0.666911, 0.000245,   0.0000, 0.0000, 1.0000,
    -1.000000, 1.000000, 1.000000,   0.999755, 0.000245,   0.0000, 0.0000, 1.0000,

    1.000000, -1.000000, 1.000000,   0.666911, 0.499511,   0.0000, 0.0000, 1.0000,
    -1.000000, 1.000000, 1.000000,   0.999755, 0.000245,   0.0000, 0.0000, 1.0000,
    -1.000000, -1.000000, 1.000000,  0.999756, 0.499511,   0.0000, 0.0000, 1.0000,


    -1.000000, -1.000000, 1.000000,  0.666422, 0.500000,   -1.0000, 0.0000, 0.0000,
    -1.000000, 1.000000, 1.000000,   0.666422, 0.999266,   -1.0000, 0.0000, 0.0000,
    -1.000000, 1.000000, -1.000000,  0.333578, 0.999266,   -1.0000, 0.0000, 0.0000,

    -1.000000, -1.000000, 1.000000,  0.666422, 0.500000,   -1.0000, 0.0000, 0.0000,
    -1.000000, 1.000000, -1.000000,  0.333578, 0.999266,   -1.0000, 0.0000, 0.0000,
    -1.000000, -1.000000, -1.000000, 0.333578, 0.500000,   -1.0000, 0.0000, 0.0000,


    -1.000000, -1.000000, -1.000000, 0.000245, 0.000245,   0.0000, -1.0000, 0.0000,
    1.000000, -1.000000, -1.000000,  0.333089, 0.000245,   0.0000, -1.0000, 0.0000,
    1.000000, -1.000000, 1.000000,   0.333089, 0.499511,   0.0000, -1.0000, 0.0000,

    -1.000000, -1.000000, -1.000000, 0.000245, 0.000245,   0.0000, -1.0000, 0.0000,
    1.000000, -1.000000, 1.000000,   0.333089, 0.499511,   0.0000, -1.0000, 0.0000,
    -1.000000, -1.000000, 1.000000,  0.000245, 0.499511,   0.0000, -1.0000, 0.0000,


    1.000000, -1.000000, -1.000000,  0.666911, 0.999266,   1.0000, 0.0000, 0.0000,
    1.000000, 1.000000, -1.000000,   0.666911, 0.500000,   1.0000, 0.0000, 0.0000,
    1.000000, 1.000000, 1.000000,    0.999755, 0.500000,   1.0000, 0.0000, 0.0000,

    1.000000, -1.000000, -1.000000,  0.666911, 0.999266,   1.0000, 0.0000, 0.0000,
    1.000000, 1.000000, 1.000000,    0.999755, 0.500000,   1.0000, 0.0000, 0.0000,
    1.000000, -1.000000, 1.000000,   0.999756, 0.999266,   1.0000, 0.0000, 0.0000,


    -1.000000, -1.000000, -1.000000, 0.666422, 0.000245,   0.0000, 0.0000, -1.0000,
    -1.000000, 1.000000, -1.000000,  0.666422, 0.499511,   0.0000, 0.0000, -1.0000,
    1.000000, 1.000000, -1.000000,   0.333578, 0.499511,   0.0000, 0.0000, -1.0000,

    -1.000000, -1.000000, -1.000000, 0.666422, 0.000245,   0.0000, 0.0000, -1.0000,
    1.000000, 1.000000, -1.000000,   0.333578, 0.499511,   0.0000, 0.0000, -1.0000,
    1.000000, -1.000000, -1.000000,  0.333578, 0.000245,   0.0000, 0.0000, -1.0000
};

static GLuint indices[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};

void scene::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

//    makeCurrent();
    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vert.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/frag.glsl");
    program.link();

    texture = new QOpenGLTexture(QImage("/Users/stevenso/shaderslight/cube.png"));
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    light.create();
    light.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/l_vert.glsl");
    light.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/l_frag.glsl");
    light.link();

    vao.create();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    vao.bind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    program.enableAttributeArray(0);

    program.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 8 * sizeof(float));
    program.enableAttributeArray(1);

    program.setAttributeBuffer(2, GL_FLOAT, 5 * sizeof(float), 3, 8 * sizeof(float));
    program.enableAttributeArray(2);

    ebo.bind();
    ebo.allocate(indices, sizeof(indices));
    program.bind();

    vao_light.create();
    vbo.bind();
    vao_light.bind();
    light.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    light.enableAttributeArray(0);

}

void scene::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void scene::paintGL() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    calculateCamera();
    program.bind();

    program.setUniformValueArray(program.uniformLocation("view"), &view, 1);

//    QVector3D ocol(1.0f, 0.9f, 0.31f);
//    program.setUniformValueArray(program.uniformLocation("objectColor"), &ocol, 1);
    QVector3D lcol(1.0f, 1.0f, 1.0f);
    program.setUniformValueArray(program.uniformLocation("lightColor"), &lcol, 1);

    QVector3D lpos(1.0f, 1.0f, 1.0f);
//    static float x = 0.0f, y = -1.0f;
//    lpos[0] = 1.0f + sin(x) * 2.0f;
//    lpos[2] = sin(y) * 2.0f + 1.0f;
//    x+=0.1, y+=0.1;

    program.setUniformValueArray(program.uniformLocation("lightPos"), &lpos, 1);
    program.setUniformValueArray(program.uniformLocation("viewPos"), &cameraPos, 1);

    projection.setToIdentity();
    view.setToIdentity();
//    projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    projection.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f);
//    projection.frustum(-2, 2, -2, 2, 2, 1000);
//    view.lookAt(cameraPos, cameraTarget, cameraUp);
    float r = 3.0 * cos(y_rot_ * M_PI / 180);
    view.lookAt(QVector3D(r * sin(x_rot_ * M_PI / 180), 3.0f * sin(y_rot_ * M_PI / 180), r * cos(x_rot_ * M_PI / 180)) + QVector3D(0.0f, 0.0f, 0.0f),
                QVector3D(0.0f, 0.0f, 0.0f),
                QVector3D(-sin(x_rot_ * M_PI / 180) * sin(x_rot_ * M_PI / 180),
                          cos(y_rot_ * M_PI / 180),
                          -cos(x_rot_ * M_PI / 180) * sin(x_rot_ * M_PI / 180)));

    program.setUniformValueArray(program.uniformLocation("projection"), &projection, 1);
    QVector4D col = QVector4D(0.3f, 0.0f, 0.0f, 1.0f);
    program.setUniformValueArray(program.uniformLocation("lineColor"), &col, 1);
    program.setUniformValueArray(program.uniformLocation("model"), &model, 1);

    vao.bind();

    texture->bind();

    vao.bind();
    glDrawArrays(GL_POINTS, 0, 8);
    glPointSize(15);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


    light.bind();

    light.setUniformValueArray(light.uniformLocation("projection"), &projection, 1);
    light.setUniformValueArray(light.uniformLocation("view"), &view, 1);

    lamp = model;
    lamp.setToIdentity();
    lamp.translate(lpos);
    lamp.scale(0.1f);
    light.setUniformValueArray(light.uniformLocation("model"), &lamp, 1);

    vao_light.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
//    program.disableAttributeArray(0);
//    program.disableAttributeArray(1);

    vao.release();
    vao_light.release();
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  if (projection) {
//    glFrustum(-1, 1, -1, 1, 1, 1000000);
//  } else {
//    glOrtho(-1, 1, -1, 1, -1, 1000000);
//  }

//  if (projection)
//    glTranslatef(x_trans_, y_trans_, zoom_scale_);
//  else
//    glScaled(zoom_scale_, zoom_scale_, zoom_scale_);

//  glClearColor(red_bg / 255.0f, green_bg / 255.0f, blue_bg / 255.0f, alpha_bg);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();

//  glTranslatef(0,0,-2.5);
//  glRotatef(x_rot_, 1, 0, 0);
//  glRotatef(y_rot_, 0, 1, 0);
//  angle += 0.3f;

//  StartDraw_();
}

void scene::StartDraw_() {
//  glVertexPointer(3, GL_DOUBLE, 0,
//                  s21::Controller::GetInstance().GetVertex().data()); // массив данных вершин
//  glEnableClientState(GL_VERTEX_ARRAY); // позволяют включать и отключать массивы соответственно

//  glColor3d(red_vertex / 255.0f, green_vertex / 255.0f, blue_vertex / 255.0f);
//  glPointSize(vertex_size);
//  if (!circle_square) glEnable(GL_POINT_SMOOTH);
//  if (!is_none)
//    glDrawArrays(GL_POINTS, 1,
//                 ((s21::Controller::GetInstance().GetVertex().size()
//                       ? s21::Controller::GetInstance().GetVertex().size() + 1
//                       : 3) -
//                  3) /
//                     3);

//  glColor3d(red_lines / 255.0f, green_lines / 255.0f, blue_lines / 255.0f);
//  glLineWidth(line_width);
//  if (dashed_solid) {
//    glLineStipple(1, 0x00FF);
//    glEnable(GL_LINE_STIPPLE);
//  }

//  glDrawElements(GL_LINES, s21::Controller::GetInstance().GetIndices().size(),
//                 GL_UNSIGNED_INT,
//                 s21::Controller::GetInstance().GetIndices().data());
//  SaveSettings_();
//  glDisable(GL_LINE_STIPPLE);
//  glDisable(GL_POINT_SMOOTH);
//  glDisableClientState(GL_VERTEX_ARRAY);
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
//    y_rot_ = y_rot_ + (mouse->pos().x() - start_x_);
//    x_rot_ = x_rot_ + (mouse->pos().y() - start_y_);
      float tmpX = mouse->position().x();
      float tmpY = mouse->position().y();

      float xoffset = tmpX - start_x_;
      float yoffset = start_y_ - tmpY;

      float sensitivity = 1.0f; // change this value to your liking
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      y_rot_ += -yoffset;
      y_rot_ = y_rot_ > 360.0f ? (y_rot_ - 360.0f) : y_rot_;
      y_rot_ = y_rot_ < -360.0f ? (y_rot_ + 360.0f) : y_rot_;

      x_rot_ += -xoffset;
      x_rot_ = x_rot_ > 360.0f ? (x_rot_ - 360.0f) : x_rot_;
      x_rot_ = x_rot_ < -360.0f ? (x_rot_ + 360.0f) : x_rot_;

      start_x_ = tmpX;
      start_y_ = tmpY;
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
//      projection = false;
      break;
    case Qt::Key_P:
      x_trans_ = 0, y_trans_ = 0, zoom_scale_ = -10;
//      projection = true;
      break;
  }
  update();
}

void scene::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        model.scale(QVector3D(1.1f, 1.1f, 1.1f));
    } else {
        model.scale(QVector3D(0.9f, 0.9f, 0.9f));
    }
  update();
}
