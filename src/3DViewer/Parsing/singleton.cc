#include "singleton.h"
#include <QDebug>

namespace s21 {

void parse::CheckFlags(const std::string path_to_file) {
    QFile file(QString::fromStdString(path_to_file));
    if (file.open(QFile::ReadOnly)) {
        QString str;
        while (!file.atEnd()) {
             str = file.readLine();
             str = str.simplified();
             if(str[0] == 'v') {
                 if(str[1] == 'n') { vn_used = true; }
                 else if (str[1] == 't') { vt_used = true; }
             }
             if(str[0] == 'f') {
                 int flag_slash = 0;
                 for(int i = 2; str[i] != ' '; ++i) {
                     if(str[i] == '/') flag_slash += 1;
                 }
                 if(str.contains("//")) { vt_used = false; }
                 if(flag_slash == 1) { vn_used = false; }
             }
         }
    }
}


void parse::pushArr(const char **curr) {
    int vertIndex = std::stoi(*curr);
//    qDebug() << "tmp into push arr" << *curr;
    facetsArray.emplace_back(vertex_[vertIndex].x());
    facetsArray.emplace_back(vertex_[vertIndex].y());
    facetsArray.emplace_back(vertex_[vertIndex].z());
    allElemsIntoFacetsArr += 3;
    while(**curr != '/') {
        ++*curr;
    }
    ++*curr;
    if(vt_used) {
        int textureIndex = std::atoi(*curr) -1;
        facetsArray.emplace_back(uvs_[textureIndex].x());
        facetsArray.emplace_back(uvs_[textureIndex].y());
        allElemsIntoFacetsArr += 2;
        while(**curr != '/') {
            ++*curr;
        }
        ++*curr;
    }
    if(vn_used) {
        int normalIndex = std::atoi(*curr) -1;
        facetsArray.emplace_back(normals_[normalIndex].x());
        facetsArray.emplace_back(normals_[normalIndex].y());
        facetsArray.emplace_back(normals_[normalIndex].z());
        allElemsIntoFacetsArr += 3;
    }
}

void parse::ParseF(QStringList str_list) {
    int counter = 0;
    const char *copy_curr;
    QString first_elem = str_list[1];
    for(const auto &i : str_list) {
        const char *curr = i.toStdString().c_str();
        if(counter < 4) {
            copy_curr = i.toStdString().c_str();
            if(std::isdigit(*curr)) {
                pushArr(&curr);
            }
            ++counter;
        } else {
            const char *first = first_elem.toStdString().c_str();
            pushArr(&curr);
            pushArr(&copy_curr);
            pushArr(&first);
        }
    }
}

void parse::ParseVertex_3D(const std::string path_to_file) {
  CheckFlags(path_to_file);
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
