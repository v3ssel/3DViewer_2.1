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

    void ParseVertex_3D(const std::string path_to_file);
    void ParseF(QStringList str);
    void CountOfFacets(const std::string path_to_file);

    QVector<GLfloat> facetsArray;
    int allElemsIntoFacetsArr;

    QVector<QVector3D> vertex_;
    QVector<QVector3D> normals_;
    QVector<QVector2D> uvs_;
private:
    parse() {}
    parse(const parse&);
    void operator=(parse&);

    uint CountOfFacets_;
};
}

#endif // PARSE_H
