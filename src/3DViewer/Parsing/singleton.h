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
    void pushArr(const char **tmp);
    void CheckFlags(const std::string path_to_file);
    void arrIndices();
    QVector<QVector3D> & getVertexArr() { return vertex_; }
    QVector<QVector3D> & getNormalsArr() { return normals_; }
    QVector<QVector2D> & getUVsArr() { return uvs_; }
    QVector<GLfloat> & getFacetsArr() { return facetsArray; }
    GLuint & getIsize() { return isize; }

    bool vn_used = false;
    bool vt_used = false;
    QVector<GLfloat> facetsArray;
    QVector<GLuint> indices;
private:
    parse() {}
    parse(const parse&);
    void operator=(parse&);

    void add_pseudo_str();

    QVector<QVector3D> vertex_;
    QVector<QVector3D> normals_;
    QVector<QVector2D> uvs_;
    GLuint isize = 0;
};
}

#endif // PARSE_H
