#include <iostream>
void unit_test_load_agml() {
  std::cout << "unit_test_load_agml" << std::endl;
  gROOT->LoadMacro("StRoot/macros/bfc.C");
  gROOT->ProcessLine("bfc(0,\"y2019a agml nodefault\");");
}

