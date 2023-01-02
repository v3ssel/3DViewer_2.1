#include "viewer.h"

#include "ui_viewer.h"

viewer::viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::viewer) {
  ui->setupUi(this);
  this->setWindowTitle("3D Viewer 2.0");
  ui->horizontalSlider_lineWidth->setValue(ui->widget->line_width);
  ui->lcdNumber_lineWidth->display((int)ui->widget->line_width);
  ui->horizontalSlider_versize->setValue(ui->widget->vertex_size);
  ui->lcdNumber_versize->display((int)ui->widget->vertex_size);

  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(ui->widget->red_bg) + ", " +
                      QString::number(ui->widget->green_bg) + ", " +
                      QString::number(ui->widget->blue_bg) + ");}");

  hiden_ = false, is_recording_ = false;
  time_ = 0.0;
  record_time_ = new QTimer(this);
  connect(record_time_, &QTimer::timeout, this, &viewer::Recording_);
}

viewer::~viewer() { delete ui; }

void viewer::on_actionOpen_triggered() {
  QString fname = QFileDialog::getOpenFileName(
      this, "Choose File", QDir::homePath(), tr("OBJ (*.obj)"));
  if (fname != "") {
    ui->widget->filename = fname;
    ui->widget->update();

    s21::Controller::GetInstance().GetVertex().clear();
    s21::Controller::GetInstance().GetIndices().clear();
    s21::Controller::GetInstance().ParseVertex(fname.toStdString());
    s21::Controller::GetInstance().ParseIndices(fname.toStdString());

    m_texture = new QOpenGLTexture (QImage(":/Blocks.jpeg"));
  }
}

void viewer::on_actionClose_triggered() {
  if (!s21::Controller::GetInstance().GetVertex().empty()) {
    s21::Controller::GetInstance().GetVertex().clear();
    s21::Controller::GetInstance().GetIndices().clear();
    ui->widget->filename = "";
    ui->widget->update();
  }
}

void viewer::on_actionInfo_triggered() {
  QMessageBox::information(
      this, "Information",
      "Filename: " + ui->widget->filename + "\nVertices: " +
          QString::number(
              ui->widget->filename == ""
                  ? 0
                  : ((s21::Controller::GetInstance().GetVertex().size() - 3) /
                     3)) +
          "\nLines: " +
          QString::number(
              ui->widget->filename == ""
                  ? 0
                  : (s21::Controller::GetInstance().GetIndices().size() / 3 -
                     4)));
}

void viewer::keyPressEvent(QKeyEvent *event) {
  ui->widget->keyPressEvent(event);
}

void viewer::on_pushButton_bg_clicked() {
  QColor BG_color = QColorDialog::getColor();
  ui->widget->red_bg = BG_color.red();
  ui->widget->green_bg = BG_color.green();
  ui->widget->blue_bg = BG_color.blue();
  ui->widget->alpha_bg = BG_color.alpha();
  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(BG_color.red()) + ", " +
                      QString::number(BG_color.green()) + ", " +
                      QString::number(BG_color.blue()) + ");}");
}

void viewer::on_pushButton_vertex_clicked() {
  QColor ver_color = QColorDialog::getColor();
  ui->widget->red_vertex = ver_color.red();
  ui->widget->green_vertex = ver_color.green();
  ui->widget->blue_vertex = ver_color.blue();
}

void viewer::on_pushButton_lines_clicked() {
  QColor line_color = QColorDialog::getColor();
  ui->widget->red_lines = line_color.red();
  ui->widget->green_lines = line_color.green();
  ui->widget->blue_lines = line_color.blue();
}

void viewer::on_horizontalSlider_lineWidth_sliderMoved(int position) {
  ui->widget->line_width = position;
  ui->lcdNumber_lineWidth->display(position);
  ui->widget->update();
}

void viewer::on_horizontalSlider_lineWidth_sliderPressed() {
  on_horizontalSlider_lineWidth_sliderMoved(
      ui->horizontalSlider_lineWidth->value());
}

void viewer::on_horizontalSlider_versize_sliderMoved(int position) {
  ui->widget->is_none = false;
  ui->widget->vertex_size = position;
  ui->lcdNumber_versize->display(position);
  ui->widget->update();
}

void viewer::on_horizontalSlider_versize_sliderPressed() {
  on_horizontalSlider_versize_sliderMoved(
      ui->horizontalSlider_versize->value());
}

