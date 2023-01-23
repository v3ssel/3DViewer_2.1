#include "viewer.h"

#include "ui_viewer.h"

viewer::viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::viewer) {
  ui->setupUi(this);
  this->setWindowTitle("3D Viewer 2.1");
  ui->horizontalSlider_lineWidth->setValue(ui->widget->line_width);
  ui->lcdNumber_lineWidth->display((int)ui->widget->line_width);
  ui->horizontalSlider_versize->setValue(ui->widget->vertex_size);
  ui->lcdNumber_versize->display((int)ui->widget->vertex_size);

  ui->pushButton_apply_texture->setDisabled(true);
  ui->pushButton_unload_texture->setDisabled(true);
  ui->pushButton_save_uvmap->setDisabled(true);

  SetFrameColor();

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
    s21::Controller::GetInstance().clearArrays();
    s21::Controller::GetInstance().ParseVertex_3D(fname);
    ui->widget->InitModel(s21::Controller::GetInstance().GetPolygonsArray(), s21::Controller::GetInstance().GetIndices());

    on_pushButton_unload_texture_clicked();
    if ((ui->widget->has_normals = s21::Controller::GetInstance().NormalsUsage()))
        ui->actionLight->setDisabled(false);
    else
        ui->actionLight->setDisabled(true);

    if ((ui->widget->has_texture = s21::Controller::GetInstance().TextureUsage()) && !ui->widget->wireframe)
        ui->pushButton_apply_texture->setDisabled(false);
    else
        ui->pushButton_apply_texture->setDisabled(true);

    ui->widget->update();
  }
}

void viewer::on_actionClose_triggered() {
    s21::Controller::GetInstance().clearArrays();
    on_pushButton_unload_texture_clicked();
    ui->pushButton_apply_texture->setDisabled(true);
    ui->widget->has_texture = false;
    ui->widget->has_normals = false;
    ui->widget->filename = "";
    ui->widget->update();
}

void viewer::on_actionInfo_triggered() {
//  QMessageBox::information(
//      this, "Information",
//      "Filename: " + ui->widget->filename + "\nVertices: " +
//          QString::number(
//              ui->widget->filename == ""
//                  ? 0
//                  : ((s21::Controller::GetInstance().GetVertex().size() - 3) /
//                     3)) +
//          "\nLines: " +
//          QString::number(
//              ui->widget->filename == ""
//                  ? 0
//                  : (s21::Controller::GetInstance().GetIndices().size() / 3 -
//                     4)));
}

void viewer::keyPressEvent(QKeyEvent *event) {
    ui->widget->keyPressEvent(event);
}

void viewer::SetFrameColor() {
    this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                        QString::number(ui->widget->background.red()) + ", " +
                        QString::number(ui->widget->background.green()) + ", " +
                        QString::number(ui->widget->background.blue()) + ");}");
}

void viewer::on_pushButton_bg_clicked() {
  ui->widget->background = QColorDialog::getColor();
  SetFrameColor();
}

void viewer::on_pushButton_vertex_clicked() {
  ui->widget->vertices_color = QColorDialog::getColor();
}

void viewer::on_pushButton_lines_clicked() {
  ui->widget->lines_color = QColorDialog::getColor();
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
//  if (ui->widget->filename != "") {
//    s21::Transform::GetInstance().Scale((double)position);
//  }
//  ui->lcdNumber_scale->display(position);
//  ui->widget->update();
}

void viewer::on_doubleSpinBox_valueChanged(double arg1) {
//  if (ui->widget->filename != "") {
//    s21::Transform::GetInstance().Scale((double)arg1);
//  }
//  ui->widget->update();
}

void viewer::on_horizontalSlider_scale_sliderPressed() {
  on_doubleSpinBox_valueChanged(ui->horizontalSlider_scale->value());
}

void viewer::on_doubleSpinBox_x_move_valueChanged() {
//  if (ui->widget->filename != "") {
//    double x = ui->doubleSpinBox_x_move->value();
//    double y = ui->doubleSpinBox_y_move->value();
//    double z = ui->doubleSpinBox_z_move->value();

//    s21::Transform::GetInstance().MoveXYZ(x, y, z);
//    ui->widget->update();
//  }
}

void viewer::on_doubleSpinBox_y_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_z_move_valueChanged() {
  on_doubleSpinBox_x_move_valueChanged();
}

void viewer::on_doubleSpinBox_x_rot_valueChanged(double arg1) {
//  if (ui->widget->filename != "") {
//    s21::Transform::GetInstance().RotateXYZ('x',
//                                            ui->doubleSpinBox_x_rot->value());
//    ui->widget->update();
//  }
}

