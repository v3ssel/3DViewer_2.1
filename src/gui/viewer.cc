#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "viewer.h"
#include "ui_viewer.h"

#include "../QGifImage/QGifImage/qgifimage.h"
#include "../controller/controller.h"

namespace s21 {
Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
    ui->setupUi(this);
    this->setWindowTitle("3DViewer 2.0");
    this->setFixedSize(width(), height());

    scene = Controller::Instance().GetScene(ui->centralwidget, QRect(0, 15, 650, 650));

    settings_ = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                             QSettings::IniFormat);
    LoadSettings();
    SetFrameColor();

    ui->horizontalSlider_lineWidth->setValue(scene->line_width);
    ui->lcdNumber_lineWidth->display((int)scene->line_width);
    ui->horizontalSlider_versize->setValue(scene->vertex_size);
    ui->lcdNumber_versize->display((int)scene->vertex_size);

    mesh_ = nullptr;
    is_recording_ = false;
    time_ = 0.0;

    record_time_ = new QTimer(this);
    SetupConnections();
}

Viewer::~Viewer() {
    SaveSettings();
    if (mesh_) delete mesh_;

    delete settings_;
    delete record_time_;
    delete scene;
    delete ui;
}

void Viewer::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_R:
            ui->spinBox_x_rot->setValue(0),
            ui->spinBox_y_rot->setValue(0),
            ui->spinBox_z_rot->setValue(0);

            ui->doubleSpinBox_x_move->setValue(0.0f),
            ui->doubleSpinBox_y_move->setValue(0.0f),
            ui->doubleSpinBox_z_move->setValue(0.0f);

            ui->horizontalSlider_scale->setValue(1.0f);
            ui->lcdNumber_scale->display(1.0f);
            scene->ResetScene();
            break;

        case Qt::Key_P:
            scene->ChangeProjectionType();
            break;
    }
}

void Viewer::OpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Choose File", QDir::homePath(), tr("OBJ (*.obj)"));
    if (filename == "") {
        return;
    }

    filename_ = filename;
    s21::Mesh* tmp_mesh = mesh_;

    try {
        mesh_ = Controller::Instance().ParseMeshFromFile(filename);
        if (tmp_mesh) {
            delete tmp_mesh;
        }

        scene->InitModel(mesh_);
        scene->update();
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", "An error occured during model loading."
                                    "\nPlease ensure file correctness or try other file");
        mesh_ = tmp_mesh;
    }
}

void Viewer::CloseFile() {
    if (mesh_) {
        delete mesh_;
        mesh_ = nullptr;
    }

    scene->ResetModel();
    scene->update();
    filename_ = "";
}

void Viewer::FileInfo() {
    QMessageBox::information(
        this, "Information",
        "Filename: " + filename_ +
        "\nVertices: " + QString::number(scene->VertexCount()) +
        "\nLines: " + QString::number(scene->IndexCount() / 2));
}

void Viewer::ChangeProjection() {
    scene->ChangeProjectionType();
}

void Viewer::ChangeBackgroundColor() {
    scene->background = QColorDialog::getColor();
    SetFrameColor();
}

void Viewer::ChangeVerticesColor() {
    scene->vertices_color = QColorDialog::getColor();
}

void Viewer::ChangeLinesColor() {
    scene->lines_color = QColorDialog::getColor();
}

void Viewer::LineWidthSliderMoved(int width) {
    scene->line_width = width;
    ui->lcdNumber_lineWidth->display(width);
    scene->update();
}

void Viewer::LineWidthSliderPressed() {
    LineWidthSliderMoved(ui->horizontalSlider_lineWidth->value());
}

void Viewer::VertexSizeSliderMoved(int position) {
    scene->no_vertices = false;
    scene->vertex_size = position;
    ui->lcdNumber_versize->display(position);
    scene->update();
}

void Viewer::VertexSizeSliderPressed() {
    VertexSizeSliderMoved(ui->horizontalSlider_versize->value());
}

void Viewer::ScaleSliderMoved(int position) {
    scene->ScaleModel(position);
    ui->lcdNumber_scale->display(position);
    scene->update();
}

void Viewer::ScaleSliderPressed() {
    ScaleSliderMoved(ui->horizontalSlider_scale->value());
}

