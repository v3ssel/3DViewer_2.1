#include <vector>

#include "../Controller/controller.h"
#include "gtest/gtest.h"

TEST(Test, test_vertex) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  double vertex[27] = {0,         0,         0,         1.000000,  1.000000,
                       -1.000000, 1.000000,  -1.000000, -1.000000, 1.000000,
                       1.000000,  1.000000,  1.000000,  -1.000000, 1.000000,
                       -1.000000, 1.000000,  -1.000000, -1.000000, -1.000000,
                       -1.000000, -1.000000, 1.000000,  1.000000,  -1.000000,
                       -1.000000, 1.000000};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

TEST(Test, test_indices) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseIndices(file_path);
  int indices_1[] = {1, 5, 5, 7, 7, 3, 3, 1, 4, 3, 3, 7, 7, 8, 8, 4,
                     8, 7, 7, 5, 5, 6, 6, 8, 6, 2, 2, 4, 4, 8, 8, 6,
                     2, 1, 1, 3, 3, 4, 4, 2, 6, 5, 5, 1, 1, 2, 2, 6};
  for (size_t i = 0; i < 48; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetIndices()[i],
                     indices_1[i]);
  }
}

TEST(Test, test_3) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  s21::Controller::GetInstance().MoveXYZ(1, 1, 1);
  double vertex[27] = {
      1,        1,        1,        2.000000, 2.000000, 0.000000, 2.000000,
      0.000000, 0.000000, 2.000000, 2.000000, 2.000000, 2.000000, 0.000000,
      2.000000, 0.000000, 2.000000, 0.000000, 0.000000, 0.000000, 0.000000,
      0.000000, 2.000000, 2.000000, 0.000000, 0.000000, 2.000000};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

TEST(Test, test_4) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  s21::Controller::GetInstance().Scale(1.5);
  double vertex[27] = {
      1.5,      1.5,      1.5,      3.000000, 3.000000, 0.000000, 3.000000,
      0.000000, 0.000000, 3.000000, 3.000000, 3.000000, 3.000000, 0.000000,
      3.000000, 0.000000, 3.000000, 0.000000, 0.000000, 0.000000, 0.000000,
      0.000000, 3.000000, 3.000000, 0.000000, 0.000000, 3.000000};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

TEST(Test, test_5) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  s21::Controller::GetInstance().RotateXYZ('y', 15);
  double vertex[27] = {1.5,
                       1.5,
                       1.5,
                       -2.2790637385764638,
                       3,
                       1.9508635204713509,
                       -2.2790637385764638,
                       0,
                       1.9508635204713509,
                       -4.2299272590478143,
                       3,
                       -0.32820021810511291,
                       -4.2299272590478143,
                       0,
                       -0.32820021810511291,
                       -0,
                       3,
                       0,
                       -0,
                       0,
                       0,
                       -1.9508635204713509,
                       3,
                       -2.2790637385764638,
                       -1.9508635204713509,
                       0,
                       -2.2790637385764638};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

TEST(Test, test_6) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  s21::Controller::GetInstance().RotateXYZ('x', 15);
  double vertex[27] = {1.5,
                       1.5,
                       1.5,
                       -2.2790637385764638,
                       -3.5476865637450881,
                       0.46881608433205813,
                       -2.2790637385764638,
                       -1.2686228251686242,
                       -1.4820474361392928,
                       -4.2299272590478143,
                       -2.0656391276057953,
                       2.2001932591634339,
                       -4.2299272590478143,
                       0.21342461097066859,
                       0.24932973869208314,
                       -0,
                       -2.2790637385764638,
                       1.9508635204713509,
                       -0,
                       -0,
                       0,
                       -1.9508635204713509,
                       -0.79701630243717103,
                       3.6822406953027267,
                       -1.9508635204713509,
                       1.4820474361392928,
                       1.731377174831376};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

TEST(Test, test_7) {
  std::string file_path = "objs/CUBE.obj";
  s21::Controller::GetInstance().ParseVertex(file_path);
  s21::Controller::GetInstance().RotateXYZ('z', 15);
  double vertex[27] = {1.5,
                       1.5,
                       1.5,
                       4.0383946079235935,
                       1.2130871649494965,
                       0.46881608433205813,
                       2.5563471717843003,
                       -0.51829000988187945,
                       -1.4820474361392928,
                       4.5566846178054723,
                       -1.1814291837375541,
                       2.2001932591634339,
                       3.0746371816661795,
                       -2.9128063585689303,
                       0.24932973869208314,
                       1.4820474361392928,
                       1.731377174831376,
                       1.9508635204713509,
                       0,
                       0,
                       0,
                       2.000337446021172,
                       -0.66313917385567478,
                       3.6822406953027267,
                       0.51829000988187945,
                       -2.3945163486870507,
                       1.731377174831376};
  for (size_t i = 0; i < 27; i++) {
    ASSERT_DOUBLE_EQ(s21::Controller::GetInstance().GetVertex()[i], vertex[i]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