void viewer::on_pushButton_line_solid_clicked() {
  ui->widget->dashed_solid = false;
  ui->widget->update();
}

void viewer::on_pushButton_line_dashed_clicked() {
  ui->widget->dashed_solid = true;
  ui->widget->update();
}

void viewer::on_pushButton_ver_circle_clicked() {
  ui->widget->is_none = false;
  ui->widget->circle_square = false;
  ui->widget->update();
}

void viewer::on_pushButton_ver_square_clicked() {
  ui->widget->is_none = false;
  ui->widget->circle_square = true;
  ui->widget->update();
}

void viewer::on_pushButton_ver_none_clicked() {
  ui->widget->is_none = true;
  ui->widget->update();
}

void viewer::on_horizontalSlider_scale_sliderMoved(int position) {
  if (ui->widget->filename != "") {
    s21::Transform::GetInstance().Scale((double)position);
  }
  ui->lcdNumber_scale->display(position);
  ui->widget->update();
}

void viewer::on_doubleSpinBox_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    s21::Transform::GetInstance().Scale((double)arg1);
  }
  ui->widget->update();
}

void viewer::on_horizontalSlider_scale_sliderPressed() {
  on_doubleSpinBox_valueChanged(ui->horizontalSlider_scale->value());
}

void viewer::on_doubleSpinBox_x_move_valueChanged() {
  if (ui->widget->filename != "") {
    double x = ui->doubleSpinBox_x_move->value();
    double y = ui->doubleSpinBox_y_move->value();
    double z = ui->doubleSpinBox_z_move->value();

    s21::Transform::GetInstance().MoveXYZ(x, y, z);
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_y_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_z_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_x_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    s21::Transform::GetInstance().RotateXYZ('x',
                                            ui->doubleSpinBox_x_rot->value());
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_y_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    s21::Transform::GetInstance().RotateXYZ('y',
                                            ui->doubleSpinBox_y_rot->value());
    ui->widget->update();
  }
}

void viewer::on_doubleSpinBox_z_rot_valueChanged(double arg1) {
  if (ui->widget->filename != "") {
    s21::Transform::GetInstance().RotateXYZ('z',
                                            ui->doubleSpinBox_z_rot->value());
    ui->widget->update();
  }
}

void viewer::on_actionOrthographic_Perspective_triggered() {
  if (ui->widget->projection) {
    QKeyEvent *key = new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier);
    ui->widget->keyPressEvent(key);
    delete key;
  } else {
    QKeyEvent *key = new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier);
    ui->widget->keyPressEvent(key);
    delete key;
  }
}

void viewer::on_actionHide_triggered() {
  if (hiden_) {
    ui->dockWidget_main->show();
    hiden_ = false;
  } else {
    ui->dockWidget_main->hide();
    hiden_ = true;
  }
}

void viewer::SaveImage_(QString format) {
  QString str = QFileDialog::getSaveFileName(this, "Save file as",
                                             QDir::homePath(), format);
  if (str != "") ui->widget->grabFramebuffer().save(str);
}

void viewer::on_actionJPEG_triggered() { SaveImage_("*.jpeg"); }

void viewer::on_actionBMP_triggered() { SaveImage_("*.bmp"); }

void viewer::on_actionGIF_triggered() {
  if (!is_recording_) {
    is_recording_ = true;
    this->setStyleSheet("QMainWindow{ background-color: red;}");
    record_time_->start(100);
  }
}

void viewer::Recording_() {
  if (is_recording_ && time_ <= 5.0) {
    GIF_.push_back(ui->widget->grab().toImage());
    time_ += 0.1;
  } else {
    SaveGIF_();
    record_time_->stop();
  }
}

void viewer::SaveGIF_() {
  QString str = QFileDialog::getSaveFileName(
      this, tr("Save GIF"), QDir::homePath(), tr("GIF (*.gif)"));
  if (str != "") {
    QGifImage gif(QSize(640, 480));

    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    for (QVector<QImage>::Iterator frame = GIF_.begin(); frame != GIF_.end();
         frame++) {
      gif.addFrame(*frame);
    }

    gif.save(str);
    GIF_.clear();
  }
  time_ = 0.0;
  is_recording_ = false;

  this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                      QString::number(ui->widget->red_bg) + ", " +
                      QString::number(ui->widget->green_bg) + ", " +
                      QString::number(ui->widget->blue_bg) + ");}");
}
