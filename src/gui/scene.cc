#include "scene.h"

#include "viewer.h"

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent) {
    settings_ = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                             QSettings::IniFormat);

    model_pos = camera_target_ = QVector3D(0.0f, 0.0f, 0.0f);

    is_moving_ = false;
    projection_type = true;

    mesh_ = nullptr;
    scale_factor = 1.0f;
    start_x_ = 0.0f, start_y_ = 0.0f;
    x_rot_ = 1.0f, y_rot_ = 1.0f;
    prev_rotation_ = QVector3D(0.0f, 0.0f, 0.0f);

    LoadSettings();
}

Scene::~Scene() {
    SaveSettings();

    program.bind();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    
    if (mesh_) {
        delete mesh_;
        mesh_ = nullptr;
    }

    delete settings_;
}

void Scene::InitModel(const QString& filename) {
    if (mesh_) {
        delete mesh_;
        mesh_ = nullptr;
    }

    mesh_ = s21::Controller::Instance().ParseMeshFromFile(filename);

    program.bind();
    vao.bind();

    vbo.bind();
    vbo.allocate(mesh_->facets.data(), sizeof(mesh_->facets[0]) * mesh_->facets.size());

    ebo.bind();
    ebo.allocate(mesh_->indices.data(), sizeof(mesh_->indices[0]) * mesh_->indices.size());
}

void Scene::ResetModel() {
    if (mesh_) mesh_->Reset();
}

void Scene::MoveModel(float x, float y, float z) {
    model_pos.setX(x);
    model_pos.setY(y);
    model_pos.setZ(z);
}

void Scene::RotateModel(float x, float y, float z) {
    float diff_x = x - prev_rotation_.x();
    float diff_y = y - prev_rotation_.y();
    float diff_z = z - prev_rotation_.z();
    prev_rotation_.setX(x);
    prev_rotation_.setY(y);
    prev_rotation_.setZ(z);

    QVector3D axis = QVector3D(diff_y, diff_x, diff_z);
    rotation = QQuaternion::fromAxisAndAngle(axis, axis.length()) * rotation;
}

void Scene::ScaleModel(float scale) {
    scale_factor = scale;
}

void Scene::ResetScene() {
    camera_pos_ = camera_up_ = model_pos = QVector3D(0.0f, 0.0f, 0.0f);
    SetCamera();
    rotation = QQuaternion();
    prev_rotation_ = QVector3D(0.0f, 0.0f, 0.0f);
}

void Scene::ChangeProjectionType() {
    projection_type = !projection_type;
}

size_t Scene::VertexCount() { return mesh_->vertices.size(); }

size_t Scene::IndexCount() { return mesh_->indices.size(); }

void Scene::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    LoadShaders();

    program.bind();
    vao.create();
    vao.bind();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    program.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, 8 * sizeof(float));
    program.enableAttributeArray("aPos");

    ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.bind();
    ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
}

void Scene::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void Scene::paintGL() {
    glClearColor(background.red() / 255.0f, background.green() / 255.0f,
                 background.blue() / 255.0f, background.alpha() / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!mesh_) return;

    program.bind();
    vao.bind();

    QVector3D linesColor(
                    lines_color.red() / 255.0f,
                    lines_color.green() / 255.0f,
                    lines_color.blue() / 255.0f);
    program.setUniformValueArray("objectColor", &linesColor, 1);


    SetCamera();
    view.setToIdentity();
    view.lookAt(camera_pos_, camera_target_, camera_up_);
    program.setUniformValueArray("view", &view, 1);

    projection.setToIdentity();
    projection_type
        ? projection.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f)
        : projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    program.setUniformValueArray("projection", &projection, 1);

    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(model_pos);
    model.rotate(rotation);
    model.scale(scale_factor);
    program.setUniformValueArray("model", &model, 1);

    DrawModel();
}

void Scene::LoadShaders() {
    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/vertex.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/fragment.glsl");
    if (!program.link()) {
        QMessageBox::critical(this, "Error", "Shader program error" + program.log());
    }
}

