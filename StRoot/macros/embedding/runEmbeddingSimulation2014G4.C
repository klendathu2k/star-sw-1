class StBFChain;
class StMessMgr;

#include <string>
#include <TString.h>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TSystem.h"

#include <iostream>

// Load sufficient libraries to bootstrap the StBFChain framework
#pragma cling load("libTree.so")
#pragma cling load("StarRoot")
#pragma cling load("St_base")
#pragma cling load("StChain")
#pragma cling load("StUtilities")
#pragma cling load("StBFChain")
#pragma cling load("liblog4cxx")
#pragma cling load("StStarLogger.so")
#pragma cling load("StarClassLibrary.so")
#pragma cling load("libmysqlclient.so")
//#pragma cling load("libStarMiniCern.so")

#if !(defined(__CINT__) || defined(__CLING__)) || defined(__MAKECINT__)

#include "Stiostream.h"
#include "TSystem.h"
#include "TClassTable.h"
#include "TApplication.h"
#include "TInterpreter.h"
#include "StBFChain.h"
#include "StMessMgr.h"
#include "TROOT.h"
#include "TAttr.h"
#include "Rtypes.h"

#endif


// Defined w/in the StBFChain library
extern StBFChain* chain;

StBFChain* top = new StBFChain("physicssim");

StMaker* g4star = 0;


TFile* _tagfile = 0; // file containing the tags
TTree* _tags    = 0; // tree containing the tags
double _vxyz[3] = { 0, 0, 0 }; // event vertex
double _sxyz[3] = { 0, 0, 0 }; // additional smearing
int    _runnumber = 0;
int    _evtnumber = 0;
UInt_t _unprimaries = 0;
double _eventtime = 0;
double _prodtime  = 0;
double _magfield  = -5.005;
int     _npart = 5;  // floor number of tracks per event
float   _fpart = 0.05;  // fraction of track multiplicity to embed
float   _ptmn  = 5.0 - 0.0001  ;  // min pT to simulate [GeV]
float   _ptmx  = 5.0 + 0.0001  ; // max pT to simulate [GeV]
float   _etamn =  0.25 - 0.0001;   // min eta to simulate
float   _etamx =  0.25 + 0.0001;   // max eta to simulate
int     _pid   = 14; // default to proton
int     npart  = 0;
TString DBV;     // If unset, will fill from tag file  "DbV20161018";
TString SDT;
std::string _dbv="DbV20230818";
std::string _geom="y2021a";
// tagfile, geantfile, ne
std::string _tagfileName = "/gpfs01/star/embed/tags/2021/auau17_phys/st_physics_adc_22158015_raw_5000016.tags.root";
std::string _geantfileName = "geant4out.geant.root";
int         _nevents = 2;
//______________________________________________________________________________________
void process( const char* line ){
  gMessMgr->Info(line);
  gInterpreter->ProcessLine( Form("%s", line) );
};
void runEmbeddingSimulation2014G4()
{

  //________________________________________________________
  //
  // Open tagfile from where we will obtain the event vertex
  //
  // We do not have access to the embedding maker for now do this the hacky way and set the vertex directly in the primary maker
  _tagfile = TFile::Open(_tagfileName.c_str());
  _tags    = (TTree*) _tagfile -> Get("Tag");
  //
  _tags->SetBranchAddress( "EvtHddr.mRunNumber",       &_runnumber );
  _tags->SetBranchAddress( "EvtHddr.mEventNumber",     &_evtnumber );
  _tags->SetBranchAddress( "GlobalTag.primaryVertexX", &_vxyz[0] );
  _tags->SetBranchAddress( "GlobalTag.primaryVertexY", &_vxyz[1] );
  _tags->SetBranchAddress( "GlobalTag.primaryVertexZ", &_vxyz[2] );
  _tags->SetBranchAddress( "GlobalTag.sigmaPVX", &_sxyz[0] );
  _tags->SetBranchAddress( "GlobalTag.sigmaPVY", &_sxyz[1] );
  _tags->SetBranchAddress( "GlobalTag.sigmaPVZ", &_sxyz[2] );

  _tags->SetBranchAddress( "GlobalTag.uncorrectedNumberOfPrimaries", &_unprimaries );

  _tags->SetBranchAddress( "EvtHddr.mEventTime",       &_eventtime );
  _tags->SetBranchAddress( "EvtHddr.mProdTime",        &_prodtime );
  // _tags->SetBranchAddress( "magField",        &_magfield );


  _tags->GetEntry(0); // read in first event

  SDT = Form("sdt%i",int(_eventtime));
  if ( DBV == "" )  DBV = Form("dbv%i",int(_prodtime));

  // Determine maximum number of events to process
  if ( _nevents < 0 ) _nevents = _tags->GetEntries();

  std::string chainopts =  "nodefault " + _geom + " " + _dbv + " agml simu sim_T gen_T stargen:mk kinematics:mk g4star:mk noinput geant4out ";
  // chainopts += SDT; chainopts += " ";
  // chainopts += DBV; chainopts += " ";
  top->SetDebug(1);
  top->SetFlags(chainopts.c_str());
  top->Set_IO_Files(0, _geantfileName.c_str());
  top->Load();
  top->Instantiate();

  process( "auto* primary_ = dynamic_cast<StarPrimaryMaker*>( StMaker::GetTopChain()->Maker(\"PrimaryMaker\") );" );
  process( "auto* kine_    = dynamic_cast<StarKinematics*>( StMaker::GetTopChain()->Maker(\"StarKine\") );" );
  process( "kine_->SetAttr(\"rapidity\",1);" );
  process( "primary_ -> AddGenerator( kine_ );");



  g4star = top->Maker("geant4star");
  g4star->SetAttr("G4VmcOpt","FTFP_BERT");

  g4star->SetAttr(
      "G4UI:PREINIT",
      "/process/eLoss/maxKinEnergy 250.0 GeV"   ";"
      "/mcCrossSection/setMaxKinE  250.0 GeV"   ";"
  );
  g4star->SetAttr(
      "G4UI:INIT",
      "/mcCrossSection/setMinKinE 1 keV "       ";"
      "/mcCrossSection/setMaxKinE 250 GeV "     ";"
      "/mcCrossSection/setMinMomentum 10 keV "  ";"
      "/mcCrossSection/setMaxMomentum 250 GeV " ";"
  );

  g4star->SetAttr( "runnumber",_runnumber );

  g4star->SetAttr( "g4:initAtInitRun" , 1 ); // Defer geometry and VMC initialization until InitRun

  top->ls(5);

  gInterpreter->ProcessLine("{Geometry* __hack = new Geometry(); delete __hack;}");
  
  top->Init();

  gSystem->SetFPEMask( kNoneMask );
  // top->EventLoop(_nevents, top->Maker("outputStream"));
  for( int i=0; i<_nevents; i++ ) {
    top->Clear();    
    _tags->GetEntry(i);

    process(Form("primary_->SetVertex( %f, %f, %f );", _vxyz[0], _vxyz[1], _vxyz[2]));
    process(Form("primary_->SetSigma( %f, %f, %f );", _sxyz[0], _sxyz[1], _sxyz[2]));

    
    printf("run=%i event=%i mult=%i vxyz=%f %f %f sxyz=%f %f %f\n",     
      _runnumber, _evtnumber, _unprimaries, _vxyz[0], _vxyz[1], _vxyz[2], _sxyz[0], _sxyz[1], _sxyz[2] );

    npart = 0;
    if ( _fpart < 1.0 ) {
	    npart = int(_unprimaries * _fpart);
      if ( npart < _npart ) npart = _npart;
    } else {
      npart = int(_fpart);
    }

    top->SetDateTime( int(_eventtime), int( 100000*(_eventtime-int(_eventtime)) ) );

    g4star->SetAttr( "runnumber",_runnumber );
    g4star->SetAttr( "vertex:x", _vxyz[0] );
    g4star->SetAttr( "vertex:y", _vxyz[1] );
    g4star->SetAttr( "vertex:z", _vxyz[2] );
    g4star->SetAttr( "vertex:sigmax", _sxyz[0] );
    g4star->SetAttr( "vertex:sigmay", _sxyz[1] );
    g4star->SetAttr( "vertex:sigmaz", _sxyz[2] );

    process(" kine_->SetAttr(\"mode\",    \"FlatPT\"); ");
    process(Form(" kine_->SetAttr(\"ptlow\",   %f ); ", _ptmn) );
    process(Form(" kine_->SetAttr(\"pthigh\",  %f ); ", _ptmx) );
    process(Form(" kine_->SetAttr(\"etalow\",  %f ); ", _etamn) );
    process(Form(" kine_->SetAttr(\"etahigh\", %f ); ", _etamx) );
    process(Form(" kine_->SetAttr(\"phimn\",   %f ); ", 0.0) );
    process(Form(" kine_->SetAttr(\"phimx\",   %f ); ", TMath::TwoPi()) );
    process(Form(" kine_->SetAttr(\"pid\",     %d ); ", _pid ) );
    process(Form(" kine_->SetAttr(\"ntrack\",  %d ); ", npart ) );

    top->Make();
  }
  top->Finish();


}

