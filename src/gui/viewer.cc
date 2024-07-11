#include "viewer.h"
#include "ui_viewer.h"

#include "../QGifImage/QGifImage/qgifimage.h"
#include "../controller/controller.h"

Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
    ui->setupUi(this);
    this->setWindowTitle("3DViewer 2.0");
    this->setFixedSize(width(), height());
    SetFrameColor();

    ui->horizontalSlider_lineWidth->setValue(ui->widget->line_width);
    ui->lcdNumber_lineWidth->display((int)ui->widget->line_width);
    ui->horizontalSlider_versize->setValue(ui->widget->vertex_size);
    ui->lcdNumber_versize->display((int)ui->widget->vertex_size);

    mesh_ = nullptr;
    is_recording_ = false;
    time_ = 0.0;

    record_time_ = new QTimer(this);
    SetupConnects();
}

Viewer::~Viewer() {
    if (mesh_) delete mesh_;

    delete record_time_;
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
            ui->widget->ResetScene();
            break;

        case Qt::Key_P:
            ui->widget->ChangeProjectionType();
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
        mesh_ = s21::Controller::Instance().ParseMeshFromFile(filename);
        if (tmp_mesh) {
            delete tmp_mesh;
        }

        ui->widget->InitModel(mesh_);
        ui->widget->update();
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

    ui->widget->ResetModel();
    ui->widget->update();
    filename_ = "";
}

void Viewer::FileInfo() {
    size_t vertices =
            ui->widget->VertexCount() == 0 ? 0 : ui->widget->VertexCount() - 1;
    
    QMessageBox::information(
        this, "Information",
        "Filename: " + filename_ +
        "\nVertices: " + QString::number(vertices) +
        "\nLines: " + QString::number(ui->widget->IndexCount()));
}

void Viewer::ChangeProjection() {
    ui->widget->ChangeProjectionType();
}

void Viewer::ChangeBackgroundColor() {
    ui->widget->background = QColorDialog::getColor();
    SetFrameColor();
}

void Viewer::ChangeVerticesColor() {
    ui->widget->vertices_color = QColorDialog::getColor();
}

void Viewer::ChangeLinesColor() {
    ui->widget->lines_color = QColorDialog::getColor();
}

void Viewer::LineWidthSliderMoved(int width) {
    ui->widget->line_width = width;
    ui->lcdNumber_lineWidth->display(width);
    ui->widget->update();
}

void Viewer::LineWidthSliderPressed() {
    LineWidthSliderMoved(ui->horizontalSlider_lineWidth->value());
}

void Viewer::VertexSizeSliderMoved(int position) {
    ui->widget->no_vertices = false;
    ui->widget->vertex_size = position;
    ui->lcdNumber_versize->display(position);
    ui->widget->update();
}

void Viewer::VertexSizeSliderPressed() {
    VertexSizeSliderMoved(ui->horizontalSlider_versize->value());
}

void Viewer::ScaleSliderMoved(int position) {
    if (position > 0) {
        ui->widget->ScaleModel(position);
    } else {
        ui->widget->ScaleModel(1.0f - std::abs(position) / 100.0f);
    }

    ui->lcdNumber_scale->display(position);
    ui->widget->update();
}

void Viewer::ScaleSliderPressed() {
    ScaleSliderMoved(ui->horizontalSlider_scale->value());
}

void Viewer::MakeSolidLines() {
    ui->widget->dashed_line = false;
    ui->widget->update();
}

void Viewer::MakeDashedLines() {
    ui->widget->dashed_line = true;
    ui->widget->update();
}

void Viewer::MakeVertexCircle() {
    ui->widget->no_vertices = false;
    ui->widget->circle_vertex = false;
    ui->horizontalSlider_versize->setValue(ui->widget->vertex_size);
    ui->lcdNumber_versize->display((int)ui->widget->vertex_size);
    ui->widget->update();
}

void Viewer::MakeVertexSquare() {
    ui->widget->no_vertices = false;
    ui->widget->circle_vertex = true;
    ui->horizontalSlider_versize->setValue(ui->widget->vertex_size);
    ui->lcdNumber_versize->display((int)ui->widget->vertex_size);
    ui->widget->update();
}

void Viewer::DisableVertices() {
    ui->widget->no_vertices = true;
    ui->horizontalSlider_versize->setValue(1);
    ui->lcdNumber_versize->display(1);
    ui->widget->update();
}

void Viewer::MoveObjectInX() {
    ui->widget->MoveModel(ui->doubleSpinBox_x_move->value(),
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
    ui->widget->RotateModel(ui->spinBox_x_rot->value(),
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
        is_recording_ = true;
        this->setStyleSheet("QMainWindow{ background-color: red; }");
        record_time_->start(100);
    }
}

void Viewer::SaveImage(QString format) {
    QString str = QFileDialog::getSaveFileName(this, "Save file as",
                                               QDir::homePath(), format);
    if (str != "") ui->widget->grabFramebuffer().save(str);
}

void Viewer::Recording() {
    if (is_recording_ && time_ <= 5.0) {
        gif_images_.push_back(ui->widget->grab().toImage());
        time_ += 0.1;
    } else {
        record_time_->stop();
        SaveFullGif();
    }
}

void Viewer::SaveFullGif() {
    QString str = QFileDialog::getSaveFileName(
        this, tr("Save GIF"), QDir::homePath(), tr("GIF (*.gif)"));
    if (str != "") {
        QGifImage gif(QSize(640, 480));

        gif.setDefaultTransparentColor(Qt::black);
        gif.setDefaultDelay(100);

        for (QVector<QImage>::Iterator frame = gif_images_.begin();
             frame != gif_images_.end(); frame++) {
            gif.addFrame(*frame);
        }

        gif.save(str);
        gif_images_.clear();
    }
    time_ = 0.0;
    is_recording_ = false;

    SetFrameColor();
}

void Viewer::SetFrameColor() {
    this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                        QString::number(ui->widget->background.red()) + ", " +
                        QString::number(ui->widget->background.green()) + ", " +
                        QString::number(ui->widget->background.blue()) + ");}");
}

void Viewer::SetupConnects() {
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
