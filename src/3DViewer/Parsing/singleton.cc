#include "singleton.h"

namespace s21 {
void parse::CountOfFacets(const std::string path_to_file) {
    QFile file(QString::fromStdString(path_to_file));
    if (file.open(QFile::ReadOnly)) {
        QString str;
        while (!file.atEnd()) {
          str = file.readLine();
          if (str[0] == 'f') CountOfFacets_++;
        }
    }
}

void parse::ParseF(QStringList str_list) {
    for(const auto &i : str_list) {
        const char *tmp = i.toStdString().c_str();
        if(std::isdigit(*tmp)) {
            int vertIndex = std::stoi(tmp);
            facetsArray.emplace_back(vertex_[vertIndex].x());
            facetsArray.emplace_back(vertex_[vertIndex].y());
            facetsArray.emplace_back(vertex_[vertIndex].z());
            allElemsIntoFacetsArr += 3;
            while(*tmp != '/') {
                ++tmp;
            }
            tmp++;

            int textureIndex = std::atoi(tmp) -1;
            facetsArray.emplace_back(uvs_[textureIndex].x());
            facetsArray.emplace_back(uvs_[textureIndex].y());
            allElemsIntoFacetsArr += 2;
            while(*tmp != '/') {
                ++tmp;
            }
            tmp++;

            int normalIndex = std::atoi(tmp) -1;
            facetsArray.emplace_back(normals_[normalIndex].x());
            facetsArray.emplace_back(normals_[normalIndex].y());
            facetsArray.emplace_back(normals_[normalIndex].z());
            allElemsIntoFacetsArr += 3;
        }
    }
}


void parse::ParseVertex_3D(const std::string path_to_file) {
  CountOfFacets(path_to_file);
  QFile file(QString::fromStdString(path_to_file));
  if (file.open(QFile::ReadOnly)) {
    if (vertex_.empty())
          vertex_.push_back({0,0,0}); // псевдострока
    QString current_string;
    while (!file.atEnd()) {
      current_string = file.readLine();
      current_string = current_string.simplified();
      QStringList numbers = current_string.split(" ");
      if (numbers[0] == "v") vertex_.push_back(QVector3D(numbers[1].toFloat(), numbers[2].toFloat(), numbers[3].toFloat()));
      if (numbers[0] == "vt") uvs_.push_back(QVector2D(numbers[1].toFloat(), numbers[2].toFloat()));
      if (numbers[0] == "vn") normals_.push_back(QVector3D(numbers[1].toFloat(), numbers[2].toFloat(), numbers[3].toFloat()));
      if (numbers[0] == "f") ParseF(numbers);
    }
  }
}
}