void Viewer::MakeSolidLines() {
    scene->dashed_line = false;
    scene->update();
}

void Viewer::MakeDashedLines() {
    scene->dashed_line = true;
    scene->update();
}

void Viewer::MakeVertexCircle() {
    scene->no_vertices = false;
    scene->circle_vertex = false;
    ui->horizontalSlider_versize->setValue(scene->vertex_size);
    ui->lcdNumber_versize->display((int)scene->vertex_size);
    scene->update();
}

void Viewer::MakeVertexSquare() {
    scene->no_vertices = false;
    scene->circle_vertex = true;
    ui->horizontalSlider_versize->setValue(scene->vertex_size);
    ui->lcdNumber_versize->display((int)scene->vertex_size);
    scene->update();
}

void Viewer::DisableVertices() {
    scene->no_vertices = true;
    ui->horizontalSlider_versize->setValue(1);
    ui->lcdNumber_versize->display(1);
    scene->update();
}

void Viewer::MoveObjectInX() {
    scene->MoveModel(ui->doubleSpinBox_x_move->value(),
                          ui->doubleSpinBox_y_move->value(),
                          ui->doubleSpinBox_z_move->value());
}

void Viewer::MoveObjectInY() {
    MoveObjectInX();
}

void Viewer::MoveObjectInZ() {
    MoveObjectInX();
}

void Viewer::RotateObjectInX() {
    scene->RotateModel(ui->spinBox_x_rot->value(),
                            ui->spinBox_y_rot->value(),
                            ui->spinBox_z_rot->value());
}

void Viewer::RotateObjectInY() {
    RotateObjectInX();
}

void Viewer::RotateObjectInZ() {
    RotateObjectInX();
}

void Viewer::SaveJpeg() { SaveImage("*.jpeg"); }

void Viewer::SaveBmp() { SaveImage("*.bmp"); }

void Viewer::SaveGif() {
    if (!is_recording_) {
        this->setStyleSheet("QMainWindow{ background-color: red; }");
        is_recording_ = true;
        record_time_->start(100);
    }
}

void Viewer::SaveImage(QString format) {
    QString str = QFileDialog::getSaveFileName(this, "Save file as",
                                               QDir::homePath(), format);
    if (str != "") scene->grabFramebuffer().save(str);
}

void Viewer::Recording() {
    if (is_recording_ && time_ <= 5.0) {
        gif_images_.push_back(scene->grab().toImage());
        time_ += 0.1;
    } else {
        record_time_->stop();
        SaveFullGif();
    }
}

void Viewer::SaveFullGif() {
    time_ = 0.0;
    is_recording_ = false;
    SetFrameColor();

    QString str = QFileDialog::getSaveFileName(
        this, tr("Save GIF"), QDir::homePath(), tr("GIF (*.gif)"));
    
    if (str.isEmpty()) {
        return;
    }
    
    QGifImage gif(QSize(640, 480));

    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    for (auto& frame : gif_images_) {
        gif.addFrame(frame);
    }

    gif.save(str);
    gif_images_.clear();
}

void Viewer::SaveSettings() {
    settings_->beginGroup("coordinate");
    settings_->setValue("dashed_line", scene->dashed_line);
    settings_->setValue("projection", scene->projection_type);
    settings_->setValue("circle_vertex", scene->circle_vertex);
    settings_->setValue("no_vertices", scene->no_vertices);
    settings_->endGroup();

    settings_->beginGroup("rgb");
    settings_->setValue("background_color", scene->background);
    settings_->setValue("vertices_color", scene->vertices_color);
    settings_->setValue("lines_color", scene->lines_color);
    settings_->endGroup();

    settings_->beginGroup("size");
    settings_->setValue("line_width", scene->line_width);
    settings_->setValue("vertex_size", scene->vertex_size);
    settings_->endGroup();
}

