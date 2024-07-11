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
    static constexpr int kFacetRowSize = 8;

    static MeshParser& Instance() {
        static MeshParser instance;
        return instance;
    }

    Mesh* Parse(const QString& path);

   private:
    MeshParser() = default;
    MeshParser(const MeshParser&);
    MeshParser& operator=(MeshParser&) = delete;

    void AddDefaultVertex(Mesh* mesh);
    void ParseFacets(Mesh* mesh, const QStringList& str_list);
    void AddFacet(Mesh* mesh, const QString& str);
};
}  // namespace s21

#endif  // _MESH_PARSER_H_