//______________________________________________________________________________________


void runEmbeddingSimulation2014G4(
			    const int ne,
			    const char* geantfile,
			    const char* tagfile,
			    const float mult,
			    const int   pid,
			    const float ptmn,
			    const float ptmx,
			    const float etamn,
			    const float etamx,
          const char* dbv,
          const char* geom)
{
  _nevents = ne;
  _tagfileName = tagfile;
  _geantfileName = geantfile;
  _fpart = mult;
  _ptmn  = ptmn;
  _ptmx  = ptmx;
  _etamn = etamn;
  _etamx = etamx;
  _pid   = pid;
  _dbv   = dbv;
  _geom  = geom;
  
  runEmbeddingSimulation2014G4();

}
//______________________________________________________________________________________

void runEmbeddingSimulation2014G4(const char* dbg)
{
  std::string dbg_ = dbg;
  if ( dbg_ == "test1" ) {
   _nevents = 10;
    _tagfileName = "/gpfs01/star/embed/tags/2019/auau19_phys/st_physics_adc_20057049_raw_2000003.tags.root";
    _geantfileName = "geant4out.geant.root";
    _fpart = 0.05;
    _ptmn  = 0.;
    _ptmx  = 6.;
    _etamn = -1.;
    _etamx =  1.;
    _pid   = 45;
    _dbv   = "DbV20210827";
    _geom  = "y2019a";
    runEmbeddingSimulation2014G4();

  }
}