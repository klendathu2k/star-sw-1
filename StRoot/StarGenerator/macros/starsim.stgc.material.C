// macro to instantiate the Geant3 from within
// STAR  C++  framework and get the starsim prompt
// To use it do
//  root4star starsim.C

class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *_primary = 0;

class StarKinematics;
StarKinematics *kinematics = 0;

TF1 *ptDist  = 0;
TF1 *etaDist = 0;

// ----------------------------------------------------------------------------
void geometry( TString tag, Bool_t agml=true )
{
  TString cmd = "DETP GEOM "; cmd += tag;
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> LoadGeometry(cmd);
  //  if ( agml ) command("gexec $STAR_LIB/libxgeometry.so");
}
// ----------------------------------------------------------------------------
void command( TString cmd )
{
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> Do( cmd );
}
// ----------------------------------------------------------------------------
void trig( Int_t n=1 )
{
  for ( Int_t i=0; i<n; i++ ) {

    // Clear the chain from the previous event
    chain->Clear();

    // Generate 1 mu minus at high pT
    kinematics->Kine( 1, "mu+", 10.0, 50.0, 2.8, 3.9 );
		      
    // // Generate 4 muons flat in pT and eta 
    // kinematics->Kine(4, "mu+", 0., 5., -2.0, +2.0 );

    // // Generate 4 neutral pions according to a PT and ETA distribution
    // kinematics->Dist(4, "pi0", ptDist, etaDist );

    // Generate the event
    chain->Make();

    // Print the event
    //    _primary->event()->Print();
    //    command("gprint hits");
  }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Kinematics()
{
  
  //  gSystem->Load( "libStarGeneratorPoolPythia6_4_23.so" );
  gSystem->Load( "libKinematics.so");
  kinematics = new StarKinematics();
    
  _primary->AddGenerator(kinematics);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void starsim( Int_t nevents=50000, Int_t rngSeed=1234 )
{ 

  gROOT->ProcessLine(".L bfc.C");
  {
    TString simple = "y2023 geant gstar usexgeom agml ";
    bfc(0, simple );
  }

  gSystem->Load( "libVMC.so");

  gSystem->Load( "StarGeneratorUtil.so" );
  gSystem->Load( "StarGeneratorEvent.so" );
  gSystem->Load( "StarGeneratorBase.so" );
  gSystem->Load( "StarGeneratorStep.so" );

  gSystem->Load( "libMathMore.so"   );  
  gSystem->Load( "xgeometry.so"     );

  // Setup RNG seed and map all ROOT TRandom here
  StarRandom::seed( rngSeed );
  StarRandom::capture();

  AgUStep *step = AgUStep::Instance(); 
  {
    step->Init( "steps.root" );
    const char* volumes[] = { 
      "STGM",  //  1 STGC Mother volume -->    
      "STFM",  //  2 pentagon module full (PCB, Gas, FEEs etc.) -->
      "STMP",  //  3 pentagon module of PCB -->
      "STMG",  //  4 pentagon module of Gas (active volume) -->
      "STMH",  //  5 pentagon module of Honeycomb -->
      "STGP",  //  6 pentagon sub shapes for Gas module, pgon triangle -->
      "STGL",  //  7 pentagon sub shapes for Gas module, long box -->
      "STGS",  //  8 pentagon sub shapes for Gas module, short box -->
      "STPP",  //  9 pentagon sub shapes for PCB module, pgon triangle -->
      "STPL",  // 10 pentagon sub shapes for PCB module, long box -->
      "STPS",  // 11 pentagon sub shapes for PCB module, short box -->
      "STHP",  // 12 pentagon sub shapes for Honeycomb module, pgon triangle -->
      "STHL",  // 13 pentagon sub shapes for Honeycomb module, long box -->
      "STHS"   // 14 pentagon sub shapes for Honeycomb module, short box -->
    };

    const char* titles[] = { 
      "STGM  1 STGC Mother volume ",    
      "STFM  2 pentagon module full (PCB, Gas, FEEs etc.) ",
      "STMP  3 pentagon module of PCB ",
      "STMG  4 pentagon module of Gas (active volume) ",
      "STMH  5 pentagon module of Honeycomb ",
      "STGP  6 pentagon sub shapes for Gas module, pgon triangle ",
      "STGL  7 pentagon sub shapes for Gas module, long box ",
      "STGS  8 pentagon sub shapes for Gas module, short box ",
      "STPP  9 pentagon sub shapes for PCB module, pgon triangle ",
      "STPL 10 pentagon sub shapes for PCB module, long box ",
      "STPS 11 pentagon sub shapes for PCB module, short box ",
      "STHP 12 pentagon sub shapes for Honeycomb module, pgon triangle ",
      "STHL 13 pentagon sub shapes for Honeycomb module, long box ",
      "STHS 14 pentagon sub shapes for Honeycomb module, short box "
    };

    for ( int i=0; i < 14 ; i++ ) {
      step->AddMaterialHist( volumes[i], titles[i], 150, 2.5, 4.0, 180.0, -TMath::Pi(), TMath::Pi() );
    }

  }
  AgUStep::verbose = 0;
  AgUStep::zmin = 255.6510;
  AgUStep::zmax = 369.5846;




  //
  // Create the primary event generator and insert it
  // before the geant maker
  //
  //  StarPrimaryMaker *
  _primary = new StarPrimaryMaker();
  {
    _primary -> SetFileName( "kinematics.starsim.root");
    chain -> AddBefore( "geant", _primary );
  }

  Kinematics();

  //
  // Initialize primary event generator and all sub makers
  //
  _primary -> Init();
  _primary->SetSigma( 0.1, 0.1, 0.1 ); // 1mm x 1mm x 1mm smearing at the vertex
  _primary->SetVertex(0.0, 0.0, 0.0 );

  //
  // Setup geometry and set starsim to use agusread for input
  //
  //geometry("y2012");
  command("gkine -4 0");
  command("gfile o kinematics.starsim.fzd");

  command( "DCAY 0" );
  command( "ANNI 0" );
  command( "BREM 0" );
  command( "COMP 0" );
  command( "HADR 0" );
  command( "MUNU 0" );
  command( "PAIR 0" );
  command( "PFIS 0" );
  command( "PHOT 0" );
  command( "RAYL 0" );
  command( "LOSS 4" );
  command( "DRAY 0" );
  command( "MULS 0" );
  command( "STRA 0" );
  command( "physi"  );
  

  //
  // Setup PT and ETA distributions
  //

  Double_t pt0 = 3.0;
  ptDist = new TF1("ptDist","(x/[0])/(1+(x/[0])^2)^6",0.0,10.0);
  ptDist->SetParameter(0, pt0);
  ptDist->Draw();

  etaDist = new TF1("etaDist","-TMath::Erf(x+2.6)*TMath::Erf(x-2.6)",-0.8,+0.8);

  //
  // Trigger on nevents
  //
  trig( nevents );

  step->Finish();

  command("call agexit");  // Make sure that STARSIM exits properly

}
// ----------------------------------------------------------------------------

