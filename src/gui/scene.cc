#include "scene.h"

#include "viewer.h"

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent) {
    settings = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                             QSettings::IniFormat);

    model_pos = camera_target_ = QVector3D(0.0f, 0.0f, 0.0f);

    is_moving_ = false;
    projection_type = true;

    mesh_ = nullptr;
    scale_factor = 1.0f;
    start_x_ = 0.0f, start_y_ = 0.0f;
    x_rot_ = 1.0f, y_rot_ = 1.0f;
    prev_rotation = QVector3D(0.0f, 0.0f, 0.0f);

    LoadSettings();
}

Scene::~Scene() {
    program.bind();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    
    if (mesh_) {
        delete mesh_;
        mesh_ = nullptr;
    }

    delete settings;
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
    mesh_->Reset();
}

void Scene::RotateModel(float x, float y, float z) {
    float diff_x = x - prev_rotation.x();
    float diff_y = y - prev_rotation.y();
    float diff_z = z - prev_rotation.z();
    prev_rotation.setX(x);
    prev_rotation.setY(y);
    prev_rotation.setZ(z);

    double angle = QVector3D(diff_y, diff_x, diff_z).length();
    QVector3D axis = QVector3D(diff_y, diff_x, diff_z);
    rotation_ = QQuaternion::fromAxisAndAngle(axis, angle) * rotation_;
}


size_t Scene::VertexCount() { return mesh_->vertices.size(); }

size_t Scene::IndexCount() { return mesh_->indices.size(); }

void Scene::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_R:
            camera_pos_ = camera_up_ = model_pos =
                QVector3D(0.0f, 0.0f, 0.0f);
            CalculateCamera();
            rotation_ = QQuaternion();
            prev_rotation = QVector3D(0.0f, 0.0f, 0.0f);
            break;
        case Qt::Key_O:
            projection_type = false;
            break;
        case Qt::Key_P:
            projection_type = true;
            break;
    }

    update();
}

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


    program.setUniformValueArray("view", &view, 1);
    view.setToIdentity();
    CalculateCamera();
    view.lookAt(camera_pos_, camera_target_, camera_up_);

    projection.setToIdentity();
    projection_type
        ? projection.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f)
        : projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    program.setUniformValueArray("projection", &projection, 1);

    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(model_pos);
    model.rotate(rotation_);
    model.scale(scale_factor);
    program.setUniformValueArray("model", &model, 1);

    DrawModel();

    SaveSettings(); // dtor?
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
    if (dashed_solid) {
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
    }
    glDrawElements(GL_TRIANGLES, mesh_->indices.size(), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_LINE_STIPPLE);

    if (!no_vertices) {
        if (!circle_square) glEnable(GL_POINT_SMOOTH);
        glPointSize(vertex_size);
        QVector3D v_col(vertices_color.red() / 255.0f,
                        vertices_color.green() / 255.0f,
                        vertices_color.blue() / 255.0f);
        program.setUniformValueArray("objectColor", &v_col, 1);

        glDrawArrays(GL_POINTS, 0, mesh_->indices.size());
        glDisable(GL_POINT_SMOOTH);
    }
}

void Scene::CalculateCamera() {
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

    start_x_ = mouse->pos().x();
    start_y_ = mouse->pos().y();
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
    start_x_ = mouse->pos().x();
    start_y_ = mouse->pos().y();

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
    settings->beginGroup("coordinate");
    settings->setValue("dashed_solid", dashed_solid);
    settings->setValue("projection", projection_type);
    settings->setValue("circle_square", circle_square);
    settings->setValue("no_vertices", no_vertices);
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

void Scene::LoadSettings() {
    settings->beginGroup("coordinate");
    dashed_solid = settings->value("dashed_solid", false).toBool();
    projection_type = settings->value("projection", true).toBool();
    circle_square = settings->value("circle_square", false).toBool();
    no_vertices = settings->value("no_vertices", false).toBool();
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
