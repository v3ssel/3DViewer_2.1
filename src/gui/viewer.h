#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QSettings>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Viewer;
}
QT_END_NAMESPACE

//  Observer
namespace s21 {
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

    void SaveSettings();
    void LoadSettings();

    void SetFrameColor();
    void SetupConnections();

    float time_;
    bool is_recording_;

    QVector<QImage> gif_images_;
    QString filename_;
    QTimer *record_time_;
    QSettings *settings_;

    Mesh* mesh_;
    Scene* scene;

    Ui::Viewer *ui;
};
}

#endif  // VIEWER_H
