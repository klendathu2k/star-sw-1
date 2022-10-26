#include <iostream>
#include <TString.h>
void unit_test_load_y2019a() {
  std::cout << "unit_test_load_agml" << std::endl;
  gROOT->LoadMacro("StRoot/macros/bfc.C");
  gROOT->ProcessLine("bfc(0,\"y2019a agml sdt20181220 nodefault\");");
  gROOT->ProcessLine("chain->GetDataBase(\"VmcGeometry\");");
  gROOT->ProcessLine("assert(gGeoManager);");
}

