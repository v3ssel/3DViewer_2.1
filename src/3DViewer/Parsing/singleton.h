#ifndef PARSE_H
#define PARSE_H

#include <QFile>

#include <QWidget>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

#include <QVector>

namespace s21 {
class parse {
public:
    static parse& GetInstance() {
        static parse instance;
        return instance;
    }
    void CheckFlags(const std::string path_to_file);

    void ParseVertex_3D(const std::string path_to_file);
    void ParseF(QStringList str);

    void pushArr(const char **tmp);

    QVector<GLfloat> facetsArray;
    QVector<QVector3D> & getVertexArr() { return vertex_; }
    QVector<QVector3D> & getNormalsArr() { return normals_; }
    QVector<QVector2D> & getUVsArr() { return uvs_; }

    int allElemsIntoFacetsArr;

    bool vn_used = false;
    bool vt_used = false;

private:
    parse() {}
    parse(const parse&);
    void operator=(parse&);

    QVector<QVector3D> vertex_;
    QVector<QVector3D> normals_;
    QVector<QVector2D> uvs_;
};
}

#endif // PARSE_H
