#ifndef VIEWER_H
#define VIEWER_H

#include <QColorDialog>
#include <QFileDialog>
#include <QImage>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLTexture>
#include <QPainter>
#include <QTimer>
#include <QVector>

#include "../core/mesh.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Viewer;
}
QT_END_NAMESPACE

//  Observer
class Viewer : public QMainWindow {
    Q_OBJECT

   public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();

   protected:
    void keyPressEvent(QKeyEvent *event) override;

   private slots:
    void OpenFile();
    void CloseFile();
    void FileInfo();
    void ChangeProjection();

    void ChangeBackgroundColor();
    void ChangeVerticesColor();
    void ChangeLinesColor();

    void LineWidthSliderMoved(int position);
    void LineWidthSliderPressed();
    void VertexSizeSliderMoved(int position);
    void VertexSizeSliderPressed();
    void ScaleSliderMoved(int position);
    void ScaleSliderPressed();

    void MakeSolidLines();
    void MakeDashedLines();
    void MakeVertexCircle();
    void MakeVertexSquare();
    void DisableVertices();

    void MoveObjectInX();
    void MoveObjectInY();
    void MoveObjectInZ();

    void RotateObjectInX();
    void RotateObjectInY();
    void RotateObjectInZ();

    void SaveJpeg();
    void SaveBmp();
    void SaveGif();

   private:
    void SaveImage(QString format);
    void Recording();
    void SaveFullGif();

    void SetFrameColor();
    void SetupConnects();

    float time_;
    bool is_recording_;

    QVector<QImage> gif_images_;
    QString filename_;
    QTimer *record_time_;

    s21::Mesh* mesh_;

    Ui::Viewer *ui;
};

#endif  // VIEWER_H
