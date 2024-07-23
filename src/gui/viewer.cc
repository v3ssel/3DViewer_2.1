#include "viewer.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "../QGifImage/QGifImage/qgifimage.h"
#include "../scene/scene_builder.h"
#include "ui_viewer.h"

namespace s21 {
Viewer::Viewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::Viewer) {
  ui->setupUi(this);
  this->setWindowTitle("3DViewer 2.0");
  this->setFixedSize(width(), height());

  SceneBuilder sb;
  scene_ = sb.SetParent(ui->centralwidget)
               ->SetSceneSize(QRect(0, 15, 650, 650))
               ->Build();

  settings_ = new QSettings(QDir::homePath() + "/3DViewerConfig/settings.conf",
                            QSettings::IniFormat);
  LoadSettings();
  SetFrameColor();

  ui->horizontalSlider_lineWidth->setValue(scene_->line_width);
  ui->lcdNumber_lineWidth->display((int)scene_->line_width);
  ui->horizontalSlider_versize->setValue(scene_->vertex_size);
  ui->lcdNumber_versize->display((int)scene_->vertex_size);

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
  delete scene_;
  delete ui;
}

void Viewer::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_R:
      ui->spinBox_x_rot->setValue(0), ui->spinBox_y_rot->setValue(0),
          ui->spinBox_z_rot->setValue(0);

      ui->doubleSpinBox_x_move->setValue(0.0f),
          ui->doubleSpinBox_y_move->setValue(0.0f),
          ui->doubleSpinBox_z_move->setValue(0.0f);

      ui->horizontalSlider_scale->setValue(1.0f);
      ui->lcdNumber_scale->display(1.0f);
      scene_->ResetScene();
      break;

    case Qt::Key_P:
      ChangeProjection();
      break;
  }
}

void Viewer::OpenFile() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Choose File", QDir::homePath(), tr("OBJ (*.obj)"));
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

    scene_->InitModel(mesh_);
    scene_->update();
  } catch (const std::exception& ex) {
    QMessageBox::critical(this, "Error",
                          "An error occured during model loading."
                          "\nPlease ensure file correctness or try other file");
    mesh_ = tmp_mesh;
  }
}

void Viewer::CloseFile() {
  if (mesh_) {
    delete mesh_;
    mesh_ = nullptr;
  }

  scene_->ResetModel();
  scene_->update();
  filename_ = "";
}

void Viewer::FileInfo() {
  QMessageBox::information(
      this, "Information",
      "Filename: " + filename_ +
          "\nVertices: " + QString::number(scene_->VertexCount()) +
          "\nLines: " + QString::number(scene_->IndexCount() / 2));
}

void Viewer::ChangeProjection() {
  scene_->projection_type = !scene_->projection_type;
  scene_->update();
}

void Viewer::ChangeBackgroundColor() {
  scene_->background = QColorDialog::getColor();
  SetFrameColor();
}

void Viewer::ChangeVerticesColor() {
  scene_->vertices_color = QColorDialog::getColor();
}

void Viewer::ChangeLinesColor() {
  scene_->lines_color = QColorDialog::getColor();
}

void Viewer::LineWidthSliderMoved(int width) {
  scene_->line_width = width;
  ui->lcdNumber_lineWidth->display(width);
  scene_->update();
}

void Viewer::LineWidthSliderPressed() {
  LineWidthSliderMoved(ui->horizontalSlider_lineWidth->value());
}

void Viewer::VertexSizeSliderMoved(int position) {
  scene_->no_vertices = false;
  scene_->vertex_size = position;
  ui->lcdNumber_versize->display(position);
  scene_->update();
}

void Viewer::VertexSizeSliderPressed() {
  VertexSizeSliderMoved(ui->horizontalSlider_versize->value());
}

void Viewer::ScaleSliderMoved(int position) {
  scene_->ScaleModel(position);
  ui->lcdNumber_scale->display(position);
  scene_->update();
}

void Viewer::ScaleSliderPressed() {
  ScaleSliderMoved(ui->horizontalSlider_scale->value());
}

void Viewer::MakeSolidLines() {
  scene_->dashed_line = false;
  scene_->update();
}

void Viewer::MakeDashedLines() {
  scene_->dashed_line = true;
  scene_->update();
}

void Viewer::MakeVertexCircle() {
  scene_->no_vertices = false;
  scene_->circle_vertex = false;
  ui->horizontalSlider_versize->setValue(scene_->vertex_size);
  ui->lcdNumber_versize->display((int)scene_->vertex_size);
  scene_->update();
}

void Viewer::MakeVertexSquare() {
  scene_->no_vertices = false;
  scene_->circle_vertex = true;
  ui->horizontalSlider_versize->setValue(scene_->vertex_size);
  ui->lcdNumber_versize->display((int)scene_->vertex_size);
  scene_->update();
}

void Viewer::DisableVertices() {
  scene_->no_vertices = true;
  ui->horizontalSlider_versize->setValue(1);
  ui->lcdNumber_versize->display(1);
  scene_->update();
}

void Viewer::MoveObjectInX() {
  scene_->MoveModel(ui->doubleSpinBox_x_move->value(),
                    ui->doubleSpinBox_y_move->value(),
                    ui->doubleSpinBox_z_move->value());
}

void Viewer::MoveObjectInY() { MoveObjectInX(); }

void Viewer::MoveObjectInZ() { MoveObjectInX(); }

void Viewer::RotateObjectInX() {
  scene_->RotateModel(ui->spinBox_x_rot->value(), ui->spinBox_y_rot->value(),
                      ui->spinBox_z_rot->value());
}

void Viewer::RotateObjectInY() { RotateObjectInX(); }

