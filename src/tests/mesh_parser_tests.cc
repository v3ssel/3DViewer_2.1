#include <gtest/gtest.h>
#include <filesystem>

#include "../controller/controller.h"

TEST(MeshParser, Cube) {
    QString path = QString::fromStdString((std::filesystem::current_path() / "tests/OBJ/cube.obj").string());
    s21::Mesh* mesh = s21::Controller::Instance().ParseMeshFromFile(path);

    QVector<GLfloat> polygons_after_pars = {
        1,  1,  -1, 0.000245, 0.5,      0,  1,  0,
        -1, 1,  -1, 0.333089, 0.5,      0,  1,  0,
        -1, 1,  1,  0.333089, 0.999266, 0,  1,  0,
        1,  1,  -1, 0.000245, 0.5,      0,  1,  0,
        -1, 1,  1,  0.333089, 0.999266, 0,  1,  0,
        1,  1,  1,  0.000245, 0.999266, 0,  1,  0,
        1,  -1, 1,  0.666911, 0.499511, 0,  0,  1,
        1,  1,  1,  0.666911, 0.000245, 0,  0,  1,
        -1, 1,  1,  0.999755, 0.000245, 0,  0,  1,
        1,  -1, 1,  0.666911, 0.499511, 0,  0,  1,
        -1, 1,  1,  0.999755, 0.000245, 0,  0,  1,
        -1, -1, 1,  0.999756, 0.499511, 0,  0,  1,
        -1, -1, 1,  0.666422, 0.5,      -1, 0,  0,
        -1, 1,  1,  0.666422, 0.999266, -1, 0,  0,
        -1, 1,  -1, 0.333578, 0.999266, -1, 0,  0,
        -1, -1, 1,  0.666422, 0.5,      -1, 0,  0,
        -1, 1,  -1, 0.333578, 0.999266, -1, 0,  0,
        -1, -1, -1, 0.333578, 0.5,      -1, 0,  0,
        -1, -1, -1, 0.000245, 0.000245, 0,  -1, 0,
        1,  -1, -1, 0.333089, 0.000245, 0,  -1, 0,
        1,  -1, 1,  0.333089, 0.499511, 0,  -1, 0,
        -1, -1, -1, 0.000245, 0.000245, 0,  -1, 0,
        1,  -1, 1,  0.333089, 0.499511, 0,  -1, 0,
        -1, -1, 1,  0.000245, 0.499511, 0,  -1, 0,
        1,  -1, -1, 0.666911, 0.999266, 1,  0,  0,
        1,  1,  -1, 0.666911, 0.5,      1,  0,  0,
        1,  1,  1,  0.999755, 0.5,      1,  0,  0,
        1,  -1, -1, 0.666911, 0.999266, 1,  0,  0,
        1,  1,  1,  0.999755, 0.5,      1,  0,  0,
        1,  -1, 1,  0.999756, 0.999266, 1,  0,  0,
        -1, -1, -1, 0.666422, 0.000245, 0,  0,  -1,
        -1, 1,  -1, 0.666422, 0.499511, 0,  0,  -1,
        1,  1,  -1, 0.333578, 0.499511, 0,  0,  -1,
        -1, -1, -1, 0.666422, 0.000245, 0,  0,  -1,
        1,  1,  -1, 0.333578, 0.499511, 0,  0,  -1,
        1,  -1, -1, 0.333578, 0.000245, 0,  0,  -1};
    EXPECT_EQ(mesh->facets.size(), polygons_after_pars.size());
    for (auto it = 0; it < mesh->facets.size(); ++it)
        EXPECT_NEAR(mesh->facets[it], polygons_after_pars[it], 1e-4);

    QVector<GLuint> indices = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                               12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                               24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
    EXPECT_EQ(mesh->indices.size(), indices.size());
    for (auto it = 0; it < mesh->indices.size(); ++it)
        EXPECT_EQ(mesh->indices[it], indices[it]);

    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),    QVector3D(1, 1, -1), QVector3D(1, -1, -1),
        QVector3D(1, 1, 1),    QVector3D(1, -1, 1), QVector3D(-1, 1, -1),
        QVector3D(-1, -1, -1), QVector3D(-1, 1, 1), QVector3D(-1, -1, 1)};
    EXPECT_EQ(mesh->vertices.size(), vertices.size());
    for (auto it = 0; it < mesh->vertices.size(); ++it) {
        EXPECT_NEAR(mesh->vertices[it].x(),
                    vertices[it].x(), 1e-4);
        EXPECT_NEAR(mesh->vertices[it].y(),
                    vertices[it].y(), 1e-4);
        EXPECT_NEAR(mesh->vertices[it].z(),
                    vertices[it].z(), 1e-4);
    }

    QVector<QVector2D> uv_map = {QVector2D(0, 0),
                                 QVector2D(0.000245, 0.5),
                                 QVector2D(0.333089, 0.5),
                                 QVector2D(0.333089, 0.999266),
                                 QVector2D(0.000245, 0.999266),
                                 QVector2D(0.666911, 0.499511),
                                 QVector2D(0.666911, 0.000245),
                                 QVector2D(0.999755, 0.000245),
                                 QVector2D(0.999756, 0.499511),
                                 QVector2D(0.666422, 0.5),
                                 QVector2D(0.666422, 0.999266),
                                 QVector2D(0.333578, 0.999266),
                                 QVector2D(0.333578, 0.5),
                                 QVector2D(0.000245, 0.000245),
                                 QVector2D(0.333089, 0.000245),
                                 QVector2D(0.333089, 0.499511),
                                 QVector2D(0.000245, 0.499511),
                                 QVector2D(0.666911, 0.999266),
                                 QVector2D(0.666911, 0.5),
                                 QVector2D(0.999755, 0.5),
                                 QVector2D(0.999756, 0.999266),
                                 QVector2D(0.666422, 0.000245),
                                 QVector2D(0.666422, 0.499511),
                                 QVector2D(0.333578, 0.499511),
                                 QVector2D(0.333578, 0.000245)};
    EXPECT_EQ(mesh->uvs.size(), uv_map.size());
    for (auto it = 0; it < mesh->uvs.size(); ++it) {
        EXPECT_NEAR(mesh->uvs[it].x(), uv_map[it].x(), 1e-4);
        EXPECT_NEAR(mesh->uvs[it].y(), uv_map[it].y(), 1e-4);
    }

    QVector<QVector3D> normals = {QVector3D(0, 0, 0),  QVector3D(0, 1, 0),
                                  QVector3D(0, 0, 1),  QVector3D(-1, 0, 0),
                                  QVector3D(0, -1, 0), QVector3D(1, 0, 0),
                                  QVector3D(0, 0, -1)};
    EXPECT_EQ(mesh->normals.size(),
              normals.size());
    for (auto it = 0; it < mesh->normals.size();
         ++it) {
        EXPECT_NEAR(mesh->normals[it].x(),
                    normals[it].x(), 1e-4);
        EXPECT_NEAR(mesh->normals[it].y(),
                    normals[it].y(), 1e-4);
        EXPECT_NEAR(mesh->normals[it].z(),
                    normals[it].z(), 1e-4);
    }

    EXPECT_EQ(mesh->normals.isEmpty(), false);
    EXPECT_EQ(mesh->uvs.isEmpty(), false);

    mesh->Reset();
    EXPECT_EQ(mesh->facets.isEmpty(), true);
    EXPECT_EQ(mesh->indices.isEmpty(), true);
    EXPECT_EQ(mesh->vertices.isEmpty(), true);
    EXPECT_EQ(mesh->uvs.isEmpty(), true);
    EXPECT_EQ(mesh->normals.isEmpty(), true);

    delete mesh;
}

TEST(MeshParser, NonExists) {
    EXPECT_THROW(s21::Controller::Instance().ParseMeshFromFile("/some/nonexisting/path"), std::invalid_argument);
}
