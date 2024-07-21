#include "mesh_parser.h"

namespace s21 {
Mesh* MeshParser::Parse(const QString& path) {
  QFile file(path);
  if (!file.open(QFile::ReadOnly)) {
    throw std::invalid_argument(
        std::string("File  not found.").insert(4, path.toStdString()));
  }

  Mesh* mesh = new Mesh();
  AddDefaultVertex(mesh);
  QString current_string;

  while (!file.atEnd()) {
    current_string = file.readLine();
    current_string = current_string.simplified();
    QStringList numbers = current_string.split(" ");

    if (numbers[0] == "v")
      mesh->vertices.push_back(
          {numbers[1].toFloat(), numbers[2].toFloat(), numbers[3].toFloat()});
    if (numbers[0] == "vt")
      mesh->uvs.push_back({numbers[1].toFloat(), numbers[2].toFloat()});

    if (numbers[0] == "vn")
      mesh->normals.push_back(
          {numbers[1].toFloat(), numbers[2].toFloat(), numbers[3].toFloat()});

    if (numbers[0] == "f") {
      numbers.pop_front();
      ParseFacets(mesh, numbers);
    }
  }

  for (int i = 0; i < mesh->facets.size() / kFacetRowSize; i++)
    mesh->indices.push_back(i);

  return mesh;
}

void MeshParser::AddDefaultVertex(Mesh* mesh) {
  mesh->vertices.push_back({0, 0, 0});
  mesh->uvs.push_back({0, 0});
  mesh->normals.push_back({0, 0, 0});
}

void MeshParser::ParseFacets(Mesh* mesh, const QStringList& str_list) {
  int counter = 0;

  QString first_elem = str_list.first();
  QString copy_curr = first_elem;

  for (const QString& str : str_list) {
    if (str_list.size() == 2 && std::isdigit(str[0].toLatin1())) {
      if (str == first_elem) {
        AddFacet(mesh, first_elem);
      }

      AddFacet(mesh, str);
    } else {
      if (counter < 3) {
        if (std::isdigit(str[0].toLatin1()) || str[0].toLatin1() == '-') {
          AddFacet(mesh, str);
        }

        ++counter;
        copy_curr = str;
      } else {
        AddFacet(mesh, first_elem);
        AddFacet(mesh, copy_curr);
        AddFacet(mesh, str);
      }
    }
  }
}

void MeshParser::AddFacet(Mesh* mesh, const QString& str) {
  std::vector<int> indices = {0, 0, 0};

  uint index = 0;
  size_t last_slash = 0;
  for (qsizetype i = 0; i <= str.length(); i++) {
    if (i == str.length() || str[i] == '/') {
      indices[index] = str.mid(last_slash, i - last_slash).toUInt();
      last_slash = i + 1;

      index++;
      if (index > 2) break;
    }
  }

  if (indices[0] < 0) indices[0] += mesh->vertices.size();
  mesh->facets.emplace_back(mesh->vertices[indices[0]].x());
  mesh->facets.emplace_back(mesh->vertices[indices[0]].y());
  mesh->facets.emplace_back(mesh->vertices[indices[0]].z());

  if (indices[1] < 0) indices[1] += mesh->uvs.size();
  mesh->facets.emplace_back(mesh->uvs[indices[1]].x());
  mesh->facets.emplace_back(mesh->uvs[indices[1]].y());

  if (indices[2] < 0) indices[2] += mesh->normals.size();
  mesh->facets.emplace_back(mesh->normals[indices[2]].x());
  mesh->facets.emplace_back(mesh->normals[indices[2]].y());
  mesh->facets.emplace_back(mesh->normals[indices[2]].z());
}
}  // namespace s21
