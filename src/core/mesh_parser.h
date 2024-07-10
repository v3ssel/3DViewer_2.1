#ifndef _MESH_PARSER_H_
#define _MESH_PARSER_H_

#include <QFile>
#include <QVector3D>
#include <QVector>
#include <QWidget>

#include "mesh.h"

namespace s21 {
class MeshParser {
   public:
    static MeshParser& GetInstance() {
        static MeshParser instance;
        return instance;
    }

    void Parse(const QString& path);
    void Clear();

    QVector<GLfloat>& getFacetsArr() { return mesh_.facets; }
    QVector<GLuint>& getIndicesArr() { return mesh_.indices; }

    QVector<QVector3D>& getVertexArr() { return mesh_.vertices; }
    QVector<QVector3D>& getNormalsArr() { return mesh_.normals; }
    QVector<QVector2D>& getUVsArr() { return mesh_.uvs; }

    static constexpr int kFacetRowSize = 8;

   private:
    MeshParser() { Clear(); }
    MeshParser(const MeshParser&);
    MeshParser& operator=(MeshParser&) = delete;

    void AddDefaultVertex();
    void ParseFacets(const QStringList& str_list);
    void AddFacet(const QString& str);

    Mesh mesh_;
};
}  // namespace s21

#endif  // _MESH_PARSER_H_
