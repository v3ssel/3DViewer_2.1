#include <gtest/gtest.h>
#include "../Controller/controller.h"

TEST(LoadingModels, FullCube) {
    s21::Controller::GetInstance().ParseVertex_3D("../Unit-tests/OBJ/cube.obj");

    QVector<GLfloat> polygons_after_pars = {
        48.6566, 48.8642,  4.76648,   0.375, 0,      0, 0, 1,
        58.1896, 48.8642,  4.76648,   0.625, 0,      0, 0, 1,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,   0, 0, 1,
        48.6566, 48.8642,  4.76648,   0.375, 0,      0, 0, 1,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,   0, 0, 1,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,   0, 0, 1,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,   0, 1, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,   0, 1, 0,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,    0, 1, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,   0, 1, 0,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,    0, 1, 0,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,    0, 1, 0,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,    0, 0, -1,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,    0, 0, -1,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,   0, 0, -1,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,    0, 0, -1,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,   0, 0, -1,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,   0, 0, -1,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,   0, -1, 0,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,   0, -1, 0,
        58.1896, 48.8642,  4.76648,   0.625, 1,      0, -1, 0,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,   0, -1, 0,
        58.1896, 48.8642,  4.76648,   0.625, 1,      0, -1, 0,
        48.6566, 48.8642,  4.76648,   0.375, 1,      0, -1, 0,
        58.1896, 48.8642,  4.76648,   0.625, 0,      1, 0, 0,
        58.1896, 48.8642, -4.76648,   0.875, 0,      1, 0, 0,
        58.1896, 58.3971, -4.76648,   0.875, 0.25,   1, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 0,      1, 0, 0,
        58.1896, 58.3971, -4.76648,   0.875, 0.25,   1, 0, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,   1, 0, 0,
        48.6566, 48.8642, -4.76648,   0.125, 0,     -1, 0, 0,
        48.6566, 48.8642,  4.76648,   0.375, 0,     -1, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  -1, 0, 0,
        48.6566, 48.8642, -4.76648,   0.125, 0,     -1, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  -1, 0, 0,
        48.6566, 58.3971, -4.76648,   0.125, 0.25,  -1, 0, 0
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().size(), polygons_after_pars.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetPolygonsArray().size(); ++it)
        EXPECT_NEAR(s21::Controller::GetInstance().GetPolygonsArray()[it], polygons_after_pars[it], 1e-4);

    QVector<GLuint> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().size(), indices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetIndices().size(); ++it)
        EXPECT_EQ(s21::Controller::GetInstance().GetIndices()[it], indices[it]);

    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(48.6566, 48.8642, 4.76648),
        QVector3D(58.1896, 48.8642, 4.76648),
        QVector3D(48.6566, 58.3971, 4.76648),
        QVector3D(58.1896, 58.3971, 4.76648),
        QVector3D(48.6566, 58.3971, -4.76648),
        QVector3D(58.1896, 58.3971, -4.76648),
        QVector3D(48.6566, 48.8642, -4.76648),
        QVector3D(58.1896, 48.8642, -4.76648)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().size(), vertices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetVertices().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].x(), vertices[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].y(), vertices[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].z(), vertices[it].z(), 1e-4);
    }

    QVector<QVector2D> uv_map = {
        QVector2D(0, 0),
        QVector2D(0.375, 0),
        QVector2D(0.625, 0),
        QVector2D(0.375, 0.25),
        QVector2D(0.625, 0.25),
        QVector2D(0.375, 0.5),
        QVector2D(0.625, 0.5),
        QVector2D(0.375, 0.75),
        QVector2D(0.625, 0.75),
        QVector2D(0.375, 1),
        QVector2D(0.625, 1),
        QVector2D(0.875, 0),
        QVector2D(0.875, 0.25),
        QVector2D(0.125, 0),
        QVector2D(0.125, 0.25)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().size(), uv_map.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetUV().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetUV()[it].x(), uv_map[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetUV()[it].y(), uv_map[it].y(), 1e-4);
    }

    QVector<QVector3D> normals = {
        QVector3D(0, 0, 0),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().size(), normals.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetNormals().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].x(), normals[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].y(), normals[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].z(), normals[it].z(), 1e-4);
    }

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), true);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), true);

    s21::Controller::GetInstance().clearArrays();
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().isEmpty(), true);

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);
}

