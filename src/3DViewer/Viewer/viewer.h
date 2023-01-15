#ifndef VIEWER_H
#define VIEWER_H

#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QVector>
#include <iostream>

#include "../Controller/controller.h"
#include "../GIFCreation/gifImage/qgifimage.h"

#include <QOpenGLTexture>

QT_BEGIN_NAMESPACE
namespace Ui {
class viewer;
}
QT_END_NAMESPACE

//  Observer
class viewer : public QMainWindow {
  Q_OBJECT

 public:
  viewer(QWidget *parent = nullptr);
  ~viewer();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_actionInfo_triggered();
  void on_actionOrthographic_Perspective_triggered();
  void on_actionHide_triggered();

  void on_pushButton_bg_clicked();
  void on_pushButton_vertex_clicked();
  void on_pushButton_lines_clicked();

  void on_horizontalSlider_lineWidth_sliderMoved(int position);
  void on_horizontalSlider_lineWidth_sliderPressed();
  void on_horizontalSlider_versize_sliderMoved(int position);
  void on_horizontalSlider_versize_sliderPressed();
  void on_horizontalSlider_scale_sliderMoved(int position);
  void on_horizontalSlider_scale_sliderPressed();

  void on_pushButton_line_solid_clicked();
  void on_pushButton_line_dashed_clicked();
  void on_pushButton_ver_circle_clicked();
  void on_pushButton_ver_square_clicked();
  void on_pushButton_ver_none_clicked();
  void on_pushButton_wireframe_clicked();

  void on_doubleSpinBox_x_move_valueChanged();
  void on_doubleSpinBox_y_move_valueChanged();
  void on_doubleSpinBox_z_move_valueChanged();

  void on_doubleSpinBox_x_rot_valueChanged(double);
  void on_doubleSpinBox_y_rot_valueChanged(double);
  void on_doubleSpinBox_z_rot_valueChanged(double);
  void on_doubleSpinBox_valueChanged(double arg1);

  void on_actionJPEG_triggered();
  void on_actionBMP_triggered();

  void on_actionGIF_triggered();


  void on_pushButton_apply_texture_clicked();

  void on_pushButton_unload_texture_clicked();

private:
  void SaveImage_(QString format);
  void Recording_();
  void SaveGIF_();

  float time_;
  bool hiden_, is_recording_;
  QTimer *record_time_;
  QVector<QImage> GIF_;

  Ui::viewer *ui;
};
#endif  // VIEWER_H
