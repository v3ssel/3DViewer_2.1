#include "controller.h"

namespace s21 {
std::vector<double>& Controller::GetVertex() {
  return s21::Parser::GetInstance().GetVertex();
}

std::vector<int>& Controller::GetIndices() {
  return s21::Parser::GetInstance().GetIndices();
}

void Controller::ParseVertex(std::string path_to_file) {
  return s21::Parser::GetInstance().ParseVertex(path_to_file);
}

void Controller::ParseIndices(std::string path_to_file) {
  return s21::Parser::GetInstance().ParseIndices(path_to_file);
}

void Controller::MoveXYZ(double x1, double y1, double z1) {
  return s21::Transform::GetInstance().MoveXYZ(x1, y1, z1);
}

void Controller::Scale(double c) {
  return s21::Transform::GetInstance().Scale(c);
}

void Controller::RotateXYZ(char coordinate, double angle) {
  return s21::Transform::GetInstance().RotateXYZ(coordinate, angle);
}
}  // namespace s21