TEST(LoadingModels, CubeNoLight) {
    s21::Controller::GetInstance().ParseVertex_3D("../Unit-tests/OBJ/cube_no_light.obj");

    QVector<GLfloat> polygons_after_pars = {
        48.6566, 48.8642,  4.76648,   0.375, 0,     0, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 0,     0, 0, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,  0, 0, 0,
        48.6566, 48.8642,  4.76648,   0.375, 0,     0, 0, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,   0, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,   0, 0, 0,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,   0, 0, 0,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,   0, 0, 0,
        58.1896, 58.3971, -4.76648,   0.625, 0.5,   0, 0, 0,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0.375, 0.5,   0, 0, 0,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,  0, 0, 0,
        58.1896, 48.8642, -4.76648,   0.625, 0.75,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 1,     0, 0, 0,
        48.6566, 48.8642, -4.76648,   0.375, 0.75,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 1,     0, 0, 0,
        48.6566, 48.8642,  4.76648,   0.375, 1,     0, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 0,     0, 0, 0,
        58.1896, 48.8642, -4.76648,   0.875, 0,     0, 0, 0,
        58.1896, 58.3971, -4.76648,   0.875, 0.25,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0.625, 0,     0, 0, 0,
        58.1896, 58.3971, -4.76648,   0.875, 0.25,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0.625, 0.25,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0.125, 0,     0, 0, 0,
        48.6566, 48.8642,  4.76648,   0.375, 0,     0, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0.125, 0,     0, 0, 0,
        48.6566, 58.3971,  4.76648,   0.375, 0.25,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0.125, 0.25,  0, 0, 0
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().size(), polygons_after_pars.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetPolygonsArray().size(); ++it)
        EXPECT_NEAR(s21::Controller::GetInstance().GetPolygonsArray()[it], polygons_after_pars[it], 1e-4);

    QVector<GLuint> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().size(), indices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetIndices().size(); ++it)
        EXPECT_EQ(s21::Controller::GetInstance().GetIndices()[it], indices[it]);

    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(48.6566, 48.8642, 4.76648),
        QVector3D(58.1896, 48.8642, 4.76648),
        QVector3D(48.6566, 58.3971, 4.76648),
        QVector3D(58.1896, 58.3971, 4.76648),
        QVector3D(48.6566, 58.3971, -4.76648),
        QVector3D(58.1896, 58.3971, -4.76648),
        QVector3D(48.6566, 48.8642, -4.76648),
        QVector3D(58.1896, 48.8642, -4.76648)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().size(), vertices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetVertices().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].x(), vertices[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].y(), vertices[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].z(), vertices[it].z(), 1e-4);
    }

    QVector<QVector2D> uv_map = {
        QVector2D(0, 0),
        QVector2D(0.375, 0),
        QVector2D(0.625, 0),
        QVector2D(0.375, 0.25),
        QVector2D(0.625, 0.25),
        QVector2D(0.375, 0.5),
        QVector2D(0.625, 0.5),
        QVector2D(0.375, 0.75),
        QVector2D(0.625, 0.75),
        QVector2D(0.375, 1),
        QVector2D(0.625, 1),
        QVector2D(0.875, 0),
        QVector2D(0.875, 0.25),
        QVector2D(0.125, 0),
        QVector2D(0.125, 0.25)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().size(), uv_map.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetUV().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetUV()[it].x(), uv_map[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetUV()[it].y(), uv_map[it].y(), 1e-4);
    }


    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().size(), 1);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals()[0] == QVector3D(0, 0, 0), true);


    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), true);

    s21::Controller::GetInstance().clearArrays();
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().isEmpty(), true);

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);
}

