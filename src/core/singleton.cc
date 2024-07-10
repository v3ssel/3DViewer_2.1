#include "singleton.h"

namespace s21 {

void Parse::CheckFlags(QString path_to_file) {
    QFile file(path_to_file);
    if (file.open(QFile::ReadOnly)) {
        QString str;
        while (!file.atEnd()) {
            str = file.readLine();
            str = str.simplified();
            if (!str.isEmpty()) {
                if (str.at(0) == 'v') {
                    if (str.at(1) == 'n') {
                        vn_used = true;
                    } else if (str[1] == 't') {
                        vt_used = true;
                    }
                }
                if (str[0] == 'f') {
                    int flag_slash = 0;
                    for (int i = 2; str[i] != ' '; ++i) {
                        if (str[i] == '/') flag_slash += 1;
                    }
                    if (str.contains("//")) {
                        vt_used = false;
                        break;
                    }
                    if (flag_slash == 1) {
                        vn_used = false;
                        break;
                    }
                }
            }
        }
    }
}

void Parse::clear() {
    facets_array_.clear();
    indices_.clear();
    vertex_.clear();
    normals_.clear();
    uvs_.clear();
    vn_used = false;
    vt_used = false;
}

void Parse::pushArr(const QString &str) {
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

    if (indices[0] < 0) indices[0] += vertex_.size();
    facets_array_.emplace_back(vertex_[indices[0]].x());
    facets_array_.emplace_back(vertex_[indices[0]].y());
    facets_array_.emplace_back(vertex_[indices[0]].z());

    if (indices[1] < 0) indices[1] += uvs_.size();
    facets_array_.emplace_back(uvs_[indices[1]].x());
    facets_array_.emplace_back(uvs_[indices[1]].y());

    if (indices[2] < 0) indices[2] += normals_.size();
    facets_array_.emplace_back(normals_[indices[2]].x());
    facets_array_.emplace_back(normals_[indices[2]].y());
    facets_array_.emplace_back(normals_[indices[2]].z());
}

void Parse::ParseF(QStringList& str_list) {
    int counter = 0;
    str_list.pop_front();

    QString first_elem = str_list.first();
    QString copy_curr = first_elem;

    for (const QString &str : str_list) {
        if (str_list.size() == 2 && std::isdigit(str[0].toLatin1())) {
            if (str == first_elem) {
                pushArr(first_elem);
            }

            pushArr(str);
        } else {
            if (counter < 3) {
                if (std::isdigit(str[0].toLatin1()) || str[0].toLatin1() == '-') {
                    pushArr(str);
                }

                ++counter;
                copy_curr = str;
            } else {
                pushArr(first_elem);
                pushArr(copy_curr);
                pushArr(str);
            }
        }
    }
}

void Parse::add_pseudo_str_() {
    if (vertex_.empty()) vertex_.push_back({0, 0, 0});
    if (uvs_.empty()) uvs_.push_back({0, 0});
    if (normals_.empty()) normals_.push_back({0, 0, 0});
}

void Parse::ParseVertex_3D(QString path_to_file) {
    CheckFlags(path_to_file);
    QFile file(path_to_file);

    if (file.open(QFile::ReadOnly)) {
        add_pseudo_str_();
        QString current_string;

        while (!file.atEnd()) {
            current_string = file.readLine();
            current_string = current_string.simplified();
            QStringList numbers = current_string.split(" ");

            // if (current_string.contains("9227/10128/9551")) {
            //     qDebug() << "heh\n";
            //     for (QString& s : numbers) {
            //         qDebug() << s;
            //     }
            // }

            if (numbers[0] == "v")
                vertex_.push_back(QVector3D(numbers[1].toFloat(),
                                            numbers[2].toFloat(),
                                            numbers[3].toFloat()));
            if (numbers[0] == "vt")
                uvs_.push_back(QVector2D(numbers[1].toFloat(),
                                         numbers[2].toFloat()));

            if (numbers[0] == "vn")
                normals_.push_back(QVector3D(numbers[1].toFloat(),
                                             numbers[2].toFloat(),
                                             numbers[3].toFloat()));

            if (numbers[0] == "f") ParseF(numbers);
        }
    }

    for (int i = 0; i < facets_array_.size() / 8; i++) indices_.push_back(i);
}
}  // namespace s21
