#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Parsing/singleton.h"

//  Facade
namespace s21 {

class Controller {
 public:
  static Controller& GetInstance() {
    static Controller controller_;
    return controller_;
  }

  std::vector<double>& GetVertex();
  std::vector<int>& GetIndices();
  std::vector<double>& GetNormals();
  std::vector<double>& GetUvs();

  void ParseVertex(const std::string path_to_file);
  void ParseNormals(const std::string path_to_file);
  void ParseIndices(const std::string path_to_file);
  void MoveXYZ(double x1, double y1, double z1);
  void Scale(double c);
  void RotateXYZ(char coordinate, double angle);

 private:
  Controller() {}
  Controller(const Controller&);
  void operator=(Controller&);
};

}  //  namespace s21

#endif
