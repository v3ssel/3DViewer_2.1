#ifndef SRC_CPART_PARSER_H
#define SRC_CPART_PARSER_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//  Singleton
namespace s21 {
class Parser {
 public:
  static Parser& GetInstance() {
    static Parser instance;
    return instance;
  }

  void ParseVertex(const std::string path_to_file);
  void ParseIndices(const std::string path_to_file);

  std::vector<double>& GetVertex();
  std::vector<int>& GetIndices();

 private:
  Parser() {}
  Parser(const Parser&);
  void operator=(Parser&);

  std::vector<double> vertex_;
  std::vector<int> indices_;
};

class Transform {
 public:
  static Transform& GetInstance() {
    static Transform instance;
    return instance;
  }

  void MoveXYZ(double x1, double y1, double z1);
  void Scale(double c);
  void RotateXYZ(char coordinate, double angle);

 private:
  Transform() {}
  Transform(const Transform&);
  void operator=(const Transform&);
};
}  // namespace s21

#endif