void Viewer::LoadSettings() {
    settings_->beginGroup("coordinate");
    scene->dashed_line = settings_->value("dashed_line", false).toBool();
    scene->projection_type = settings_->value("projection", true).toBool();
    scene->circle_vertex = settings_->value("circle_vertex", false).toBool();
    scene->no_vertices = settings_->value("no_vertices", false).toBool();
    settings_->endGroup();

    settings_->beginGroup("rgb");
    scene->background = settings_->value("background_color", QColor(0.0f, 0.0f, 0.0f, 0.0f)).value<QColor>();
    scene->vertices_color = settings_->value("vertices_color", QColor(0.0f, 0.0f, 0.0f)).value<QColor>();
    scene->lines_color = settings_->value("lines_color", QColor(255.0f, 0.0f, 45.0f)).value<QColor>();
    settings_->endGroup();

    settings_->beginGroup("size");
    scene->line_width = settings_->value("line_width", 5).toUInt();
    scene->vertex_size = settings_->value("vertex_size", 1).toUInt();
    settings_->endGroup();
}

void Viewer::SetFrameColor() {
    this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                        QString::number(scene->background.red()) + ", " +
                        QString::number(scene->background.green()) + ", " +
                        QString::number(scene->background.blue()) + ");}");
}

void Viewer::SetupConnections() {
    connect(ui->actionOpen, &QAction::triggered, this, &Viewer::OpenFile);
    connect(ui->actionClose, &QAction::triggered, this, &Viewer::CloseFile);
    connect(ui->actionInfo, &QAction::triggered, this, &Viewer::FileInfo);
    connect(ui->actionProjection, &QAction::triggered, this, &Viewer::ChangeProjection);
    connect(ui->actionJPEG, &QAction::triggered, this, &Viewer::SaveJpeg);
    connect(ui->actionBMP, &QAction::triggered, this, &Viewer::SaveBmp);
    connect(ui->actionGIF, &QAction::triggered, this, &Viewer::SaveGif);

    connect(ui->pushButton_bg, &QAbstractButton::clicked, this, &Viewer::ChangeBackgroundColor);
    connect(ui->pushButton_vertex, &QAbstractButton::clicked, this, &Viewer::ChangeVerticesColor);
    connect(ui->pushButton_lines, &QAbstractButton::clicked, this, &Viewer::ChangeLinesColor);

    connect(ui->horizontalSlider_lineWidth, &QSlider::sliderMoved, this, &Viewer::LineWidthSliderMoved);
    connect(ui->horizontalSlider_lineWidth, &QSlider::sliderPressed, this, &Viewer::LineWidthSliderPressed);
    connect(ui->horizontalSlider_versize, &QSlider::sliderMoved, this, &Viewer::VertexSizeSliderMoved);
    connect(ui->horizontalSlider_versize, &QSlider::sliderPressed, this, &Viewer::VertexSizeSliderPressed);
    connect(ui->horizontalSlider_scale, &QSlider::sliderMoved, this, &Viewer::ScaleSliderMoved);
    connect(ui->horizontalSlider_scale, &QSlider::sliderPressed, this, &Viewer::ScaleSliderPressed);

    connect(ui->pushButton_line_solid, &QAbstractButton::clicked, this, &Viewer::MakeSolidLines);
    connect(ui->pushButton_line_dashed, &QAbstractButton::clicked, this, &Viewer::MakeDashedLines);
    connect(ui->pushButton_ver_circle, &QAbstractButton::clicked, this, &Viewer::MakeVertexCircle);
    connect(ui->pushButton_ver_square, &QAbstractButton::clicked, this, &Viewer::MakeVertexSquare);
    connect(ui->pushButton_ver_none, &QAbstractButton::clicked, this, &Viewer::DisableVertices);

    connect(ui->doubleSpinBox_x_move, &QDoubleSpinBox::valueChanged, this, &Viewer::MoveObjectInX);
    connect(ui->doubleSpinBox_y_move, &QDoubleSpinBox::valueChanged, this, &Viewer::MoveObjectInY);
    connect(ui->doubleSpinBox_z_move, &QDoubleSpinBox::valueChanged, this, &Viewer::MoveObjectInZ);

    connect(ui->spinBox_x_rot, &QSpinBox::valueChanged, this, &Viewer::RotateObjectInX);
    connect(ui->spinBox_y_rot, &QSpinBox::valueChanged, this, &Viewer::RotateObjectInY);
    connect(ui->spinBox_z_rot, &QSpinBox::valueChanged, this, &Viewer::RotateObjectInZ);
    connect(record_time_, &QTimer::timeout, this, &Viewer::Recording);
}
}
