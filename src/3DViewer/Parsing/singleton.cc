#include "singleton.h"


namespace s21 {
std::vector<double>& Parser::GetVertex() { return this->vertex_; }
std::vector<int>& Parser::GetIndices() { return this->indices_; }
std::vector<double>& Parser::GetNormals() { return this->normals_; }
std::vector<double>& Parser::GetUvs() { return this->uvs_; }

void Parser::ParseVertex(const std::string path_to_file) {
  std::ifstream file(path_to_file);
  if (file.is_open()) {
    if (vertex_.empty())
      for (size_t i = 0; i < 3; ++i) vertex_.push_back(0);
    size_t len = 0;
    std::string current_string;
    while (std::getline(file, current_string)) {
      for (size_t d = 0; d < current_string.length(); d++) {
        if (current_string[0] == 'v') {
          if (current_string[d] == ' ' &&
              ((current_string[d + 1] >= '0' && current_string[d + 1] <= '9') ||
               current_string[d + 1] == '-')) {
            std::string number;
            size_t l = 0;
            d++;
            while (current_string[d] != ' ' && current_string[d] != '\0') {
              number[l] = current_string[d];
              l++, d++;
            }
            d--;
            if (current_string[1] == ' ') {
                vertex_.push_back(std::stod(number));
            } else if (current_string[1] == 'n') {
                normals_.push_back(std::stod(number));
            } else if (current_string[1] == 't') {
                uvs_.push_back(std::stod(number));
            }
          }
        }  else { break; }
      }
    }
    file.close();
  } else {
    throw std::invalid_argument("Error! File Not Found!");
  }
}

void Parser::ParseIndices(const std::string path_to_file) {
  std::ifstream file(path_to_file);
  if (file.is_open()) {
    int last_elem = 0;
    std::string current_string;
    while (std::getline(file, current_string)) {
      if (current_string[0] == 'f' && current_string[1] == ' ') {
        int counter = 0;
        for (size_t i = 0; i < current_string.length(); i++) {
          if (current_string[i] == ' ' &&
              (current_string[i + 1] >= '0' && current_string[i + 1] <= '9')) {
            int current_num;
            std::string num;
            i++;
            int j = 0;
            while (current_string[i] != '/' && current_string[i] != ' ' &&
                   current_string[i] != '\0') {
              num[j] = current_string[i];
              j++, i++;
            }
            current_num = std::stoi(num);
            if (counter == 0) {
              last_elem = current_num;
              indices_.push_back(current_num);
            } else {
              counter == 1 ? (indices_.push_back(current_num))
                           : (void)(indices_.back() = current_num);
              indices_.push_back(current_num);
              indices_.push_back(last_elem);
            }
            counter++;
          }
        }
      }
    }
    file.close();
  } else {
    throw std::invalid_argument("Error! File Not Found!");
  }
}

void Transform::MoveXYZ(double x1, double y1, double z1) {
  static double diff_1 = 0;
  static double diff_2 = 0;
  static double diff_3 = 0;
  for (size_t x = 0; x < Parser::GetInstance().GetVertex().size(); x += 3) {
    Parser::GetInstance().GetVertex()[x] += x1 - diff_1;
    Parser::GetInstance().GetVertex()[x + 1] += y1 - diff_2;
    Parser::GetInstance().GetVertex()[x + 2] += z1 - diff_3;
  }
  diff_1 = x1;
  diff_2 = y1;
  diff_3 = z1;
}

void Transform::Scale(double c) {
  static double prev = 1.0;
  if (c == 0.0) c = 1;
  for (size_t x = 0; x < Parser::GetInstance().GetVertex().size(); ++x) {
    Parser::GetInstance().GetVertex().at(x) =
        Parser::GetInstance().GetVertex().at(x) / prev * c;
  }
  prev = c;
}

void Transform::RotateXYZ(char coordinate, double angle) {
  int a = 0;
  int b = 0;
  double prev_angle = 0;
  static double prev_angle_x = 0;
  static double prev_angle_y = 0;
  static double prev_angle_z = 0;
  switch (coordinate) {
    case 'x':
      prev_angle = prev_angle_x;
      a = 1;
      b = 2;
      break;
    case 'y':
      prev_angle = prev_angle_y;
      a = 0;
      b = 2;
      break;
    case 'z':
      prev_angle = prev_angle_z;
      a = 0;
      b = 1;
      break;
  }
  for (size_t x = 3; x < Parser::GetInstance().GetVertex().size(); x += 3) {
    double temp_1 = Parser::GetInstance().GetVertex()[x + a];
    double temp_2 = Parser::GetInstance().GetVertex()[x + b];
    Parser::GetInstance().GetVertex()[x + a] =
        std::cos(angle - prev_angle) * temp_1 -
        std::sin(angle - prev_angle) * temp_2;
    Parser::GetInstance().GetVertex()[x + b] =
        std::sin(angle - prev_angle) * temp_1 +
        std::cos(angle - prev_angle) * temp_2;
  }
  if (coordinate == 'x') prev_angle_x = angle;
  if (coordinate == 'y') prev_angle_y = angle;
  if (coordinate == 'z') prev_angle_z = angle;
}
}  // namespace s21