void viewer::on_doubleSpinBox_y_rot_valueChanged(double arg1) {
//  if (ui->widget->filename != "") {
//    s21::Transform::GetInstance().RotateXYZ('y',
//                                            ui->doubleSpinBox_y_rot->value());
//    ui->widget->update();
//  }
}

void viewer::on_doubleSpinBox_z_rot_valueChanged(double arg1) {
//  if (ui->widget->filename != "") {
//    s21::Transform::GetInstance().RotateXYZ('z',
//                                            ui->doubleSpinBox_z_rot->value());
//    ui->widget->update();
//  }
}

void viewer::on_actionOrthographic_Perspective_triggered() {
  QKeyEvent *key = new QKeyEvent(QEvent::KeyPress,
                                 ui->widget->projection_type ? Qt::Key_O : Qt::Key_P,
                                 Qt::NoModifier);
  ui->widget->keyPressEvent(key);
  delete key;
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

  SetFrameColor();
}

void viewer::on_pushButton_wireframe_clicked() {
    if (!ui->widget->wireframe) ui->widget->wireframe = true;
    ui->pushButton_apply_texture->setDisabled(true);
    on_pushButton_unload_texture_clicked();
    ui->widget->update();
}

void viewer::on_pushButton_flat_shading_clicked() {
    if (ui->widget->wireframe) ui->widget->wireframe = false;
    if (ui->widget->has_texture) ui->pushButton_apply_texture->setDisabled(false);
    ui->widget->flat_shading = true;
    ui->widget->update();
}


void viewer::on_pushButton_smooth_shading_clicked() {
    if (ui->widget->wireframe) ui->widget->wireframe = false;
    if (ui->widget->has_texture) ui->pushButton_apply_texture->setDisabled(false);
    ui->widget->flat_shading = false;
    ui->widget->update();
}

void viewer::on_pushButton_apply_texture_clicked() {
    ui->widget->wireframe = false;
    QString fname = QFileDialog::getOpenFileName(
        this, "Choose File", QDir::homePath(), tr("BMP (*.bmp)"));

    if (fname != "") {
        if (!ui->widget->texture)
            delete ui->widget->texture;
        ui->widget->texture = new QOpenGLTexture(QImage(fname));
        ui->widget->texture->setMinificationFilter(QOpenGLTexture::Nearest);
        ui->widget->texture->setMagnificationFilter(QOpenGLTexture::Linear);
        ui->widget->texture->setWrapMode(QOpenGLTexture::Repeat);
        ui->pushButton_unload_texture->setDisabled(false);
        ui->pushButton_save_uvmap->setDisabled(false);
    }
    ui->widget->update();
}

void viewer::on_pushButton_unload_texture_clicked() {
    if (ui->widget->texture)
        delete ui->widget->texture;
    ui->widget->texture = nullptr;
    ui->pushButton_unload_texture->setDisabled(true);
    ui->pushButton_save_uvmap->setDisabled(true);
    ui->widget->update();
}

void viewer::on_doubleSpinBox_x_light_pos_valueChanged(double arg1) {
    ui->widget->light_pos[0] = arg1;
    ui->widget->update();
}


void viewer::on_doubleSpinBox_y_light_pos_valueChanged(double arg1) {
    ui->widget->light_pos[1] = arg1;
    ui->widget->update();
}


void viewer::on_doubleSpinBox_z_light_pos_valueChanged(double arg1) {
    ui->widget->light_pos[2] = arg1;
    ui->widget->update();
}