void Scene::DrawModel() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glLineWidth(line_width);
    if (dashed_line) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }
    glDrawElements(GL_TRIANGLES, mesh_->indices.size(), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_LINE_STIPPLE);

    if (!no_vertices) {
        if (!circle_vertex) glEnable(GL_POINT_SMOOTH);
        glPointSize(vertex_size);
        QVector3D v_col(vertices_color.red() / 255.0f,
                        vertices_color.green() / 255.0f,
                        vertices_color.blue() / 255.0f);
        program.setUniformValueArray("objectColor", &v_col, 1);

        glDrawArrays(GL_POINTS, 0, mesh_->indices.size());
        glDisable(GL_POINT_SMOOTH);
    }
}

void Scene::SetCamera() {
    float r = 3.0f * cos(y_rot_ * M_PI / 180);
    camera_pos_ =
        QVector3D(camera_target_.x() + r * sin(x_rot_ * M_PI / 180),
                  camera_target_.y() + 3.0f * sin(y_rot_ * M_PI / 180),
                  camera_target_.z() + r * cos(x_rot_ * M_PI / 180)) +
        camera_target_;

    camera_up_ =
        QVector3D(-sin(x_rot_ * M_PI / 180) * sin(y_rot_ * M_PI / 180),
                  cos(y_rot_ * M_PI / 180),
                  -cos(x_rot_ * M_PI / 180) * sin(y_rot_ * M_PI / 180));
}

void Scene::mousePressEvent(QMouseEvent* mouse) {
    switch (mouse->button()) {
        case Qt::LeftButton:
            is_moving_ = true;
            break;
        default:
            is_moving_ = false;
            break;
    }

    start_x_ = mouse->position().x();
    start_y_ = mouse->position().y();
}

void Scene::mouseMoveEvent(QMouseEvent* mouse) {
    if (is_moving_) {
        float tmpX = mouse->position().x();
        float tmpY = mouse->position().y();

        float xoffset = tmpX - start_x_;
        float yoffset = start_y_ - tmpY;

        float sensitivity = 1.0f;  // change this value to your liking
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
    
    start_x_ = mouse->position().x();
    start_y_ = mouse->position().y();

    update();
}

void Scene::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        scale_factor *= 1.1f;
    } else {
        scale_factor *= 0.9f;
    }

    update();
}

void Scene::SaveSettings() {
    settings_->beginGroup("coordinate");
    settings_->setValue("dashed_line", dashed_line);
    settings_->setValue("projection", projection_type);
    settings_->setValue("circle_vertex", circle_vertex);
    settings_->setValue("no_vertices", no_vertices);
    settings_->endGroup();

    settings_->beginGroup("rgb");
    settings_->setValue("background_color", background);
    settings_->setValue("vertices_color", vertices_color);
    settings_->setValue("lines_color", lines_color);
    settings_->endGroup();

    settings_->beginGroup("size");
    settings_->setValue("line_width", line_width);
    settings_->setValue("vertex_size", vertex_size);
    settings_->endGroup();
}

void Scene::LoadSettings() {
    settings_->beginGroup("coordinate");
    dashed_line = settings_->value("dashed_line", false).toBool();
    projection_type = settings_->value("projection", true).toBool();
    circle_vertex = settings_->value("circle_vertex", false).toBool();
    no_vertices = settings_->value("no_vertices", false).toBool();
    settings_->endGroup();

    settings_->beginGroup("rgb");
    background = settings_->value("background_color", QColor(0.0f, 0.0f, 0.0f, 0.0f)).value<QColor>();
    vertices_color = settings_->value("vertices_color", QColor(0.0f, 0.0f, 0.0f)).value<QColor>();
    lines_color = settings_->value("lines_color", QColor(255.0f, 0.0f, 45.0f)).value<QColor>();
    settings_->endGroup();

    settings_->beginGroup("size");
    line_width = settings_->value("line_width", 5).toUInt();
    vertex_size = settings_->value("vertex_size", 1).toUInt();
    settings_->endGroup();
}
