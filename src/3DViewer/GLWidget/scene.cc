#include "scene.h"

#include "../Viewer/viewer.h"

scene::scene(QWidget* parent) : QOpenGLWidget(parent) {
  settings = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                           QSettings::IniFormat);

  move_object = camera_target_ = QVector3D(0.0f, 0.0f, 0.0f);
  moving_ = false, dragging_ = false;
  wireframe = true, flat_shading = false;
  projection_type = true, is_light_enabled = false;

  light_pos = QVector3D(1.0f, 1.0f, 1.0f);
  light_color = QVector3D(1.0f, 1.0f, 1.0f);

  texture = nullptr;
  scale_factor = 1.0f;
  start_x_ = 0, start_y_ = 0;
  x_rot_ = 1, y_rot_ = 1;
  x_trans_ = 0, y_trans_ = 0; // not needed now

  LoadSettings_();
}

scene::~scene() {
}

void scene::SaveSettings_() {
  settings->beginGroup("coordinate");
  settings->setValue("dashed_solid", dashed_solid);
  settings->setValue("projection", projection_type);
  settings->setValue("circle_square", circle_square);
  settings->setValue("is_none", is_none);
  settings->endGroup();

  settings->beginGroup("rgb");
  settings->setValue("background_color", background);
  settings->setValue("vertices_color", vertices_color);
  settings->setValue("lines_color", lines_color);
  settings->endGroup();

  settings->beginGroup("size");
  settings->setValue("line_width", line_width);
  settings->setValue("vertex_size", vertex_size);
  settings->endGroup();
}

void scene::LoadSettings_() {
  settings->beginGroup("coordinate");
  dashed_solid = settings->value("dashed_solid", false).toBool();
  projection_type = settings->value("projection", true).toBool();
  circle_square = settings->value("circle_square", false).toBool();
  is_none = settings->value("is_none", false).toBool();
  settings->endGroup();

  settings->beginGroup("rgb");
  background = settings->value("background_color", QColor(0.0f, 0.0f, 0.0f, 0.0f)).value<QColor>();
  vertices_color = settings->value("vertices_color", QColor(0.0f, 0.0f, 0.0f)).value<QColor>();
  lines_color = settings->value("lines_color", QColor(255.0f, 0.0f, 45.0f)).value<QColor>();
  settings->endGroup();

  settings->beginGroup("size");
  line_width = settings->value("line_width", 5).toUInt();
  vertex_size = settings->value("vertex_size", 1).toUInt();
  settings->endGroup();
}

void scene::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vert.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/frag.glsl");
    program.link();

    light.create();
    light.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/light_vert.glsl");
    light.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/light_frag.glsl");
    light.link();

    vao.create();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    vao.bind();
    vao.release();

    LightInit_();
}

void scene::LightInit_() {
    vao_light.create();
    QOpenGLBuffer vbo_light = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo_light.create();
    vbo_light.setUsagePattern(QOpenGLBuffer::StaticDraw);

    GLfloat lamp_vertices[] = {
        1.000000,  1.000000, -1.000000,
       -1.000000, 1.000000,  -1.000000,
       -1.000000, 1.000000,   1.000000,
        1.000000,  1.000000, -1.000000,
       -1.000000, 1.000000, 1.000000,
        1.000000,  1.000000, 1.000000,
        1.000000, -1.000000, 1.000000,
        1.000000,  1.000000, 1.000000,
       -1.000000, 1.000000, 1.000000,
        1.000000, -1.000000, 1.000000,
        -1.000000, 1.000000, 1.000000,
        -1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, 1.000000,
        -1.000000, 1.000000, 1.000000,
        -1.000000, 1.000000, -1.000000,
        -1.000000, -1.000000, 1.000000,
        -1.000000, 1.000000, -1.000000,
        -1.000000, -1.000000, -1.000000,
        -1.000000, -1.000000, -1.000000,
        1.000000, -1.000000, -1.000000,
        1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, -1.000000,
        1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, 1.000000,
        1.000000, -1.000000, -1.000000,
        1.000000, 1.000000, -1.000000,
        1.000000, 1.000000, 1.000000,
        1.000000, -1.000000, -1.000000,
        1.000000, 1.000000, 1.000000,
        1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, -1.000000,
        -1.000000, 1.000000, -1.000000,
        1.000000, 1.000000, -1.000000,
        -1.000000, -1.000000, -1.000000,
        1.000000, 1.000000, -1.000000,
        1.000000, -1.000000, -1.000000
    };

    vao_light.bind();

    vbo_light.bind();
    vbo_light.allocate(lamp_vertices, sizeof(GLfloat) * 36 * 3);

    light.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    light.enableAttributeArray("aPos");
    light.bind();

    vao_light.release();
    vbo_light.release();
    light.release();
}

void scene::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void scene::InitModel(QVector<GLfloat>& vertices, QVector<GLuint>& indices) {
    vao.bind();

    vbo.bind();
    vbo.allocate(vertices.data(), sizeof(vertices[0]) * vertices.size());

    program.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, 8 * sizeof(float));
    program.setAttributeBuffer("aTexCoord", GL_FLOAT, 3 * sizeof(float), 2, 8 * sizeof(float));
    program.setAttributeBuffer("aNormal", GL_FLOAT, 5 * sizeof(float), 3, 8 * sizeof(float));

    program.enableAttributeArray("aPos");
    program.enableAttributeArray("aTexCoord");
    program.enableAttributeArray("aNormal");

    ebo.bind();
    ebo.allocate(indices.data(), sizeof(indices[0]) * indices.size());

    program.bind();

    vao.release();
    ebo.release();
    vbo.release();
    program.release();
}

