#include "TSystem.h"
#include "TString.h"
#include "TObjString.h"
#include "TObjArray.h"
#include <iostream>
#include <vector>

#include "EmbeddingChainOptions.h"

void runHFTWrapper(
    const int   nevents_ , 
    const char* daqfile_   = "/gpfs01/star/embed/daq/2021/auau17_phys_chop/st_physics_adc_22155034_raw_5500004.daq", 
    const char* tagfile_   = "/gpfs01/star/embed/tags/2021/auau17_phys/st_physics_adc_22155034_raw_5500004.tags.root" , 
    double ptmn_           =  5.0 - 0.0001 , 
    double ptmx_           =  5.0 + 0.0001 ,
    double etamn_          =  0.25 - 0.0001, 
    double etamx_          =  0.25 + 0.0001, 
    double vzmn_           = -145.0        , 
    double vzmx_           = 145.0         , 
    double vr_             = 2.0           , 
    int pid_               = 14            ,
    double mult_           = 0.1           , 
    std::vector<int> triggers_  = {870010} , 
    const char* prodname_  = "P23idAuAu17" , 
    const char* kintype_   = "FlatPT"      ,
    bool simIn_            = false         ,
    const char* macroDir_  = "StRoot/macros/embedding"  
) {

    TString dir(macroDir_);
    if (!dir.EndsWith("/") && dir.Length() > 0) {
        dir += "/";
    }
    TString simMacro = dir + "runEmbeddingSimulation2014G4.C";
    TString embMacro = dir + "bfcMixer_HftG4.C";

    EmbeddingChains<geant4star> getChainOptions;
    auto opts = getChainOptions(prodname_, false);
    
    if (!opts.isValid) {
        std::cerr << "Production " << prodname_ << " is not valid in EmbeddingChainOptions.h" << std::endl;
        return;
    }

    TString dbv = "";
    TString geom = "";

    TString allOpts = opts.loadopts.c_str();
    TObjArray* tokens = allOpts.Tokenize(" ,");
    for(int i = 0; i < tokens->GetEntries(); ++i) {
        TString tok = ((TObjString*)tokens->At(i))->GetString();
        if (tok.BeginsWith("dbv")) {
            dbv = tok;
        } else if (tok.BeginsWith("ry2") || tok.BeginsWith("y2")) {
            // remove the r, crashs in StGeant4Maker::ConstructGeometry?
            if (tok.BeginsWith("ry2")) tok.Remove(0, 1);
            geom = tok;
        }
    }
    delete tokens;

    if (dbv == "" || geom == "") {
        std::cerr << "Could not parse DbV or geometry from chain options!" << std::endl;
        return;
    }

    TString simfile_ = gSystem->BaseName(daqfile_);
    simfile_.ReplaceAll(".daq", "_HftG4.geant.root");

    TString trigStr = "{";
    for (size_t i = 0; i < triggers_.size(); ++i) {
        trigStr += Form("%d", triggers_[i]);
        if (i < triggers_.size() - 1) trigStr += ",";
    }
    trigStr += "}";

    std::cout << "\n========================================================\n";
    std::cout << " DAQ:     " << daqfile_ << "\n";
    std::cout << " TAG:     " << tagfile_ << "\n";
    std::cout << " SIM OUT: " << simfile_ << "\n";
    std::cout << " PROD:    " << prodname_ << " (DbV: " << dbv << ", Geom: " << geom << ")\n";
    std::cout << "========================================================\n";

    TString cmdSim = Form("root -l -b -q \'%s(%d, \"%s\", \"%s\", %f, %d, %f, %f, %f, %f, \"%s\", \"%s\")\'",
                          simMacro.Data(), nevents_, simfile_.Data(), tagfile_, mult_, pid_, ptmn_, ptmx_, etamn_, etamx_, dbv.Data(), geom.Data());
    
    std::cout << "\n---> Running Simulation Macro:\n" << cmdSim << "\n";

    int statusSim = gSystem->Exec(cmdSim.Data());
    
    if (statusSim != 0) {
        std::cerr << "\nSimulation macro failed. Exiting.\n";
        return;
    }

    TString cmdEmbed = Form("root4star -l -b -q \'%s(%d, \"%s\", \"%s\", \"%s\", %f, %f, %f, %s, \"%s\")\'",
                            embMacro.Data(), nevents_, daqfile_, tagfile_, simfile_.Data(), vzmn_, vzmx_, vr_, trigStr.Data(), prodname_);
    
    std::cout << "\n---> Running Embedding Macro:\n" << cmdEmbed << "\n";
    int statusEmbed = gSystem->Exec(cmdEmbed.Data());

    if (statusEmbed != 0) {
        std::cerr << "\nEmbedding macro failed. Exiting.\n";
        return;
    }

}