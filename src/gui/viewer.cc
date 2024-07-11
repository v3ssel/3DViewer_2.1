#include "viewer.h"

#include "ui_viewer.h"

Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
    ui->setupUi(this);
    this->setWindowTitle("3DViewer 2.1");

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
    connect(record_time_, &QTimer::timeout, this, &Viewer::Recording_);
}

Viewer::~Viewer() {
    delete ui;
}

void Viewer::on_actionOpen_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Choose File", QDir::homePath(), tr("OBJ (*.obj)"));

    if (filename != "") {
        filename_ = filename;
        ui->widget->InitModel(filename);
        ui->widget->update();
    }
}

void Viewer::on_actionClose_triggered() {
    ui->widget->ResetModel();
    ui->widget->update();
    filename_ = "";
}

void Viewer::on_actionInfo_triggered() {
    // size_t vertices = !s21::Controller::Instance().GetVertices().size()
    //                     ? 0
    //                     : s21::Controller::Instance().GetVertices().size() - 1;
    size_t vertices = ui->widget->VertexCount();
    
    QMessageBox::information(
        this, "Information",
        "Filename: " + filename_ +
        "\nVertices: " + QString::number(vertices) +
        "\nLines: " + QString::number(ui->widget->IndexCount()));
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
    }

    ui->widget->keyPressEvent(event);
}

void Viewer::SetFrameColor() {
    this->setStyleSheet("QMainWindow{ background-color: rgb(" +
                        QString::number(ui->widget->background.red()) + ", " +
                        QString::number(ui->widget->background.green()) + ", " +
                        QString::number(ui->widget->background.blue()) + ");}");
}

void Viewer::on_pushButton_bg_clicked() {
    ui->widget->background = QColorDialog::getColor();
    SetFrameColor();
}

void Viewer::on_pushButton_vertex_clicked() {
    ui->widget->vertices_color = QColorDialog::getColor();
}

void Viewer::on_pushButton_lines_clicked() {
    ui->widget->lines_color = QColorDialog::getColor();
}

void Viewer::on_horizontalSlider_lineWidth_sliderMoved(int position) {
    ui->widget->line_width = position;
    ui->lcdNumber_lineWidth->display(position);
    ui->widget->update();
}

void Viewer::on_horizontalSlider_lineWidth_sliderPressed() {
    on_horizontalSlider_lineWidth_sliderMoved(
        ui->horizontalSlider_lineWidth->value());
}

void Viewer::on_horizontalSlider_versize_sliderMoved(int position) {
    ui->widget->no_vertices = false;
    ui->widget->vertex_size = position;
    ui->lcdNumber_versize->display(position);
    ui->widget->update();
}

void Viewer::on_horizontalSlider_versize_sliderPressed() {
    on_horizontalSlider_versize_sliderMoved(
        ui->horizontalSlider_versize->value());
}

void Viewer::on_pushButton_line_solid_clicked() {
    ui->widget->dashed_solid = false;
    ui->widget->update();
}

void Viewer::on_pushButton_line_dashed_clicked() {
    ui->widget->dashed_solid = true;
    ui->widget->update();
}

void Viewer::on_pushButton_ver_circle_clicked() {
    ui->widget->no_vertices = false;
    ui->widget->circle_square = false;
    ui->widget->update();
}

void Viewer::on_pushButton_ver_square_clicked() {
    ui->widget->no_vertices = false;
    ui->widget->circle_square = true;
    ui->widget->update();
}

void Viewer::on_pushButton_ver_none_clicked() {
    ui->widget->no_vertices = true;
    ui->horizontalSlider_versize->setValue(1);
    ui->lcdNumber_versize->display(1);
    ui->widget->update();
}

void Viewer::on_horizontalSlider_scale_sliderMoved(int position) {
    if (position > 0) {
        ui->widget->scale_factor = position;
    } else {
        ui->widget->scale_factor = float(1.0f - abs(position) / 100.0f);
    }

    ui->lcdNumber_scale->display(position);
    ui->widget->update();
}

void Viewer::on_horizontalSlider_scale_sliderPressed() {
    on_horizontalSlider_scale_sliderMoved(ui->horizontalSlider_scale->value());
}

void Viewer::on_doubleSpinBox_x_move_valueChanged() {
    ui->widget->model_pos[0] = ui->doubleSpinBox_x_move->value();
    ui->widget->model_pos[1] = ui->doubleSpinBox_y_move->value();
    ui->widget->model_pos[2] = ui->doubleSpinBox_z_move->value();
    ui->widget->update();
}

void Viewer::on_doubleSpinBox_y_move_valueChanged() {
    on_doubleSpinBox_x_move_valueChanged();
}

void Viewer::on_doubleSpinBox_z_move_valueChanged() {
    on_doubleSpinBox_x_move_valueChanged();
}

void Viewer::on_spinBox_x_rot_valueChanged(int arg1) {
    ui->widget->RotateModel(arg1, ui->widget->prev_rotation.y(), ui->widget->prev_rotation.z());
    ui->widget->update();
}

void Viewer::on_spinBox_y_rot_valueChanged(int arg1) {
    ui->widget->RotateModel(ui->widget->prev_rotation.x(), arg1, ui->widget->prev_rotation.z());
    ui->widget->update();
}

void Viewer::on_spinBox_z_rot_valueChanged(int arg1) {
    ui->widget->RotateModel(ui->widget->prev_rotation.x(), ui->widget->prev_rotation.y(), arg1);
    ui->widget->update();
}

void Viewer::on_actionOrthographic_Perspective_triggered() {
    QKeyEvent *key = new QKeyEvent(
        QEvent::KeyPress, ui->widget->projection_type ? Qt::Key_O : Qt::Key_P,
        Qt::NoModifier);
    ui->widget->keyPressEvent(key);
    delete key;
}

void Viewer::on_actionHide_triggered() {
    if (hiden_) {
        ui->dockWidget_main->show();
        ui->dockWidget_3->show();
        hiden_ = false;
    } else {
        ui->dockWidget_main->hide();
        ui->dockWidget_3->hide();
        hiden_ = true;
    }
}

void Viewer::SaveImage_(QString format) {
    QString str = QFileDialog::getSaveFileName(this, "Save file as",
                                               QDir::homePath(), format);
    if (str != "") ui->widget->grabFramebuffer().save(str);
}

void Viewer::on_actionJPEG_triggered() { SaveImage_("*.jpeg"); }

void Viewer::on_actionBMP_triggered() { SaveImage_("*.bmp"); }

void Viewer::on_actionGIF_triggered() {
    if (!is_recording_) {
        is_recording_ = true;
        this->setStyleSheet("QMainWindow{ background-color: red;}");
        record_time_->start(100);
    }
}

void Viewer::Recording_() {
    if (is_recording_ && time_ <= 5.0) {
        GIF_.push_back(ui->widget->grab().toImage());
        time_ += 0.1;
    } else {
        SaveGIF_();
        record_time_->stop();
    }
}

void Viewer::SaveGIF_() {
    QString str = QFileDialog::getSaveFileName(
        this, tr("Save GIF"), QDir::homePath(), tr("GIF (*.gif)"));
    if (str != "") {
        QGifImage gif(QSize(640, 480));

        gif.setDefaultTransparentColor(Qt::black);
        gif.setDefaultDelay(100);

        for (QVector<QImage>::Iterator frame = GIF_.begin();
             frame != GIF_.end(); frame++) {
            gif.addFrame(*frame);
        }

        gif.save(str);
        GIF_.clear();
    }
    time_ = 0.0;
    is_recording_ = false;

    SetFrameColor();
}