TEST(LoadingModels, CubeNoTexture) {
    s21::Controller::GetInstance().ParseVertex_3D("../Unit-tests/OBJ/cube_no_texture.obj");

    QVector<GLfloat> polygons_after_pars = {
        48.6566, 48.8642,  4.76648,   0, 0,   0, 0, 1,
        58.1896, 48.8642,  4.76648,   0, 0,   0, 0, 1,
        58.1896, 58.3971,  4.76648,   0, 0,   0, 0, 1,
        48.6566, 48.8642,  4.76648,   0, 0,   0, 0, 1,
        58.1896, 58.3971,  4.76648,   0, 0,   0, 0, 1,
        48.6566, 58.3971,  4.76648,   0, 0,   0, 0, 1,
        48.6566, 58.3971,  4.76648,   0, 0,   0, 1, 0,
        58.1896, 58.3971,  4.76648,   0, 0,   0, 1, 0,
        58.1896, 58.3971, -4.76648,   0, 0,   0, 1, 0,
        48.6566, 58.3971,  4.76648,   0, 0,   0, 1, 0,
        58.1896, 58.3971, -4.76648,   0, 0,   0, 1, 0,
        48.6566, 58.3971, -4.76648,   0, 0,   0, 1, 0,
        48.6566, 58.3971, -4.76648,   0, 0,   0, 0, -1,
        58.1896, 58.3971, -4.76648,   0, 0,   0, 0, -1,
        58.1896, 48.8642, -4.76648,   0, 0,   0, 0, -1,
        48.6566, 58.3971, -4.76648,   0, 0,   0, 0, -1,
        58.1896, 48.8642, -4.76648,   0, 0,   0, 0, -1,
        48.6566, 48.8642, -4.76648,   0, 0,   0, 0, -1,
        48.6566, 48.8642, -4.76648,   0, 0,   0, -1, 0,
        58.1896, 48.8642, -4.76648,   0, 0,   0, -1, 0,
        58.1896, 48.8642,  4.76648,   0, 0,   0, -1, 0,
        48.6566, 48.8642, -4.76648,   0, 0,   0, -1, 0,
        58.1896, 48.8642,  4.76648,   0, 0,   0, -1, 0,
        48.6566, 48.8642,  4.76648,   0, 0,   0, -1, 0,
        58.1896, 48.8642,  4.76648,   0, 0,   1, 0, 0,
        58.1896, 48.8642, -4.76648,   0, 0,   1, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,   1, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,   1, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,   1, 0, 0,
        58.1896, 58.3971,  4.76648,   0, 0,   1, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  -1, 0, 0,
        48.6566, 48.8642,  4.76648,   0, 0,  -1, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  -1, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  -1, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  -1, 0, 0,
        48.6566, 58.3971, -4.76648,   0, 0,  -1, 0, 0
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().size(), polygons_after_pars.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetPolygonsArray().size(); ++it)
        EXPECT_NEAR(s21::Controller::GetInstance().GetPolygonsArray()[it], polygons_after_pars[it], 1e-4);

    QVector<GLuint> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().size(), indices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetIndices().size(); ++it)
        EXPECT_EQ(s21::Controller::GetInstance().GetIndices()[it], indices[it]);

    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(48.6566, 48.8642, 4.76648),
        QVector3D(58.1896, 48.8642, 4.76648),
        QVector3D(48.6566, 58.3971, 4.76648),
        QVector3D(58.1896, 58.3971, 4.76648),
        QVector3D(48.6566, 58.3971, -4.76648),
        QVector3D(58.1896, 58.3971, -4.76648),
        QVector3D(48.6566, 48.8642, -4.76648),
        QVector3D(58.1896, 48.8642, -4.76648)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().size(), vertices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetVertices().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].x(), vertices[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].y(), vertices[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].z(), vertices[it].z(), 1e-4);
    }

    EXPECT_EQ(s21::Controller::GetInstance().GetUV().size(), 1);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV()[0] == QVector2D(0, 0), true);


    QVector<QVector3D> normals = {
        QVector3D(0, 0, 0),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 0, 1),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, 0, -1),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(0, -1, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0),
        QVector3D(-1, 0, 0)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().size(), normals.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetNormals().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].x(), normals[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].y(), normals[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetNormals()[it].z(), normals[it].z(), 1e-4);
    }

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), true);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);

    s21::Controller::GetInstance().clearArrays();
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().isEmpty(), true);

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);
}

TEST(LoadingModels, CubeNoTextureAndLight) {
    s21::Controller::GetInstance().ParseVertex_3D("../Unit-tests/OBJ/cube_no_texture_and_light.obj");

    QVector<GLfloat> polygons_after_pars = {
        48.6566, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971, -4.76648,   0, 0,  0, 0, 0,
        58.1896, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 48.8642, -4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971,  4.76648,   0, 0,  0, 0, 0,
        48.6566, 58.3971, -4.76648,   0, 0,  0, 0, 0
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().size(), polygons_after_pars.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetPolygonsArray().size(); ++it)
        EXPECT_NEAR(s21::Controller::GetInstance().GetPolygonsArray()[it], polygons_after_pars[it], 1e-4);

    QVector<GLuint> indices = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().size(), indices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetIndices().size(); ++it)
        EXPECT_EQ(s21::Controller::GetInstance().GetIndices()[it], indices[it]);

    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(48.6566, 48.8642, 4.76648),
        QVector3D(58.1896, 48.8642, 4.76648),
        QVector3D(48.6566, 58.3971, 4.76648),
        QVector3D(58.1896, 58.3971, 4.76648),
        QVector3D(48.6566, 58.3971, -4.76648),
        QVector3D(58.1896, 58.3971, -4.76648),
        QVector3D(48.6566, 48.8642, -4.76648),
        QVector3D(58.1896, 48.8642, -4.76648)
    };
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().size(), vertices.size());
    for (auto it = 0; it < s21::Controller::GetInstance().GetVertices().size(); ++it) {
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].x(), vertices[it].x(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].y(), vertices[it].y(), 1e-4);
        EXPECT_NEAR(s21::Controller::GetInstance().GetVertices()[it].z(), vertices[it].z(), 1e-4);
    }

    EXPECT_EQ(s21::Controller::GetInstance().GetUV().size(), 1);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV()[0] == QVector2D(0, 0), true);

    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().size(), 1);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals()[0] == QVector3D(0, 0, 0), true);


    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);

    s21::Controller::GetInstance().clearArrays();
    EXPECT_EQ(s21::Controller::GetInstance().GetPolygonsArray().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetIndices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetVertices().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetUV().isEmpty(), true);
    EXPECT_EQ(s21::Controller::GetInstance().GetNormals().isEmpty(), true);

    EXPECT_EQ(s21::Controller::GetInstance().NormalsUsage(), false);
    EXPECT_EQ(s21::Controller::GetInstance().TextureUsage(), false);
}