void Viewer::RotateObjectInZ() { RotateObjectInX(); }

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
  if (str != "") scene_->grabFramebuffer().save(str);
}

void Viewer::Recording() {
  if (is_recording_ && time_ <= 5.0) {
    gif_images_.push_back(scene_->grab().toImage());
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
  settings_->setValue("dashed_line", scene_->dashed_line);
  settings_->setValue("projection", scene_->projection_type);
  settings_->setValue("circle_vertex", scene_->circle_vertex);
  settings_->setValue("no_vertices", scene_->no_vertices);
  settings_->endGroup();

  settings_->beginGroup("rgb");
  settings_->setValue("background_color", scene_->background);
  settings_->setValue("vertices_color", scene_->vertices_color);
  settings_->setValue("lines_color", scene_->lines_color);
  settings_->endGroup();

  settings_->beginGroup("size");
  settings_->setValue("line_width", scene_->line_width);
  settings_->setValue("vertex_size", scene_->vertex_size);
  settings_->endGroup();
}

void Viewer::LoadSettings() {
  settings_->beginGroup("coordinate");
  scene_->dashed_line = settings_->value("dashed_line", false).toBool();
  scene_->projection_type = settings_->value("projection", true).toBool();
  scene_->circle_vertex = settings_->value("circle_vertex", false).toBool();
  scene_->no_vertices = settings_->value("no_vertices", false).toBool();
  settings_->endGroup();

  settings_->beginGroup("rgb");
  scene_->background =
      settings_->value("background_color", QColor(0.0f, 0.0f, 0.0f, 0.0f))
          .value<QColor>();
  scene_->vertices_color =
      settings_->value("vertices_color", QColor(0.0f, 0.0f, 0.0f))
          .value<QColor>();
  scene_->lines_color =
      settings_->value("lines_color", QColor(255.0f, 0.0f, 45.0f))
          .value<QColor>();
  settings_->endGroup();

  settings_->beginGroup("size");
  scene_->line_width = settings_->value("line_width", 5).toUInt();
  scene_->vertex_size = settings_->value("vertex_size", 1).toUInt();
  settings_->endGroup();
}

void Viewer::SetFrameColor() {
  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(scene_->background.red()) + ", " +
                      QString::number(scene_->background.green()) + ", " +
                      QString::number(scene_->background.blue()) + ");}");
}

void Viewer::SetupConnections() {
  connect(ui->actionOpen, &QAction::triggered, this, &Viewer::OpenFile);
  connect(ui->actionClose, &QAction::triggered, this, &Viewer::CloseFile);
  connect(ui->actionInfo, &QAction::triggered, this, &Viewer::FileInfo);
  connect(ui->actionProjection, &QAction::triggered, this,
          &Viewer::ChangeProjection);
  connect(ui->actionJPEG, &QAction::triggered, this, &Viewer::SaveJpeg);
  connect(ui->actionBMP, &QAction::triggered, this, &Viewer::SaveBmp);
  connect(ui->actionGIF, &QAction::triggered, this, &Viewer::SaveGif);

  connect(ui->pushButton_bg, &QAbstractButton::clicked, this,
          &Viewer::ChangeBackgroundColor);
  connect(ui->pushButton_vertex, &QAbstractButton::clicked, this,
          &Viewer::ChangeVerticesColor);
  connect(ui->pushButton_lines, &QAbstractButton::clicked, this,
          &Viewer::ChangeLinesColor);

  connect(ui->horizontalSlider_lineWidth, &QSlider::sliderMoved, this,
          &Viewer::LineWidthSliderMoved);
  connect(ui->horizontalSlider_lineWidth, &QSlider::sliderPressed, this,
          &Viewer::LineWidthSliderPressed);
  connect(ui->horizontalSlider_versize, &QSlider::sliderMoved, this,
          &Viewer::VertexSizeSliderMoved);
  connect(ui->horizontalSlider_versize, &QSlider::sliderPressed, this,
          &Viewer::VertexSizeSliderPressed);
  connect(ui->horizontalSlider_scale, &QSlider::sliderMoved, this,
          &Viewer::ScaleSliderMoved);
  connect(ui->horizontalSlider_scale, &QSlider::sliderPressed, this,
          &Viewer::ScaleSliderPressed);

  connect(ui->pushButton_line_solid, &QAbstractButton::clicked, this,
          &Viewer::MakeSolidLines);
  connect(ui->pushButton_line_dashed, &QAbstractButton::clicked, this,
          &Viewer::MakeDashedLines);
  connect(ui->pushButton_ver_circle, &QAbstractButton::clicked, this,
          &Viewer::MakeVertexCircle);
  connect(ui->pushButton_ver_square, &QAbstractButton::clicked, this,
          &Viewer::MakeVertexSquare);
  connect(ui->pushButton_ver_none, &QAbstractButton::clicked, this,
          &Viewer::DisableVertices);

  connect(ui->doubleSpinBox_x_move, &QDoubleSpinBox::valueChanged, this,
          &Viewer::MoveObjectInX);
  connect(ui->doubleSpinBox_y_move, &QDoubleSpinBox::valueChanged, this,
          &Viewer::MoveObjectInY);
  connect(ui->doubleSpinBox_z_move, &QDoubleSpinBox::valueChanged, this,
          &Viewer::MoveObjectInZ);

  connect(ui->spinBox_x_rot, &QSpinBox::valueChanged, this,
          &Viewer::RotateObjectInX);
  connect(ui->spinBox_y_rot, &QSpinBox::valueChanged, this,
          &Viewer::RotateObjectInY);
  connect(ui->spinBox_z_rot, &QSpinBox::valueChanged, this,
          &Viewer::RotateObjectInZ);
  connect(record_time_, &QTimer::timeout, this, &Viewer::Recording);
}
}  // namespace s21