void scene::paintGL() {
    glClearColor(background.red() / 255.0f, background.green() / 255.0f, background.blue() / 255.0f, background.alpha() / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CalculateCamera();

    program.bind();
    CheckDisplayType_();

    QMatrix4x4 model, lamp;
    program.setUniformValueArray("view", &view, 1);
    projection.setToIdentity();
    view.setToIdentity();

    projection_type ? projection.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f)
                    : projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    view.lookAt(camera_pos_, camera_target_, camera_up_);

    program.setUniformValueArray("projection", &projection, 1);

    model.setToIdentity();
    model.translate(move_object);
    model.scale(scale_factor);
    program.setUniformValueArray("model", &model, 1);

    QMatrix4x4 inversed_transposed_model = model.inverted().transposed();
    program.setUniformValueArray("it_model", &inversed_transposed_model, 1);

    if (texture) texture->bind();

    StartDraw_();
    program.release();

    DrawLight_(lamp);
    SaveSettings_();
}

void scene::CheckDisplayType_() {
    if (texture && !wireframe) {
        program.setUniformValue("is_textured", true);
    } else {
        program.setUniformValue("is_textured", false);
        QVector3D ocol(lines_color.red() / 255.0f, lines_color.green() / 255.0f, lines_color.blue() / 255.0f);
        program.setUniformValueArray("objectColor", &ocol, 1);
    }

    if (has_normals && !wireframe && is_light_enabled) {
        program.setUniformValue("have_normals", true);
        program.setUniformValueArray("lightColor", &light_color, 1);
        program.setUniformValueArray("lightPos", &light_pos, 1);
        program.setUniformValueArray("viewPos", &camera_pos_, 1);
        program.setUniformValue("flat_shading", flat_shading ? true : false);
    } else {
        program.setUniformValue("have_normals", false);
    }
}

void scene::StartDraw_() {
    vao.bind();
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    glLineWidth(line_width);
    if (dashed_solid) {
      glLineStipple(1, 0x00FF);
      glEnable(GL_LINE_STIPPLE);
    }
    glDrawElements(GL_TRIANGLES , s21::Controller::GetInstance().GetIndices().size(), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_LINE_STIPPLE);

    if (!is_none) {
        if (!circle_square) glEnable(GL_POINT_SMOOTH);
        glPointSize(vertex_size);
        QVector3D v_col(vertices_color.red() / 255.0f, vertices_color.green() / 255.0f, vertices_color.blue() / 255.0f);
        program.setUniformValueArray("objectColor", &v_col, 1);
        glDrawArrays(GL_POINTS, 1, s21::Controller::GetInstance().GetVertices().size() * 3);
        glDisable(GL_POINT_SMOOTH);
    }

    vao.release();
}

void scene::DrawLight_(QMatrix4x4& lamp) {
    if (has_normals && !wireframe && is_light_enabled) {
        light.bind();
        vao_light.bind();

        light.setUniformValueArray("projection", &projection, 1);
        light.setUniformValueArray("view", &view, 1);

        lamp.setToIdentity();
        lamp.translate(light_pos);
        lamp.scale(0.1f);
        light.setUniformValueArray("model", &lamp, 1);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        vao_light.release();
        light.release();
    }
}

void scene::CalculateCamera() {
    float r = 3.0f * cos(y_rot_ * M_PI / 180);
    camera_pos_   = QVector3D(camera_target_.x() + r * sin(x_rot_ * M_PI / 180),
                            camera_target_.y() + 3.0f * sin(y_rot_ * M_PI / 180),
                            camera_target_.z() + r * cos(x_rot_ * M_PI / 180)) + camera_target_;

    camera_up_    = QVector3D(-sin(x_rot_ * M_PI / 180) * sin(y_rot_ * M_PI / 180),
                            cos(y_rot_ * M_PI / 180),
                            -cos(x_rot_ * M_PI / 180) * sin(y_rot_ * M_PI / 180));
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
      camera_target_ = QVector3D(mouse->pos().x() - start_x_,(mouse->pos().y() - start_y_), 1.0f);
//    x_trans_ = x_trans_ + (mouse->pos().x() - start_x_) / 4.f;
//    y_trans_ = y_trans_ - (mouse->pos().y() - start_y_) / 4.f;
  }
  start_x_ = mouse->pos().x();
  start_y_ = mouse->pos().y();
  update();
}

void scene::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_R:
      x_trans_ = 0, y_trans_ = 0;
      break;
    case Qt::Key_O:
      x_trans_ = 0, y_trans_ = 0;
      projection_type = false;
      break;
    case Qt::Key_P:
      x_trans_ = 0, y_trans_ = 0;
      projection_type = true;
      break;
  }
  update();
}

void scene::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        scale_factor *= 1.1f;
    } else {
        scale_factor *= 0.9f;
    }
    update();
}