static const QVector<GLfloat> text = {
//    0.000245, 0.500000,
//    0.333089, 0.500000,
//    0.333089, 0.999266,

//    0.000245, 0.500000,
//    0.333089, 0.999266,
//    0.000245, 0.999266,


//    0.666911, 0.499511,
//    0.666911, 0.000245,
//    0.999755, 0.000245,

//    0.666911, 0.499511,
//    0.999755, 0.000245,
//    0.999756, 0.499511,


//    0.666422, 0.500000,
//    0.666422, 0.999266,
//    0.333578, 0.999266,

//    0.666422, 0.500000,
//    0.333578, 0.999266,
//    0.333578, 0.500000,


//    0.000245, 0.000245,
//    0.333089, 0.000245,
//    0.333089, 0.499511,

//    0.000245, 0.000245,
//    0.333089, 0.499511,
//    0.000245, 0.499511,


//    0.666911, 0.999266,
//    0.666911, 0.500000,
//    0.999755, 0.500000,

//    0.666911, 0.999266,
//    0.999755, 0.500000,
//    0.999756, 0.999266,


//    0.666422, 0.000245,
//    0.666422, 0.499511,
//    0.333578, 0.499511,

//    0.666422, 0.000245,
//    0.333578, 0.499511,
//    0.333578, 0.000245

    0.000245, 0.500000,
    0.333089, 0.500000,
    0.333089, 0.999266,
    0.000245, 0.500000,
    0.333089, 0.999266,
    0.000245, 0.999266,
    0.000245, 0.500000,

    0.666911, 0.499511,
    0.666911, 0.000245,
    0.999755, 0.000245,
    0.666911, 0.499511,
    0.999755, 0.000245,
    0.999756, 0.499511,
    0.666911, 0.499511,

    0.666422, 0.500000,
    0.666422, 0.999266,
    0.333578, 0.999266,
    0.666422, 0.500000,
    0.333578, 0.999266,
    0.333578, 0.500000,
    0.666422, 0.500000,

    0.000245, 0.000245,
    0.333089, 0.000245,
    0.333089, 0.499511,
    0.000245, 0.000245,
    0.333089, 0.499511,
    0.000245, 0.499511,
    0.000245, 0.000245,

    0.666911, 0.999266,
    0.666911, 0.500000,
    0.999755, 0.500000,
    0.666911, 0.999266,
    0.999755, 0.500000,
    0.999756, 0.999266,
    0.666911, 0.999266,

    0.666422, 0.000245,
    0.666422, 0.499511,
    0.333578, 0.499511,
    0.666422, 0.000245,
    0.333578, 0.499511,
    0.333578, 0.000245,
    0.666422, 0.000245
    //

};

void viewer::on_pushButton_save_uvmap_clicked()
{
    QString fname = QFileDialog::getOpenFileName(
        this, "Choose File", QDir::homePath(), tr("BMP (*.bmp)"));

    QPixmap map(fname);
    QPainter painter(&map);

    QList<QLine> parser_x_y;
    int count = 0;
//    for (int i = 0; text.size() > i; i+=2, ++count) {
//        if (i == 0) {
//            parser_x_y.push_back(QLine(i* map.width(), i * map.height(), text[i]* map.width(), text[i+1]* map.height()));
//        }
//        parser_x_y.push_back(QLine(parser_x_y[count].x2()* map.width(), parser_x_y[count].y2()* map.height(), text[i]* map.width(), text[i+1]* map.height()));

//    }

    //int count = 0; более менее финальная версия
    for (int i = 0; text.size()-2 > i; i+=2, ++count) {
        if(count == 3) {
            std::cout << "Значение n равно: " << count;
            i += 2;
            count = 0;
        }
        parser_x_y.push_back(QLine(text[i]* map.width(), text[i+1]* map.height(), text[i+2]* map.width(), text[i+3]* map.height()));



    }

//int count = 0; более менее финальная версия
    for (int i = 0; parser_x_y.size() > i; ++i, ++count) {
        painter.drawLine(parser_x_y[i]);
    }

//    painter.drawLine(parser_x_y[0]);
//    painter.drawLine(parser_x_y[1]);
//    painter.drawLine(parser_x_y[2]);
//    painter.drawLine(parser_x_y[3]);

//    painter.drawLine(parser_x_y[4]);
//    painter.drawLine(parser_x_y[5]);
//    painter.drawLine(parser_x_y[6]);
//    painter.drawLine(parser_x_y[7]);
//    painter.drawLine(parser_x_y[8]);
//    painter.drawLine(parser_x_y[9]);


//    painter.drawLine(parser_x_y[12]);
//    painter.drawLine(parser_x_y[13]);




//    painter.drawLine(parser_x_y[18]);


//    painter.drawLine(parser_x_y[21]);

    //painter.translate(this->rect().bottomLeft());
//    painter.scale(1.0, 0);
    //painter.drawLine(QLine(0.000245 * map.width(), 0.999266 * map.height(), 0.666911 * map.width(), 0.499511 * map.height()));

    QString str = QFileDialog::getSaveFileName(
        this, tr("Save GIF"), QDir::homePath(), tr("BMP (*.bmp)"));
    map.save(str);
//    painter.save();
//    QPixmap image(fname);
//    image
}

void viewer::on_doubleSpinBox_r_light_intens_valueChanged(double arg1) {
    ui->widget->light_color[0] = arg1;
    ui->widget->update();
}

void viewer::on_doubleSpinBox_g_light_intens_valueChanged(double arg1) {
    ui->widget->light_color[1] = arg1;
    ui->widget->update();
}

void viewer::on_doubleSpinBox_b_light_intens_valueChanged(double arg1) {
    ui->widget->light_color[2] = arg1;
    ui->widget->update();
}
