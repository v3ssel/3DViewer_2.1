#include <QApplication>
#include <filesystem>
#include <gtest/gtest.h>

#include "../controller/controller.h"

TEST(Scene, Empty) {
    int argc = 0;
    QApplication* app = new QApplication(argc, nullptr);

    QWidget* fake_parent = new QWidget();
    QRect sizes(10, 20, 400, 600);
    s21::Scene *scene = s21::Controller::Instance().GetScene(fake_parent, sizes);

    EXPECT_EQ(scene->parent(), fake_parent);
    EXPECT_EQ(scene->x(), 10);
    EXPECT_EQ(scene->y(), 20);
    EXPECT_EQ(scene->rect().width(), 400);
    EXPECT_EQ(scene->rect().height(), 600);

    EXPECT_EQ(scene->VertexCount(), 0);
    EXPECT_EQ(scene->IndexCount(), 0);

    EXPECT_NO_THROW(scene->InitModel(nullptr));
    EXPECT_NO_THROW(scene->ResetModel());
    
    EXPECT_NO_THROW(scene->MoveModel(0, 0, 0));
    EXPECT_NO_THROW(scene->RotateModel(0, 0, 0));
    EXPECT_NO_THROW(scene->ScaleModel(0));

    
    delete scene;
    delete fake_parent;
    delete app;
}

TEST(Scene, WithModel) {
    int argc = 0;
    QApplication* app = new QApplication(argc, nullptr);

    QWidget* fake_parent = new QWidget();
    QRect sizes(10, 20, 400, 600);
    s21::Scene *scene = s21::Controller::Instance().GetScene(fake_parent, sizes);

    EXPECT_EQ(scene->parent(), fake_parent);
    EXPECT_EQ(scene->x(), 10);
    EXPECT_EQ(scene->y(), 20);
    EXPECT_EQ(scene->rect().width(), 400);
    EXPECT_EQ(scene->rect().height(), 600);

    EXPECT_EQ(scene->VertexCount(), 0);
    EXPECT_EQ(scene->IndexCount(), 0);

    QString path = QString::fromStdString((std::filesystem::current_path() / "tests/OBJ/cube.obj").string());
    s21::Mesh* mesh = s21::Controller::Instance().ParseMeshFromFile(path);

    EXPECT_NO_THROW(scene->InitModel(mesh));
    
    EXPECT_NO_THROW(scene->MoveModel(10, 10, 10));
    EXPECT_NO_THROW(scene->RotateModel(10, 10, 10));
    EXPECT_NO_THROW(scene->ScaleModel(10));

    EXPECT_NO_THROW(scene->ResetModel());
    
    delete mesh;
    delete scene;
    delete fake_parent;
    delete app;
}
