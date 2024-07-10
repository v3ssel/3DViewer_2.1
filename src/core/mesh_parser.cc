#include "mesh_parser.h"

namespace s21 {
void MeshParser::Parse(const QString& path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        throw std::invalid_argument(std::string("File  not found.").insert(4, path.toStdString())); 
    }
    
    AddDefaultVertex();
    QString current_string;

    while (!file.atEnd()) {
        current_string = file.readLine();
        current_string = current_string.simplified();
        QStringList numbers = current_string.split(" ");

        if (numbers[0] == "v")
            mesh_.vertices.push_back({numbers[1].toFloat(),
                                      numbers[2].toFloat(),
                                      numbers[3].toFloat()});
        if (numbers[0] == "vt")
            mesh_.uvs.push_back({numbers[1].toFloat(),
                                 numbers[2].toFloat()});

        if (numbers[0] == "vn")
            mesh_.normals.push_back({numbers[1].toFloat(),
                                     numbers[2].toFloat(),
                                     numbers[3].toFloat()});

        if (numbers[0] == "f") {
            numbers.pop_front();
            ParseFacets(numbers);
        }
    }

    for (int i = 0; i < mesh_.facets.size() / kFacetRowSize; i++)
        mesh_.indices.push_back(i);
}

void MeshParser::Clear() {
    mesh_.Reset();
}

void MeshParser::AddDefaultVertex() {
    mesh_.vertices.push_back({0, 0, 0});
    mesh_.uvs.push_back({0, 0});
    mesh_.normals.push_back({0, 0, 0});
}

void MeshParser::ParseFacets(const QStringList& str_list) {
    int counter = 0;

    QString first_elem = str_list.first();
    QString copy_curr = first_elem;

    for (const QString &str : str_list) {
        if (str_list.size() == 2 && std::isdigit(str[0].toLatin1())) {
            if (str == first_elem) {
                AddFacet(first_elem);
            }

            AddFacet(str);
        } else {
            if (counter < 3) {
                if (std::isdigit(str[0].toLatin1()) || str[0].toLatin1() == '-') {
                    AddFacet(str);
                }

                ++counter;
                copy_curr = str;
            } else {
                AddFacet(first_elem);
                AddFacet(copy_curr);
                AddFacet(str);
            }
        }
    }
}

void MeshParser::AddFacet(const QString &str) {
    std::vector<uint> indices = { 0, 0, 0 };
    
    uint index = 0;
    size_t last_slash = 0;
    for (size_t i = 0; i <= str.length(); i++) {
        if (i == str.length() || str[i] == '/') {
            indices[index] = str.mid(last_slash, i - last_slash).toUInt();
            last_slash = i + 1;

            index++;
            if (index > 2) break;
        }
    }

    if (indices[0] < 0) indices[0] += mesh_.vertices.size();
    mesh_.facets.emplace_back(mesh_.vertices[indices[0]].x());
    mesh_.facets.emplace_back(mesh_.vertices[indices[0]].y());
    mesh_.facets.emplace_back(mesh_.vertices[indices[0]].z());

    if (indices[1] < 0) indices[1] += mesh_.uvs.size();
    mesh_.facets.emplace_back(mesh_.uvs[indices[1]].x());
    mesh_.facets.emplace_back(mesh_.uvs[indices[1]].y());

    if (indices[2] < 0) indices[2] += mesh_.normals.size();
    mesh_.facets.emplace_back(mesh_.normals[indices[2]].x());
    mesh_.facets.emplace_back(mesh_.normals[indices[2]].y());
    mesh_.facets.emplace_back(mesh_.normals[indices[2]].z());
}

// void MeshParser::CheckFlags(QString path_to_file) {
    // QFile file(path_to_file);

    // if (file.open(QFile::ReadOnly)) {
    //     throw std::invalid_argument(std::string("File  not found.").insert(4, path_to_file.toStdString()));
    // }

    // QString str;
    // while (!file.atEnd()) {
    //     str = file.readLine();
    //     str = str.simplified();

    //     if (str.isEmpty()) {
    //         continue;
    //     }

    //     if (str.at(0) == 'v') {
    //         if (str.at(1) == 'n') {
    //             vn_used = true;
    //         } else if (str[1] == 't') {
    //             vt_used = true;
    //         }
    //     }

    //     if (str[0] == 'f') {
    //         int flag_slash = 0;
    //         for (int i = 2; str[i] != ' '; ++i) {
    //             if (str[i] == '/') flag_slash += 1;
    //         }

    //         if (str.contains("//")) {
    //             vt_used = false;
    //             break;
    //         }
            
    //         if (flag_slash == 1) {
    //             vn_used = false;
    //             break;
    //         }
    //     }
    // }
// }

}  // namespace s21
