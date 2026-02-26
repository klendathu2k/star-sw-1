# BFC Options Reference

This table is auto-generated from `StRoot/StBFChain/BigFullChain.h`.
To regenerate after editing that file run:
```
python tests/unit/StBFChain/generate_options_table.py
```

Each row corresponds to one `Bfc_st` entry.  The **Opts** column lists options that are automatically enabled (or disabled, prefixed with `-`) when this option is activated.  The **Maker** column names the C++ class instantiated; an empty Maker means the option is a pure alias or configuration token.

<details>
<summary><b>TIME STAMPS</b> (3 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `RY2008` | db,detDb,NosvtIT,NossdIT | `` | y2008 for dAu run |
| `RY2009` | db,detDb,NosvtIT,NossdIT | `` | y2009 for p+p run |
| `ForceGeometry` |  | `` | Force geometry to overwrite the geometry coming from fz-file |

</details>

<details>
<summary><b>Valid Db</b> (12 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `NoDb` | -db,-tpcDb,-magF | `` | Take out Db from Chain |
| `DbV` | db,ry1h | `` | 19940614/0 Db Version for none |
| `DbV0614` | db,ry1h | `` | 20000614/0 Db Version for p00hd |
| `DbV0624` | db,ry1h | `` | 20000624/0 Db Version for p00hd_1 |
| `DbV0713` | db,ry1h | `` | 20000713/0 Db Version for p00he |
| `DbV0727` | db,ry1h | `` | 20000727/0 Db Version for p00he |
| `DbV0819` | db,ry1h | `` | 20000819/0 Db Version for p00hg |
| `DbV1123` | db,ry1h | `` | 20001123/0 Db wo TpcDriftVel. from StTpcT0Maker |
| `DbV0523` | db,ry1h | `` | 20010523/0 Db Version for p01he |
| `DbV1007` | db,ry1h | `` | 20011007/0 Db Version for p01hi |
| `DbV1107` | db,ry1h | `` | 20011107/0 Db Version for pass1 p01gk |
| `DbV1211` | db,ry1h | `` | 20011211/0 Db Version for prod p01gl |

</details>

<details>
<summary><b>Geometry</b> (8 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `ideal` |  | `` | Ideal Alignment |
| `misalign` |  | `` | Misaligned Geometry |
| `AgMLutil` |  | `` | AgML support |
| `AgMLlib` |  | `` | AgML support |
| `AgML` | AgMLlib,-Agi,-VmcGeo | `` | Alias VmcGeometry to AgiLGeometry |
| `AgML` | -Agi,-VmcGeo | `` | Alias VmcGeometry to AgiLGeometry |
| `Agi` | -VmcGeo | `` | Alias VmcGeometry to AgiGeometry (gstar original geometry) |
| `VmcGeo` | -AgML,-Agi | `` | Alias VmcGeometry to VmcGeo |

</details>

<details>
<summary><b>Trigger Type</b> (6 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `Physics` |  | `` | WARNING *** Option is OBSOLETE *** |
| `LaserTest` |  | `` | WARNING *** Option is OBSOLETE *** |
| `PulserSvt` |  | `` | WARNING *** Option is OBSOLETE *** |
| `alltrigger` |  | `` | WARNING *** Option is OBSOLETE *** |
| `phys_off` |  | `` | Turn off physics in simulation |
| `hadr_off` |  | `` | Turn off hadronic interactions in simulation |

</details>

<details>
<summary><b>Calibration chains</b> (27 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `LanaDV` | LanaDVtpx | `` | get LDV |
| `LanaDVtpx` | MakeEvent,trgd,in,tpx,TpcHitMover,LaserIT,VFMinuit,Lana,Analysis,Corr4,NosvtIT,NossdIT | `` | get LDV with TPX |
| `LaserDV.Chain` | in,LaserCal,fcf,TpcHitMover,OGridLeak3D,OShortR,OSpaceZ2 | `` | get LDV |
| `Test.default.ITTF` | TpcRS,Simu,sss,svt,ssd,fss,bbcSim,IdTruth,MakeEvent,genvtx,miniMcMk,McAna,Test.reco.ITTF,CMuDst | `` |  |
| `Test.default.y2005g.ITTF` | Test.default.ITTF,sdt20050130,noSimuDb | `` |  |
| `Test.default.y2007g.ITTF` | Test.default.ITTF,sdt20070322,noSimuDb | `` |  |
| `Test.fast.y2005g.ITTF` | Test.fast.ITTF,sdt20050130,noSimuDb | `` |  |
| `Test.reco.StiVMC` | MakeEvent,tpcI,tcl,ftpc,SvtCL,Test.StiVMC | `` |  |
| `Test.default.StiVMC` | TpcRS,Simu,sss,svt,ssd,fss,bbcSim,IdTruth,MakeEvent,miniMcMk,McAna,Test.reco.StiVMC,CMuDst | `` |  |
| `Test.StiVMC` | TpcRS,StiVMC,event,analysis,tags,EvOut,StarMagField,FieldOn,Idst,CMuDst | `` |  |
| `Test.VeryFast.StiVMC` | TpcFastSim,Simu,sfs,ssdfast,McEvOut,GeantOut,IdTruth,miniMcMk,McAna,SvtCL,tpc_T,globT,tls,db,tpcDB,svtDb,svtIT,ssdIT,StiVMC,Idst,event,analysis,EventQA,tags,EvOut,StarMagField,FieldOn,IAna,CMuDst | `` |  |
| `Test.fast.StiVMC` | tfs,Simu,sfs,ssdfast,McEvOut,GeantOut,IdTruth,miniMcMk,McAna,SvtCL,tpc_T,globT,tls,db,tpcDB,svtDb,StiVMC,Idst,event,analysis,EventQA,tags,EvOut,StarMagField,FieldOn,IAna,CMuDst | `` |  |
| `Test.fast.y2007g.ITTF` | Test.fast.ITTF,sdt20070322,noSimuDb | `` |  |
| `Test.VeryFast.y2005g.ITTF` | Test.VeryFast.ITTF,sdt20050130,noSimuDb | `` |  |
| `Test.VeryFast.y2007g.ITTF` | Test.VeryFast.ITTF,sdt20070322,noSimuDb | `` |  |
| `Test.default.StiVMC` | TpcRS,Simu,sss,svt,ssd,fss,bbcSim,IdTruth,MakeEvent,miniMcMk,McAna,Test.reco.ITTF,CMuDst | `` |  |
| `Test.y2009.ITTF` | Test.default.ITTF,y2009,TpcRS,sdt20090428.141700 | `` |  |
| `Test.y2009.StiVMC` | Test.default.StiVMC,y2009,TpcRS,sdt20090428.141700,noSimuDb | `` |  |
| `Test.fast.y2005g.StiVMC` | Test.fast.StiVMC,sdt20050130,noSimuDb | `` |  |
| `Test.VeryFast.y2005g.StiVMC` | Test.VeryFast.StiVMC,sdt20050130,noSimuDb | `` |  |
| `Test.VeryFast.y2007g.StiVMC` | Test.VeryFast.StiVMC,sdt20070322,noSimuDb | `` |  |
| `Test.default.Fast.ITTF` | tfs,sfs,ssdFast,IdTruth,MakeEvent,miniMcMk,Test.ITTF | `` |  |
| `Test.srs.ITTF` | TpcRS,Simu,srs,svt,ssd,fss,bbcSim,emcY2,McEvOut,GeantOut,IdTruth,miniMcMk,McAna,Test.reco.ITTF,CMuDst | `` |  |
| `Test.ITTF` | svtIT,ssdIT,ITTF,event,analysis,EventQA,tags,EvOut,StarMagField,FieldOn,IDST,CMuDst,analysis | `` |  |
| `Test.reco.ITTF` | MakeEvent,tpcI,tcl,ftpc,SvtCL,Test.ITTF | `` |  |
| `Test.fast.ITTF` | tfs,Simu,sfs,ssdfast,McEvOut,GeantOut,IdTruth,miniMcMk,McAna,SvtCL,tpc_T,globT,tls,db,tpcDB,svtDb,svtIT,ssdIT,ITTF,Idst,event,analysis,EventQA,tags,EvOut,StarMagField,FieldOn,IAna,CMuDst | `` |  |
| `Test.VeryFast.ITTF` | TpcFastSim,Simu,sfs,ssdfast,McEvOut,GeantOut,IdTruth,miniMcMk,McAna,SvtCL,tpc_T,globT,tls,db,tpcDB,svtDb,svtIT,ssdIT,ITTF,Idst,event,analysis,EventQA,tags,EvOut,StarMagField,FieldOn,IAna,CMuDst | `` |  |

</details>

<details>
<summary><b>MC----------</b> (26 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `MC.y2000` | trs,rrs,y1h,Idst,rich,IAna,l0,tpcI,tpxClu,NosvtIT,NossdIT,-Kink2,VFMinuit,geant,evout,IdTruth,tags,GeantOut,big | `` |  |
| `MC.y2001` | trs,fss,rrs,y2001n,Idst,rich,IAna,l0,tpcI,tpxClu,ftpc,-Kink2,VFMinuit,geant,evout,IdTruth,tags,emcY2,GeantOut,big | `` |  |
| `MC.pp.y2001` | trs,rrs,fss,y2001n,Idst,rich,IAna,l0,tpcI,tpxClu,ftpc,-Kink2,VFMinuit,geant,evout,IdTruth,tags,emcY2,MiniMcMk,GeantOut,big | `` |  |
| `MC.y2003` | trs,fss,y2003,Idst,IAna,l0,tpcI,tpxClu,ftpc,VFMinuit,bbcSim,tofsim,tags,emcY2,evout,IdTruth,geantout | `` |  |
| `MC.y2004` | trs,srs,fss,y2004,Idst,BAna,l0,tpcI,tpxClu,ftpc,VFMinuit,SvtIt,geant,evout,tags,bbcSim,tofsim,emcY2,EEfs,GeantOut,big | `` |  |
| `MC.y2004a` | trs,srs,fss,y2004a,Idst,BAna,l0,tpcI,tpxClu,ftpc,VFMinuit,SvtIT,geant,tags,bbcSim,tofsim,emcY2,EEfs,evout,GeantOut,big | `` |  |
| `MC.y2005` | trs,srs,fss,ssd,y2005x,Idst,IAna,l0,tpcI,tpxClu,ftpc,SvtCL,svtDb,SsdIt,SvtIt,VFMinuit,geant,evout,tags,bbcSim,tofsim,emcY2,EEfs,GeantOut,big | `` |  |
| `MC.y2006` | trs,fss,y2006h,Idst,IAna,l0,tpcI,tpxClu,ftpc,VFMinuit,NoSsdIt,NoSvtIt,MakeEvent,bbcSim,tofsim,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2007` | trs,srs,ssd,fss,y2007,Idst,BAna,tpcI,tpxClu,ftpc,SvtIt,SsdIt,VFMinuit,MakeEvent,IdTruth,geant,tags,bbcSim,tofsim,emcY2,EEfs,evout,GeantOut,big | `` |  |
| `MC.y2008` | trs,fss,y2008a,Idst,IAna,tpcI,tpxClu,ftpc,VFMinuit,NoSsdIt,NoSvtIt,MakeEvent,bbcSim,tofsim,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.in.y2008` | in,y2008e,FieldOn,ReverseField,Idst,BAna,ftpcT,fpt,NoSsdIt,NoSvtIt,VFMinuit,tpcDB,MakeEvent,IdTruth,tags,bbcsim,emcY2,EEfs,evout,big,McEvout,MiniMcMk,ReadAll | `` |  |
| `MC.in.y2009` | in,y2009c,FieldOn,ReverseField,Idst,BAna,ftpcT,fpt,NoSsdIt,NoSvtIt,VFMinuit,tpcDB,MakeEvent,IdTruth,tags,bbcsim,emcY2,EEfs,evout,big,McEvout,MiniMcMk,ReadAll | `` |  |
| `MC.y2009` | TpcRS,TpxClu,fss,y2009,Idst,IAna,tpcI,ftpc,VFMinuit,NoSsdIt,NoSvtIt,MakeEvent,bbcSim,tofsim,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2009a` | TpcRS,TpxClu,fss,y2009a,Idst,IAna,tpcI,ftpc,VFMinuit,NoSsdIt,NoSvtIt,MakeEvent,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2010a` | TpcRS,TpxClu,y2010a,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2010` | TpcRS,TpxClu,y2010,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.fast.pp.y2011` | y2011,Test.default.Fast.ITTF,bbcSim,btofsim,btofMatch,emcY2,emcSim,EEfs,NosvtIT,NossdIT,-sfs,-ssdFast,VFPPVnoCTB,beamline | `` |  |
| `MC.in.y2010` | in,y2010c,FieldOn,ReverseField,Idst,BAna,ftpcT,fpt,NoSsdIt,NoSvtIt,VFMinuit,tpcDB,MakeEvent,IdTruth,tags,bbcsim,emcY2,EEfs,evout,big,McEvout,MiniMcMk,ReadAll | `` |  |
| `MC.y2011` | TpcRS,TpxClu,y2011,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.fast.y2011` | y2011,Test.default.Fast.ITTF,NosvtIT,NossdIT,-sfs,-ssdFast,VFPPVnoCTB,beamline,emcy2 | `` |  |
| `MC.in.y2011` | in,y2011,FieldOn,ReverseField,Idst,BAna,ftpcT,fpt,NoSsdIt,NoSvtIt,VFMinuit,tpcDB,MakeEvent,IdTruth,tags,bbcsim,emcY2,EEfs,evout,big,McEvout,MiniMcMk,ReadAll | `` |  |
| `MC.y2012` | TpcRS,TpxClu,y2012,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2012a` | TpcRS,TpxClu,y2012a,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2012.eval` | in,y2012,FieldOn,ReverseField,Idst,BAna,ftpcT,fpt,NoSsdIt,NoSvtIt,VFMinuit,tpcDB,MakeEvent,IdTruth,tags,bbcsim,emcY2,EEfs,evout,big,McEvout,MiniMcMk,ReadAll | `` |  |
| `MC.y2013` | TpcRS,TpxClu,y2013_1x,useXgeom,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |
| `MC.y2014` | TpcRS,TpxClu,y2014,useXgeom,MakeEvent,NoSsdIt,NoSvtIt,Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big | `` |  |

</details>

<details>
<summary><b>RC----------</b> (41 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `RC.y2000` | p2000,VFMinuit,CMuDst,NosvtIT,NossdIT | `` |  |
| `RC.y2001` | P2001a,VFMinuit,ZDCvtx,CMuDst,NosvtIT,NossdIT | `` |  |
| `RC.pp.y2001` | pp2001a,VFMinuit,CMuDst,NossdIT | `` |  |
| `RC.pp.y2001.ppv` | pp2001a,VFPPV,beamLine,CMuDst,NossdIT | `` |  |
| `RC.y2003` | DbV20040520,dau2003i,in,-SvtIT,NossdIT | `` |  |
| `RC.pp.y2003.VFPPV` | pp2003,VFPPV,beamLine,CMuDst,-svtIT,NossdIT,Corr2,v0,xi | `` |  |
| `RC.y2004` | P2004,DbV20041213 | `` |  |
| `RC.y2004.NoSvt` | P2004,DbV20041213,-SsdIt,-SvtIt,pmdRaw | `` |  |
| `RC.y2004.NoSvt.pmd` | P2004,DbV20041213,pmdRaw,-SvtIT,-SsdIT | `` |  |
| `RC.pp.y2004` | pp2004,DbV20041213,beamLine | `` |  |
| `RC.y2005` | P2005,tofDat,MakeEvent,ssddat,sptd,SsdIt,SvtIt,pmdRaw,OShortR,OSpaceZ2 | `` |  |
| `RC.pp.y2005` | pp2005a,tofdat,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2006` | pp2006b,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2007` | DbV20080418,B2007g,IAna,VFMinuit3,emcDY2,ftpc,trgd,ZDCvtx,svtIT,ssdIT,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2007.NoSvt` | DbV20080418,B2007g,IAna,VFMinuit3,emcDY2,ftpc,trgd,ZDCvtx,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2008` | DbV20080712,P2008,OSpaceZ2,OGridLeak3D,beamLine | `` |  |
| `RC.y2008.notof` | DbV20080712,P2008,-ToF,-tofDat,-tofrMatch,-tofpMatch,-tofCalib,OSpaceZ2,OGridLeak3D,beamLine | `` |  |
| `RC.pp.y2008` | DbV20080712,pp2008,OSpaceZ2,OGridLeak3D,beamLine | `` |  |
| `RC.pp.y2008.Minuit` | DbV20080712,pp2008,-VFPPV,VFMinuit,-ToF,-tofDat,-tofrMatch,-tofpMatch,-tofCalib,OSpaceZ2,OGridLeak3D,beamLine | `` |  |
| `RC.pp.y2009` | pp2009c,VFMinuit,beamLine,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2009.notof` | pp2009c,VFMinuit,beamLine,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2009.VFPP` | pp2009c,VFPPVnoCTB,beamLine,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2010` | P2010a,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,pmdReco | `` |  |
| `RC.y2010.notof` | P2010a,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2011.VFPPV` | pp2011a,VFPPVnoCTB,beamline,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2011` | pp2011a,VFMinuit,beamline,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2011` | P2011a,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,pmdReco,mtdDat | `` |  |
| `RC.y2011.notof` | P2011a,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D,pmdReco,mtdDat | `` |  |
| `RC.y2012` | P2012a,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,mtdDat,fmsDat | `` |  |
| `RC.y2012.notof` | P2012a,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2012` | pp2012a,VFPPVnoCTB,beamline,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,mtdDat,fmsDat | `` |  |
| `RC.pp.y2012.notof` | pp2012a,VFPPVnoCTB,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D, | `` |  |
| `RC.pp.y2012.notofMin` | pp2012a,VFMinuit,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2012b` | P2012b,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,mtdDat,fmsDat | `` |  |
| `RC.y2012b.notof` | P2012b,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2012b` | pp2012b,VFPPVnoCTB,beamline,BEmcChkStat,btof,Corr4,OSpaceZ2,OGridLeak3D,mtdDat,fmsDat | `` |  |
| `RC.pp.y2012b.notof` | pp2012b,VFPPVnoCTB,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2012b.notofMin` | pp2012b,VFMinuit,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2012b.notofMin` | pp2012b,VFMinuit,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.pp.y2013` | pp2013a,mtd,btof,fmsDat,fgt,fgtPoint,VFPPVnoCTB,beamline,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |
| `RC.y2014` | P2014a,mtd,btof,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D | `` |  |

</details>

<details>
<summary><b>MC nightlies and Eval</b> (340 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `test_MC.stan.y2000` | MC.y2000,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/cocktail/hadronic/default/standard/year_1h/half_field/hadronic_on/Gstardata/hc_standard.40_evts.fz |
| `test_MC.pp.y2001` | MC.pp.y2001,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp200/pythia/default/minbias/year2001/hadronic_on/gstardata/pds0200_04_12812evts.fzd |
| `test_MC.stan.y2001` | MC.y2001,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/cocktail/hadronic/default/standard/year2001/hadronic_on/Gstardata/hc_standard.40_evts.fz |
| `test_dau.MC.y2003` | MC.y2003,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1197_05_5940evts.fzd |
| `test_auauCtr.MC.y2004` | MC.y2004a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1209_05_80evts.fzd |
| `test_auau.MC.y2004` | MC.y2004,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1207_01_225evts.fzd |
| `test_cucu200.MC.y2005` | MC.y2005,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1216_05_200evts.fzd |
| `test_cucu62.MC.y2005` | MC.y2005,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1237_01_500evts.fzd |
| `test_pp200.MC.y2006` | MC.y2006,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9991_01_1000evts.fzd |
| `test_auau200.MC.y2007` | MC.y2007,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1296_02_100evts.fzd |
| `test_dau200.MC.y2008` | MC.y2008,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9066_20_1000evts.fzd |
| `test_pp200.MC.y2008` | MC.y2008,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9992_01_1000evts.fzd |
| `test_pp200.MC.y2009` | MC.y2009a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9993_01_1000evts.fzd |
| `test_pp500.MC.y2009` | MC.y2009a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9994_01_1000evts.fzd |
| `test_auau11.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10031_1_100evts.fzd |
| `test_auau200.MC.y2010` | MC.y2010,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9068_305_100evts.fzd |
| `test_auau39.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10032_1_100evts.fzd |
| `test_auau62.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10033_1_100evts.fzd |
| `test_auau7.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10030_1_100evts.fzd |
| `test_auau200.MC.y2011` | MC.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf11023_2060_25evts.fzd |
| `test_pp500.MC.y2011` | MC.fast.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd |
| `test_pp500.pileup.MC.y2011` | MC.fast.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_200evts_Wplus_enu.fzd\n gfile b /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd\n mode TPCE back 4001400\n gback 400 400 0.1 106.6 |
| `test_CuAu200.MC.AgML.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12003_1_100evts.fzd |
| `test_CuAu200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12003_1_100evts.fzd |
| `test_pp200.MC.AgML.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12000_1_1000evts.fzd |
| `test_pp200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12000_1_1000evts.fzd |
| `test_pp500.MC.AgML.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12001_1_1000evts.fzd |
| `test_pp500.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12001_1_1000evts.fzd |
| `test_UU200.MC.AgML.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12002_1_100evts.fzd |
| `test_UU200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12002_1_100evts.fzd |
| `test.RC.centr.y2000` | RC.y2000,Sti | `` | /star/rcf/test/daq/2000/09/st_physics_1248022_raw_0001.daq |
| `test.RC.minb.y2000` | RC.y2000,Sti | `` | /star/rcf/test/daq/2000/08/st_physics_1229021_raw_0003.daq |
| `test.RC.cent.y2001d` | RC.y2001,v02,xi2,Sti | `` | /star/rcf/test/daq/2001/327/st_physics_2327038_raw_0010.daq |
| `test.RC.cent.y2001` | RC.y2001,v0,xi,Sti | `` | /star/rcf/test/daq/2001/327/st_physics_2327038_raw_0010.daq |
| `test.RC.minb.y2001d` | RC.y2001,v02,xi2,Sti | `` | /star/rcf/test/daq/2001/295/st_physics_2295030_raw_0010.daq |
| `test.RC.minb.y2001` | RC.y2001,v0,xi,Sti | `` | /star/rcf/test/daq/2001/295/st_physics_2295030_raw_0010.daq |
| `test.RC.pp.y2001d` | RC.pp.y2001,v02,xi2,Sti | `` | /star/rcf/test/daq/2002/008/st_physics_3008016_raw_0001.daq |
| `test.RC.pp.y2001` | RC.pp.y2001.ppv,v0,xi,Sti | `` | /star/rcf/test/daq/2002/008/st_physics_3008016_raw_0001.daq |
| `test.RC.pp.y2001_vfppvd` | RC.pp.y2001.ppv,v02,xi2,Sti | `` | /star/rcf/test/daq/2002/008/st_physics_3008016_raw_0001.daq |
| `test.RC.dau.y2003` | RC.y2003,ITTF,Sti | `` | /star/rcf/test/daq/2003/041/st_physics_4041002_raw_0020001.daq |
| `test.RC.pp.y2003` | RC.pp.y2003.VFPPV,Sti | `` | /star/rcf/test/daq/2003/095/st_physics_4095050_raw_0010002.daq |
| `test.RC.auau.lo.y2004` | RC.y2004,-SvtIT,-SsdIT,pmdRaw,ITTF,Sti | `` | /star/rcf/test/daq/2004/044/st_physics_5044116_raw_3010002.daq |
| `test.RC.auau.ph.y2004` | RC.y2004,pmdRaw,-SvtIT,-SsdIT,ITTF,Sti | `` | /star/rcf/test/daq/2004/044/st_physics_5044102_raw_1010003.daq |
| `test.RC.auau.StiCA.y2004` | RC.y2004,-SsdIt,-SvtIt,pmdRaw,StiCA | `` | /star/rcf/test/daq/2004/028/st_physics_5028066_raw_1010003.daq |
| `test.RC.auau.y2004` | RC.y2004,-SsdIt,-SvtIt,pmdRaw,ITTF,Sti | `` | /star/rcf/test/daq/2004/028/st_physics_5028066_raw_1010003.daq |
| `test.RC.pp.y2004` | RC.pp.y2004,ITTF,Sti | `` | /star/rcf/test/daq/2004/134/st_physics_5134013_raw_2010010.daq |
| `test.RC.cucu200.ht.y2005` | RC.y2005,ITTF,Sti | `` | /star/rcf/test/daq/2005/054/st_physics_6054016_raw_1020005.daq |
| `test.RC.cucu200.y2005` | RC.y2005,ITTF,Sti | `` | /star/rcf/test/daq/2005/048/st_physics_6048025_raw_1020002.daq |
| `test.RC.cucu22.y2005` | RC.y2005,ITTF,Sti | `` | /star/rcf/test/daq/2005/083/st_physics_6083006_raw_1040002.daq |
| `test.RC.cucu62.y2005` | RC.y2005,ITTF,Sti | `` | /star/rcf/test/daq/2005/080/st_physics_6080011_raw_1020004.daq |
| `test.RC.pp200.y2005` | RC.pp.y2005,ITTF,Sti | `` | /star/rcf/test/daq/2005/171/st_physics_6171062_raw_2040010.daq |
| `test.RC.pp200.Long.y2006` | RC.pp.y2006,ITTF,Sti | `` | /star/rcf/test/daq/2006/155/7155010/st_physics_7155010_raw_1020003.daq |
| `test.RC.pp200.Trans.y2006` | RC.pp.y2006,ITTF,Sti | `` | /star/rcf/test/daq/2006/129/7129023/st_physics_7129023_raw_1020003.daq |
| `test.RC.auau200.MB.y2007` | RC.y2007,pmdReco,ITTF,Sti | `` | /star/rcf/test/daq/2007/113/8113044/st_physics_8113044_raw_1040042.daq |
| `test.RC.auau200.y2007` | RC.y2007,pmdReco,ITTF,Sti | `` | /star/rcf/test/daq/2007/112/8112052/st_physics_8112052_raw_1020010.daq |
| `test.RC.dau200.y2008` | RC.y2008,ITTF,Sti | `` | /star/rcf/test/daq/2007/352/st_physics_8352025_raw_1030011.daq |
| `test.RC.pp200.y2008` | RC.pp.y2008,ITTF,Sti | `` | /star/rcf/test/daq/2008/043/st_physics_9043046_raw_2030002.daq |
| `test.RC.pp200.y2009` | RC.pp.y2009.VFPP,ITTF,Sti | `` | /star/rcf/test/daq/2009/115/st_physics_10115020_raw_5020001.daq |
| `test.RC.pp500.y2009` | RC.pp.y2009.VFPP,ITTF,Sti | `` | /star/rcf/test/daq/2009/085/st_physics_10085024_raw_2020001.daq |
| `test.RC.auau11.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/157/st_physics_11157020_raw_2030001.daq |
| `test.RC.auau200.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/029/st_physics_11029020_raw_1030002.daq |
| `test.RC.auau39.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/100/st_physics_11100070_raw_1030001.daq |
| `test.RC.auau62.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/098/st_physics_11098050_raw_3020001.daq |
| `test.RC.auau7.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/143/st_physics_11143043_raw_1020001.daq |
| `test.RC.auau200.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/130/st_physics_12130084_raw_5020002.daq |
| `test.RC.auau20.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/122/st_physics_12122018_raw_2010002.daq |
| `test.RC.auau27.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/174/st_physics_12174106_raw_2040001.daq |
| `test.RC.pp500.y2011` | RC.pp.y2011.VFPPV,pmdReco,mtdDat,Sti | `` | /star/rcf/test/daq/2011/059/st_physics_12059038_raw_2030002.daq |
| `test.RC.cuAu200.AgML.y2012` | RC.y2012b,AgML,Sti | `` | /star/rcf/test/daq/2012/143/st_physics_13143018_raw_3020009.daq |
| `test.RC.cuAu200.y2012` | RC.y2012b,Sti | `` | /star/rcf/test/daq/2012/143/st_physics_13143018_raw_3020009.daq |
| `test.RC.pp200.AgML.y2012` | RC.pp.y2012b,AgML,Sti | `` | /star/rcf/test/daq/2012/044/st_physics_13044030_raw_2010001.daq |
| `test.RC.pp200.y2012` | RC.pp.y2012b,Sti | `` | /star/rcf/test/daq/2012/044/st_physics_13044030_raw_2010001.daq |
| `test.RC.pp500.AgML.y2012` | RC.pp.y2012b,AgML,Sti | `` | /star/rcf/test/daq/2012/082/st_physics_13082004_raw_1020004.daq |
| `test.RC.pp500.y2012` | RC.pp.y2012b,Sti | `` | /star/rcf/test/daq/2012/082/st_physics_13082004_raw_1020004.daq |
| `test.RC.UU193.AgML.y2012` | RC.y2012b,AgML,Sti | `` | /star/rcf/test/daq/2012/115/st_physics_13115004_raw_2010002.daq |
| `test.RC.UU193.y2012` | RC.y2012b,Sti | `` | /star/rcf/test/daq/2012/115/st_physics_13115004_raw_2010002.daq |
| `test.RC.pp500.y2013` | RC.pp.y2013,Sti | `` | /star/rcf/test/daq/2013/079/st_physics_14079008_raw_1920004.daq |
| `test.RC.AuAu15.y2014` | RC.y2014,Sti | `` | /star/rcf/test/daq/2014/069/st_physics_15069008_raw_2500008.daq |
| `test.RC.AuAu200.y2014` | RC.y2014,Sti | `` | /star/rcf/test/daq/2014/086/st_physics_15086051_raw_2500017.daq |
| `test.RC.AuAu200.mid.y2014` | RC.y2014,Sti | `` | /star/rcf/test/daq/2014/146/st_mtd_15146050_raw_1000030.daq |
| `test.RC.He3Au200.y2014` | RC.y2014,Sti | `` | /star/rcf/test/daq/2014/171/st_physics_15171039_raw_2000008.daq |
| `eval_Sti_auau200.MC.y2007` | MC.y2007,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1296_02_100evts.fzd |
| `eval_StiCA_auau200.MC.y2007` | MC.y2007,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf1296_02_100evts.fzd |
| `eval_Sti.AgML_dau200.MC.y2008` | MC.y2008,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9066_20_1000evts.fzd |
| `eval_Sti.AgML_pp200.MC.y2008` | MC.y2008,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9992_01_1000evts.fzd |
| `eval_StiCA_dau200.MC.y2008` | MC.y2008,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9066_20_1000evts.fzd |
| `eval_StiCA_pp200.MC.y2008` | MC.y2008,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9992_01_1000evts.fzd |
| `eval_Sti_dau200.MC.y2008` | MC.y2008,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9066_20_1000evts.fzd |
| `eval_Sti_pp200.MC.y2008` | MC.y2008,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9992_01_1000evts.fzd |
| `eval_Sti.AgML_pp500.MC.y2009` | MC.y2009a,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9994_01_1000evts.fzd |
| `eval_StiCA_pp200.MC.y2009` | MC.y2009a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9993_01_1000evts.fzd |
| `eval_StiCA_pp500.MC.y2009` | MC.y2009a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9994_01_1000evts.fzd |
| `eval_Sti_pp200.MC.y2009` | MC.y2009a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9993_01_1000evts.fzd |
| `eval_Sti_pp500.MC.y2009` | MC.y2009a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9994_01_1000evts.fzd |
| `eval_Sti.AgML_auau11.MC.y2010` | MC.y2010a,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10031_1_100evts.fzd |
| `eval_Sti.AgML_auau200.MC.y2010` | MC.y2010,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9068_305_100evts.fzd |
| `eval_Sti_auau11.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10031_1_100evts.fzd |
| `eval_Sti_auau200.MC.y2010` | MC.y2010,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9068_305_100evts.fzd |
| `eval_Sti_auau39.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10032_1_100evts.fzd |
| `eval_Sti_auau62.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10033_1_100evts.fzd |
| `eval_Sti_auau7.MC.y2010` | MC.y2010a,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10030_1_100evts.fzd |
| `eval_StiCA_auau11.MC.y2010` | MC.y2010a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10031_1_100evts.fzd |
| `eval_StiCA_auau200.MC.y2010` | MC.y2010,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf9068_305_100evts.fzd |
| `eval_StiCA_auau39.MC.y2010` | MC.y2010a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10032_1_100evts.fzd |
| `eval_StiCA_auau62.MC.y2010` | MC.y2010a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10033_1_100evts.fzd |
| `eval_StiCA_auau7.MC.y2010` | MC.y2010a,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/rcf10030_1_100evts.fzd |
| `eval_Sti.AgML_auau200.MC.y2011` | MC.y2011,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf11023_2060_25evts.fzd |
| `eval_Sti.AgML_pp500.MC.y2011` | MC.fast.y2011,AgML,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd |
| `eval_Sti_auau200.MC.y2011` | MC.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf11023_2060_25evts.fzd |
| `eval_StiCA_pp500.MC.y2011` | MC.fast.y2011,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd |
| `eval_StiCA_pp500.pileup.MC.y2011` | MC.fast.y2011,StiCA,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_200evts_Wplus_enu.fzd\n gfile b /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd\n mode TPCE back 4001400\n gback 400 400 0.1 106.6 |
| `eval_Sti_pp500.MC.y2011` | MC.fast.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd |
| `eval_Sti_pp500.pileup.MC.y2011` | MC.fast.y2011,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_200evts_Wplus_enu.fzd\n gfile b /star/rcf/simu/pp500/pythia/pileup/rcf10100_90_4000evts_minb.fzd\n mode TPCE back 4001400\n gback 400 400 0.1 106.6 |
| `eval_Sti.AgML_CuAu200.MC.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12003_1_100evts.fzd |
| `eval_Sti.AgML_pp200.MC.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12000_1_1000evts.fzd |
| `eval_Sti.AgML_pp500.MC.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12001_1_1000evts.fzd |
| `eval_Sti.AgML_UU200.MC.y2012` | MC.y2012,Sti,AgML,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12002_1_100evts.fzd |
| `eval_Sti_CuAu200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12003_1_100evts.fzd |
| `eval_Sti_pp200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12000_1_1000evts.fzd |
| `eval_Sti_pp500.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12001_1_1000evts.fzd |
| `eval_Sti_UU200.MC.y2012` | MC.y2012,Sti,fzin,MiniMcMk | `` | /star/rcf/simu/rcf12002_1_100evts.fzd |
| `eval_Sti.AgML.RC.auau200.MB.y2007` | RC.y2007,pmdReco,ITTF,AgML,Sti | `` | /star/rcf/test/daq/2007/113/8113044/st_physics_8113044_raw_1040042.daq |
| `eval_StiCA.RC.auau200.MB.y2007` | RC.y2007,pmdReco,StiCA | `` | /star/rcf/test/daq/2007/113/8113044/st_physics_8113044_raw_1040042.daq |
| `eval_StiCA.RC.auau200.y2007` | RC.y2007,pmdReco,StiCA | `` | /star/rcf/test/daq/2007/112/8112052/st_physics_8112052_raw_1020010.daq |
| `eval_Sti.RC.auau200.MB.y2007` | RC.y2007,pmdReco,ITTF,Sti | `` | /star/rcf/test/daq/2007/113/8113044/st_physics_8113044_raw_1040042.daq |
| `eval_Sti.AgML.RC.dau200.y2008` | RC.y2008,ITTF,AgML,Sti | `` | /star/rcf/test/daq/2007/352/st_physics_8352025_raw_1030011.daq |
| `eval_StiCA.RC.dau200.y2008` | RC.y2008,StiCA | `` | /star/rcf/test/daq/2007/352/st_physics_8352025_raw_1030011.daq |
| `eval_StiCA.RC.pp200.y2008` | RC.pp.y2008,StiCA | `` | /star/rcf/test/daq/2008/043/st_physics_9043046_raw_2030002.daq |
| `eval_Sti.RC.dau200.y2008` | RC.y2008,ITTF,Sti | `` | /star/rcf/test/daq/2007/352/st_physics_8352025_raw_1030011.daq |
| `eval_Sti.RC.pp200.y2008` | RC.pp.y2008,ITTF,Sti | `` | /star/rcf/test/daq/2008/043/st_physics_9043046_raw_2030002.daq |
| `eval_Sti.AgML.RC.pp500.y2009` | RC.pp.y2009.VFPP,ITTF,Sti | `` | /star/rcf/test/daq/2009/085/st_physics_10085024_raw_2020001.daq |
| `eval_StiCA.RC.pp200.y2009` | RC.pp.y2009.VFPP,StiCA | `` | /star/rcf/test/daq/2009/115/st_physics_10115020_raw_5020001.daq |
| `eval_StiCA.RC.pp500.y2009` | RC.pp.y2009.VFPP,StiCA | `` | /star/rcf/test/daq/2009/085/st_physics_10085024_raw_2020001.daq |
| `eval_Sti.RC.pp200.y2009` | RC.pp.y2009.VFPP,ITTF,Sti | `` | /star/rcf/test/daq/2009/115/st_physics_10115020_raw_5020001.daq |
| `eval_Sti.RC.pp500.y2009` | RC.pp.y2009.VFPP,ITTF,Sti | `` | /star/rcf/test/daq/2009/085/st_physics_10085024_raw_2020001.daq |
| `eval_Sti.AgML.RC.auau200.y2010` | RC.y2010.notof,AgML,Sti | `` | /star/rcf/test/daq/2010/029/st_physics_11029020_raw_1030002.daq |
| `eval_Sti.AgML.RC.auau39.y2010` | RC.y2010.notof,AgML,Sti | `` | /star/rcf/test/daq/2010/100/st_physics_11100070_raw_1030001.daq |
| `eval_StiCA.RC.auau11.y2010` | RC.y2010,StiCA | `` | /star/rcf/test/daq/2010/157/st_physics_11157020_raw_2030001.daq |
| `eval_StiCA.RC.auau200.y2010` | RC.y2010,StiCA | `` | /star/rcf/test/daq/2010/029/st_physics_11029020_raw_1030002.daq |
| `eval_StiCA.RC.auau39.y2010` | RC.y2010,StiCA | `` | /star/rcf/test/daq/2010/100/st_physics_11100070_raw_1030001.daq |
| `eval_StiCA.RC.auau62.y2010` | RC.y2010,StiCA | `` | /star/rcf/test/daq/2010/098/st_physics_11098050_raw_3020001.daq |
| `eval_StiCA.RC.auau7.y2010` | RC.y2010,StiCA | `` | /star/rcf/test/daq/2010/138/st_physics_11138001_raw_2020001.daq |
| `eval_Sti.RC.auau11.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/157/st_physics_11157020_raw_2030001.daq |
| `eval_Sti.RC.auau200.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/029/st_physics_11029020_raw_1030002.daq |
| `eval_Sti.RC.auau39.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/100/st_physics_11100070_raw_1030001.daq |
| `eval_Sti.RC.auau62.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/098/st_physics_11098050_raw_3020001.daq |
| `eval_Sti.RC.auau7.y2010` | RC.y2010,Sti | `` | /star/rcf/test/daq/2010/138/st_physics_11138001_raw_2020001.daq |
| `eval_Sti.AgML.RC.auau200.y2011` | RC.y2011,AgML,Sti | `` | /star/rcf/test/daq/2011/130/st_physics_12130084_raw_5020002.daq |
| `eval_Sti.AgML.RC.auau20.y2011` | RC.y2011,AgML,Sti | `` | /star/rcf/test/daq/2011/122/st_physics_12122018_raw_2010002.daq |
| `eval_Sti.AgML.RC.pp500.y2011` | RC.pp.y2011.VFPPV,pmdReco,mtdDat,Sti | `` | /star/rcf/test/daq/2011/059/st_physics_12059038_raw_2030002.daq |
| `eval_StiCA.RC.auau200.y2011` | RC.y2011,StiCA | `` | /star/rcf/test/daq/2011/130/st_physics_12130084_raw_5020002.daq |
| `eval_StiCA.RC.auau20.y2011` | RC.y2011,StiCA | `` | /star/rcf/test/daq/2011/122/st_physics_12122018_raw_2010002.daq |
| `eval_StiCA.RC.auau27.y2011` | RC.y2011,StiCA | `` | /star/rcf/test/daq/2011/174/st_physics_12174106_raw_2040001.daq |
| `eval_StiCA.RC.pp500.y2011` | RC.pp.y2011.VFPPV,pmdReco,mtdDat,StiCA | `` | /star/rcf/test/daq/2011/059/st_physics_12059038_raw_2030002.daq |
| `eval_Sti.RC.auau200.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/130/st_physics_12130084_raw_5020002.daq |
| `eval_Sti.RC.auau20.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/122/st_physics_12122018_raw_2010002.daq |
| `eval_Sti.RC.auau27.y2011` | RC.y2011,Sti | `` | /star/rcf/test/daq/2011/174/st_physics_12174106_raw_2040001.daq |
| `eval_Sti.RC.pp500.y2011` | RC.pp.y2011.VFPPV,pmdReco,mtdDat,Sti | `` | /star/rcf/test/daq/2011/059/st_physics_12059038_raw_2030002.daq |
| `eval_Sti.AgML.RC.pp200.y2012` | RC.pp.y2012,AgML,Sti | `` | /star/rcf/test/daq/2012/044/st_physics_13044030_raw_2010001.daq |
| `eval_Sti.AgML.RC.pp500.y2012` | RC.pp.y2012,AgML,Sti | `` | /star/rcf/test/daq/2012/082/st_physics_13082004_raw_1020004.daq |
| `eval_Sti.AgML.RC.UU193.y2012` | RC.y2012,AgML,Sti | `` | /star/rcf/test/daq/2012/115/st_physics_13115004_raw_2010002.daq |
| `eval_StiCA.RC.pp200.y2012` | RC.pp.y2012,StiCA | `` | /star/rcf/test/daq/2012/044/st_physics_13044030_raw_2010001.daq |
| `eval_Sti.RC.pp200.y2012` | RC.pp.y2012,Sti | `` | /star/rcf/test/daq/2012/044/st_physics_13044030_raw_2010001.daq |
| `eval_Sti.RC.pp500.y2012` | RC.pp.y2012,Sti | `` | /star/rcf/test/daq/2012/082/st_physics_13082004_raw_1020004.daq |
| `eval_Sti.RC.UU193.y2012` | RC.y2012,Sti | `` | /star/rcf/test/daq/2012/115/st_physics_13115004_raw_2010002.daq |
| `doEvents` | in,StEvent,analysis,NoDb | `` |  |
| `MakeMuDst` | in,StEvent,CMuDST,Tree,nodefault,NoHistos,ReadAll | `` | StEvent => MuDst |
| `drawDst` | in,ry1h,globT,SCL,geant,display,NoDb,TbUtil | `` |  |
| `Cdst` | ITTF,event,analysis,EventQA | `` |  |
| `C1default` | rich,l0,Cdst,tags,Tree,EvOut,NoHits | `` | Year 1 chain |
| `C2default` | rich,l0,Cdst,tags,Tree,EvOut,ftpc,svt,emcY2 | `` | Year 2 chain |
| `C3default` | l0,Cdst,tags,Tree,EvOut,NoHits,ftpc,svt,bbcsim,emcY2 | `` | Year 3 simu base chain |
| `CAdefault` | l0,Cdst,tags,Tree,EvOut,NoHits,ftpc,svt,emcY2 | `` | Assymptotic chain |
| `Cy1a` | y1a,C1default | `` | Turn on chain y1a |
| `Cy1b` | y1b,C1default | `` | Turn on chain y1b |
| `Cy1s` | y1s,C1default | `` | Turn on chain y1s |
| `Cy1d` | y1d,C1default | `` | Turn on chain y1d |
| `cy1e` | y1e,C1default | `` | Turn on chain y1h |
| `cy1h` | y1h,C1default | `` | Turn on chain y1e |
| `Cy2a` | y2a,CAdefault | `` | Turn on chain y2a |
| `Cy2b` | y2b,C2default | `` | Turn on chain y2b |
| `C2000` | y2000,C1default | `` | Turn on chain Y2000 |
| `C2001` | y2001,C2default | `` | Turn on chain Y2001 |
| `C2003` | y2003,C3default | `` | Turn on chain Y2003 |
| `C2003X` | y2003X,C3default | `` | Turn on chain Y2003X (full B/E EMC) |
| `mdc3` | cy1h,GeantOut | `` | MDC3 default chain |
| `MDC4` | C2001,trs,tpc_daq,Simu,srs,fss,rrs,big,GeantOut | `` | Turn on chain for MDC4 |
| `MDC4New` | y2001n,C2default,trs,tpc_daq,Simu,srs,fss,rrs,big,GeantOut | `` | Turn on chain for MDC4 (for after September) |
| `PostMDC4` | C2001,trs,tpc_daq,Simu,sss,fss,rrs,big,GeantOut | `` | Turn on Post MDC4 chain |
| `ppMDC4` | ppOpt,C2001,mwc,trs,tpc_daq,Simu,srs,rrs,big,GeantOut | `` | Turn on chain for ppMDC |
| `dAuMDC` | ppOpt,C2003,trs,tpc_daq,Simu,srs,fss,big,GeantOut | `` | Chain for d+Au |
| `dAuMDCa` | ppOpt,C2003,trs,tpc_daq,Simu,srs,fss,big,GeantOut,est | `` | Chain for d+Au |
| `CComplete` | Complete,C2default | `` | Turn on chain for Complete STAR |
| `SvtD` | SvtCalDb,SvtSeqAdj,SvtClu,SvtCluAnal,SvtHit | `` | SVT chain for Data |
| `P00h` | ry1h,in,tpc_daq,tpc,rich,Physics,Cdst,Kalman,tags,Tree,evout,ExB,NoHits | `` | Production chain for summer 2000 data |
| `P2000` | ry2000,in,tpc_daq,tpc,rich,Physics,Cdst,Kalman,tags,Tree,evout,ExB,NoHits | `` | Production chain for summer 2000 data |
| `B2000` | ry2000,in,tpc_daq,tpc,rich,Physics,Cdst,Kalman,tags,Tree,evout,NosvtIT,NossdIT | `` | Base chain for 2001 (tpc+rhic) |
| `P2000a` | B2000,Corr1 | `` | Production chain for summer 2000 data |
| `B2001` | ry2001,in,tpc_daq,tpc,rich,Physics,Cdst,Kalman,tags,Tree,evout,svtDb | `` | Base chain for 2001 (tpc+rhic) |
| `P2001` | B2001,l3onl,tofDat,Corr2,OSpaceZ | `` | Production chain for summer 2001 data (+ l3, tof) |
| `P2001a` | B2001,svt_daq,SvtD,ftpc,l3onl,tofDat,emcDY2,Corr2,OSpaceZ | `` | Production chain for summer 2001 data (+ ftpc, svt, l3, tof, emc) |
| `pp2001` | ppOpt,B2001,-PreVtx,l3onl,tofDat,emcDY2,Corr2 | `` | pp 2001 (+ l3, tof, emc) |
| `pp2001a` | pp2001,svt_daq,SvtD,ftpc | `` | pp 2001 (+ ftpc, svt, l3, tof, emc) |
| `B2003` | ry2003,in,tpc_daq,tpc,Physics,Cdst,Kalman,tags,Tree,evout,svtDb | `` | Base chain for 2003 (tpc) |
| `dau2003` | B2003,Corr2,ppOpt,-PreVtx,l3onl,ToF,emcDY2,svt_daq,SvtD,ftpc | `` | Production chain for winter 2003 data (+ tof, svt (no est), ftpc, emc) |
| `dau2003a` | B2003,Corr2,ppOpt,-PreVtx,l3onl,ToF,emcDY2,svt_daq,SvtD,ftpc,trgd | `` | Production chain for winter 2003 data (+ tof, svt (no est), ftpc, emc, trgd) |
| `pp2003` | B2003,Corr2,ppOpt,-PreVtx,l3onl,ToF,emcDY2,svt_daq,SvtD,ftpc,trgd | `` | Production chain for Spring 2003 data (+ tof, svt (no est), ftpc, emc, trgd) |
| `Idst` | event,compend,EventQA | `` | Turn on DST for ITTF |
| `IAna` | dEdxY2,Kink2,xi2,CMuDst,analysis | `` | Turn on Xi, Kink, dEdx and MuDst |
| `BAna` | dEdxY2,CMuDst,analysis | `` | Turn on dEdx and MuDst |
| `B2003I` | ry2003,in,TpxRaw,TpxClu,Idst,l0,tags,Tree,evout,svtDb | `` | Base chain for 2003 ITTF |
| `dau2003i` | B2003I,IAna,CtbMatchVtx,Corr2,ppOpt,l3onl,tofDat,emcDY2,svt_daq,SvtD,ftpc,trgd | `` | Production chain for winter 2003 data dau2003a with ITTF |
| `pp2003i` | B2003I,IAna,CtbMatchVtx,Corr2,ppOpt,-PreVtx,l3onl,ToF,emcDY2,svt_daq,SvtD,ftpc,trgd | `` | Production chain for winter 2003 data dau2003a with ITTF |
| `B2004` | ry2004,in,tpc_daq,tpcI,svt_daq,SvtD,Physics,Idst,l0,tags,Tree,evout,ssdDb | `` | Base chain for 2004 ITTF (tpc+svt) |
| `P2004` | B2004,IAna,fcf,VFMinuit,ToF,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr4,OSpaceZ2 | `` | Production chain for 2003/2004 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2004` | B2004,IAna,fcf,ppOpt,VFppLMV5,CtbMatchVtx,ToF,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr4,OSpaceZ2 | `` | Production chain for 2003/2004 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `B2005` | ry2005b,in,tpc_daq,tpcI,svt_daq,SvtD,Physics,Idst,l0,tags,Tree,evout,ssdDb | `` | Base chain for 2005 ITTF (tpc+svt) |
| `B2005a` | ry2005b,in,tpc_daq,tpcI,Physics,Idst,l0,tags,Tree,evout,svtDb,ssdDb | `` | Base chain for 2005 ITTF (tpc only) |
| `B2005b` | ry2005f,in,tpc_daq,tpcI,svt_daq,SvtD,Physics,Idst,l0,tags,Tree,evout,ssdDb | `` | Base chain for 2005 ITTF Geo f (tpc+svt only) |
| `B2005c` | ry2005g,in,tpc_daq,tpcI,svt_daq,SvtD,Physics,Idst,l0,tags,Tree,evout,ssdDb | `` | Base chain for 2005 ITTF Geo g (tpc+svt only + dead material) |
| `P2005` | B2005,IAna,fcf,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr3 | `` | Production chain for 2004/2005 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2005b` | B2005b,IAna,fcf,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr3 | `` | Production chain for 2004/2005 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2005c` | B2005c,IAna,fcf,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr3 | `` | Production chain for 2004/2005 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2005` | B2005,IAna,fcf,ppOpt,VFppLMV5,CtbMatchVtx,emcDY2,ftpc,trgd,ZDCvtx,svtIT,Corr3 | `` | Production chain for 2005 pp data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2005a` | B2005a,IAna,fcf,ppOpt,VFPPV,beamline,CtbMatchVtx,emcDY2,ftpc,trgd,ZDCvtx,Corr4 | `` | Production chain for 2005 pp data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2005b` | B2005b,IAna,fcf,ppOpt,VFPPV,beamline,CtbMatchVtx,emcDY2,ftpc,trgd,ZDCvtx,Corr4 | `` | Production chain for 2005 pp data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `B2006` | ry2005d,in,tpc_daq,tpcI,svt_daq,SvtD,Idst,tags,Tree,evout,ssdDb | `` | Base chain for 2006 ITTF (tpc+svt) |
| `B2006a` | ry2005d,in,tpc_daq,tpcI,Idst,tags,Tree,evout,svtDb,ssdDb | `` | Base chain for 2006 with 2005d geom ITTF (tpc only) |
| `B2006b` | ry2006,in,tpc_daq,tpcI,Idst,l0,tags,Tree,evout,svtDb,ssdDb | `` | Base chain for 2006 ITTF (tpc only) |
| `B2006g` | ry2006g,in,tpc_daq,tpcI,Idst,l0,tags,Tree,evout,svtDb,ssdDb | `` | Base chain for 2006 ITTF geo g (tpc only) |
| `pp2006a` | B2006a,IAna,fcf,ppOpt,VFPPVnoCTB,beamline,emcDY2,ftpc,trgd,ZDCvtx,Corr3 | `` | Production chain for 2005 pp data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2006b` | B2006b,IAna,fcf,ppOpt,VFPPVnoCTB,beamline,emcDY2,ftpc,trgd,ZDCvtx,Corr4,BeamBack | `` | Production chain for 2005 pp data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2006g` | B2006g,IAna,fcf,ppOpt,VFPPVnoCTB,beamline,emcDY2,ftpc,trgd,ZDCvtx,Corr4,BeamBack | `` | Production chain for 2005 pp data geo g (+ l3, tof, ftpc, e/b-emc, trgd) |
| `T2007` | ry2007g,MakeEvent,in,tpc_daq,tpcI,fcf,Tree,evout | `` | TPC only chain,  2007 ITTF |
| `B2007` | ry2007,MakeEvent,in,tpc_daq,tpcI,fcf,svt_daq,SvtD,ssddat,sptd,Idst,tags,Tree,evout | `` | Base chain for 2007 ITTF (tpc+svt+ssd) |
| `B2007g` | ry2007g,MakeEvent,in,tpc_daq,tpcI,fcf,svt_daq,SvtD,ssddat,sptd,Idst,tags,Tree,evout | `` | Base chain for 2007 ITTF geo g (tpc+svt+ssd) |
| `P2007` | B2007,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,ssdIT,Corr5 | `` | Production chain for 2007 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2007g` | B2007g,IAna,VFMinuit2,emcDY2,ftpc,trgd,ZDCvtx,svtIT,ssdIT,Corr5 | `` | Production chain for 2007 data, revised 2008 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2007a` | B2007,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,ssdIT,Corr3 | `` | Production chain for 2007 data Corr3 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2007b` | B2007,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,svtIT,ssdIT,Corr4 | `` | Production chain for 2007 data Corr4 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `B2008` | ry2008,in,tpc_daq,tpcI,fcf,Idst,tags,Tree,evout | `` | Base chain for 2008 ITTF (tpc) |
| `B2008a` | ry2008,in,tpcX,ToFx,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2008 ITTF (tpc+tof) |
| `P2008a` | B2008,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr3,analysis | `` | Production chain for 2008 data Corr3 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2008b` | B2008,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data Corr4 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2008a` | B2008,IAna,ppOpt,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data Corr3 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2008c` | B2008,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2008c` | B2008,IAna,ppOpt,Minuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data Corr4 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `pp2008` | B2008a,IAna,ppOpt,VFPPV,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data Corr3 (+ l3, tof, ftpc, e/b-emc, trgd) |
| `P2008` | B2008a,IAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,Corr4,analysis | `` | Production chain for 2008 data (+ l3, tof, ftpc, e/b-emc, trgd) |
| `B2009.1` | ry2009,in,tpcX,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2009 ITTF (tpc) |
| `B2009.2` | ry2009a,in,tpcX,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2009 ITTF (tpc) |
| `B2009.3` | ry2009d,in,tpcX,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2009 ITTF (tpc) |
| `pp2009a` | B2009.1,IAna,ppOpt,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2009 data - no Corr (+ l3, ftpc, e/b-emc, trgd) |
| `pp2009b` | B2009.1,IAna,ppOpt,VFMinuit,emcDY2,ftpc,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2009 data - no Corr (+ l3, ftpc, e/b-emc, no trigger) |
| `pp2009c` | B2009.2,BAna,ppOpt,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2009 data - no Corr, no VF (+l3, ftpc, e/b-emc, trig) |
| `pp2009d` | B2009.3,BAna,ppOpt,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2009 data - no Corr, no VF (+l3, ftpc, e/b-emc, trig) |
| `B2010` | ry2010,in,tpcX,ITTF,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2010 ITTF (tpc) |
| `B2010c` | ry2010c,in,tpcX,ITTF,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2010 ITTF (tpc) |
| `P2010a` | B2010,BAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2010 data - no Corr (+ l3, ftpc, e/b-emc) |
| `pp2010a` | B2010,BAna,ppOpt,emcDY2,trgd,ftpc,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2010 data - no Corr (+ l3, ftpc, e/b-emc, no VF) |
| `P2010c` | B2010c,BAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2010 data - no Corr (+ l3, ftpc, e/b-emc) |
| `pp2010c` | B2010c,BAna,ppOpt,emcDY2,trgd,ftpc,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2010 data - no Corr (+ l3, ftpc, e/b-emc, no VF) |
| `B2011` | ry2011,in,tpcX,ITTF,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2011 ITTF (tpc) |
| `P2011a` | B2011,BAna,VFMinuit,emcDY2,ftpc,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2011 data - no Corr (+ l3, ftpc, e/b-emc) |
| `pp2011a` | B2011,BAna,ppOpt,emcDY2,trgd,ftpc,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2011 data - no Corr (+ l3, ftpc, e/b-emc, no VF) |
| `B2012` | ry2012,in,tpcX,ITTF,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2012 ITTF (tpc) |
| `pp2012a` | B2012,BAna,ppOpt,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2012 data - no Corr (+ l3, e/b-emc, no VF) |
| `P2012a` | B2012,BAna,VFMinuit,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2011 data - no Corr (+ l3, e/b-emc) |
| `B2012b` | ry2012a,in,tpcX,ITTF,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2012 ITTF (tpc) |
| `pp2012b` | B2012b,BAna,ppOpt,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2012 data - no Corr (+ l3, e/b-emc, no VF) |
| `P2012b` | B2012b,BAna,VFMinuit,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2011 data - no Corr (+ l3, e/b-emc) |
| `B2013` | in,tpcX,UseXgeom,ITTF,NosvtIT,NossdIT,tpcDB,TpcHitMover,Idst,tags,Tree,evout,btof,mtd,BAna,emcDY2,trgd,ZDCvtx | `` | Base chain for 2013 ITTF (tpc) |
| `B2013x` | ry2013_1x,B2013 | `` | Base chain for 2013 ITTF (tpc) |
| `pp2013x` | B2013x,BAna,emcDY2,trgd,ZDCvtx | `` | Production chain for 2013 data - no Corr (+ l3, e/b-emc, no VF) |
| `B2013` | B2013_c2 | `` | Alias for B2013 Configuration 2 |
| `B2013_c2` | ry2013_2c,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2013 ITTF (tpc) |
| `B2013_c1` | ry2013_1c,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2013 ITTF (tpc) |
| `pp2013a` | B2013_c2,ITTF,UseXgeom,BAna,ppOpt,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2013 data - no Corr (+ l3, e/b-emc, no VF) |
| `pp2013b` | B2013_c1,ITTF,UseXgeom,BAna,ppOpt,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2013 data - no Corr (+ l3, e/b-emc, no VF) |
| `pp2013` | BAna,ppOpt,emcDY2,trgd,ZDCvtx,NosvtIT,NossdIT,analysis | `` | Production chain for 2013 data - no Corr (+ l3, e/b-emc, no VF) |
| `B2014` | ry2014,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2014 ITTF (tpc) |
| `B2014a` | ry2014a,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2014a ITTF (tpc) |
| `P2014` | B2014,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,analysis | `` | Production chain for 2014 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2014a` | B2014a,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis, | `` | Production chain for 2014 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2015` | ry2015,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2015 ITTF (tpc) |
| `B2015a` | ry2015a,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for run 2015a data |
| `B2015b` | ry2015b,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for run 2015 with y2015b geometry |
| `B2015c` | ry2015c,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for run 2015 with y2015c geometry |
| `pp2015` | B2015,ITTF,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for 2015 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `pp2015a` | B2015a,ITTF,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015a data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `pp2015b` | B2015b,ITTF,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015b data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `pp2015c` | B2015c,ITTF,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015c data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2015` | B2015,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for 2015 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2015a` | B2015a,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015a data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2015b` | B2015b,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015b data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2015c` | B2015c,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for year 2015c data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2016` | ry2016,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for 2016 ITTF (tpc) |
| `P2016` | B2016,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for 2016 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2016a` | ry2016a,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Production chain for 2016 data (tpc) |
| `P2016a` | B2016a,ITTF,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,StiHftC,analysis | `` | Production chain for 2016 data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2017` | ry2017,in,tpcX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,evout | `` | Base chain for run 2017 data (tpc) |
| `pp2017` | B2017,Sti,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline,l3onl,emcDY2,fpd,trgd,ZDCvtx,analysis | `` | Base chain for year 2017 pp data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2017` | B2017,Sti,UseXgeom,BAna,VFMinuit,l3onl,emcDY2,fpd,trgd,ZDCvtx,analysis | `` | Base chain for year 2017 AA data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2017a` | ry2017a,in,tpcX,CorrX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite | `` | Production chain for run 2017 data (tpc) |
| `pp2017a` | B2017a,ITTF,UseXgeom,BAna,ppOpt,VFPPVnoCTB,beamline3D,l3onl,emcDY2,fpd,trgd,ZDCvtx,analysis | `` | Production chain for year 2017 pp data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `P2017a` | B2017a,ITTF,UseXgeom,BAna,VFMinuit,beamline3D,l3onl,emcDY2,fpd,trgd,ZDCvtx,analysis | `` | Production chain for year 2017 AA data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2018a` | ry2018a,in,tpcX,CorrX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite | `` | Base chain for run 2018 data (tpc) |
| `P2018a` | B2018a,ITTF,UseXgeom,BAna,VFMinuit,beamline3D,l3onl,emcDY2,epdHit,fpd,trgd,ZDCvtx,analysis | `` | Base chain for year 2018 AA data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2018c` | ry2018c,in,tpcX,UseXgeom,CorrX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2018 data (tpc) |
| `P2018c` | B2018c,ITTF,BAna,VFMinuit,beamline3D,btof,mtd,l3onl,emcDY2,epdHit,fpd,trgd,ZDCvtx,analysis | `` | Base chain for year 2018 AA data - no Corr (+ l3, bcc/fpd, e/b-emc) |
| `B2019a` | ry2019a,in,tpcX,UseXgeom,iTpcIT,CorrX,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite | `` | Base chain for run 2019 data (tpc) |
| `P2019a` | B2019a,ITTF,BAna,iTpcIT,VFMinuit,beamline3D,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2019 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2019b` | ry2019b,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2019 data (tpc) |
| `P2019b` | B2019b,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2019 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2020a` | ry2020a,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2020 data (tpc) |
| `P2020a` | B2020a,ITTF,BAna,iTpcIT,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2020 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2020b` | ry2020b,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2020 data (tpc) |
| `P2020b` | B2020b,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2020 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2021a` | ry2021a,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2021 data (tpc) |
| `P2021a` | B2021a,ITTF,BAna,iTpcIT,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2021 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2021b` | ry2021b,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2021 data (tpc) |
| `P2021b` | B2021b,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2021 AA data - no Corr (+ l3, epd, mtd, b/etof, b-emc) |
| `B2022` | ry2022,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2022 data (tpc) |
| `pp2022` | B2022,ITTF,BAna,ppOpt,ImpBToFt0Mode,VFPPVnoCTB,beamline3D,l3onl,etofa,btof,mtd,emcDY2,FttDat,fcs,trgd,ZDCvtx,analysis | `` | Production chain for year 2022 pp data - no Corr (+ l3, epd, mtf, b/etof, fcs, e/b-emc) |
| `B2022a` | ry2022a,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2022 data (tpc) |
| `pp2022a` | B2022a,ITTF,BAna,ppOpt,ImpBToFt0Mode,VFPPVnoCTB,beamline3D,l3onl,etofa,btof,mtd,emcDY2,FttDat,fcs,trgd,ZDCvtx,analysis | `` | Production chain for year 2022 pp data, using CorrY (+ l3, epd, mtf, b/etof, fcs, e/b-emc) |
| `B2023a` | ry2023a,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2023 data (tpc) |
| `P2023a` | B2023a,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2023 AA data, using CorrY (+ l3, epd, mtd, b/etof, b-emc) |
| `B2024a` | ry2024a,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2024 data (tpc) |
| `pp2024a` | B2024a,ITTF,BAna,ppOpt,ImpBToFt0Mode,VFPPVnoCTB,beamline3D,l3onl,epdhit,btof,mtd,emcDY2,ftt,fcs,trgd,ZDCvtx,analysis | `` | Production chain for year 2024 pp data, using CorrY (+ l3, epd, mtd, btof, fcs, ftt, e/b-emc) |
| `P2024a` | B2024a,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2024 AA data, using CorrY (+ l3, epd, mtd, b/etof, b-emc) |
| `B2025` | ry2025,in,tpcX,UseXgeom,iTpcIT,CorrY,AgML,tpcDB,TpcHitMover,Idst,tags,Tree,picoWrite,picoVtxDefault,picoCovMtxWrite | `` | Base chain for run 2025 data (tpc) |
| `P2025` | B2025,ITTF,BAna,VFMinuit,etofa,btof,mtd,l3onl,emcDY2,epdHit,trgd,ZDCvtx,analysis | `` | Base chain for year 2025 AA data, using CorrY (+ l3, epd, mtd, b/etof, b-emc) |
| `LaserCal0` | db,detDb,tpc_daq,tpcDb,tcl,globT,laser,LaserTest | `` | Laser Calibration Chain (tcl) |
| `LaserCal` | db,detDb,tpc_daq,tpcDb,fcf,globT,laser,LaserTest | `` | Laser Calibration Chain |
| `L3Counter` | db,detDb,in,l3count | `` | L3 Counter extraction pass |
| `VtxSeedCal` | ppOpt,ry2001,in,tpc_daq,tpc,globT,-Tree,Physics,-PreVtx,FindVtxSeed,NoEvent,Corr2 | `` | Pass0 Vertex evaluator |
| `SpcChgCal` | B2004,fcf,Corr3,OSpaceZ2,OShortR,SCEbyE,-Tree,-tags,-EvOut,-EventQA | `` | Pass0 SpaceCharge evaluator |
| `SpcChgCalG` | MuDST,fcf,Corr4,OSpaceZ2,OGridLeak3D,SCEbyE,-Tree,-tags,-EvOut,-EventQA | `` | Pass0 SpaceCharge evaluator with GridLeak, no geo or tracker dependence |
| `VtxSeedCalG` | MuDST,fcf,Corr4,FindEvtVtxSeed,-Tree,-tags,-EvOut,-EventQA | `` | Pass0 Vertex evaluator |

</details>

<details>
<summary><b>OPTIONS</b> (109 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `ITTF` |  | `` | request to use one of Sti(default), StiCA, Stv,StvCA, ... trackers |
| `NoHits` |  | `` | Don\'t write hits into Event.Branch |
| `Kalman` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Eval` |  | `` | Turn on evaluation switch for different makers |
| `Ev03` |  | `` | Turn on alternative V0 method |
| `off` |  | `` | Turn off default chain |
| `clearDAQCTB` |  | `` | clear DAQ CTB Hits for embedding |
| `NoInput` |  | `` | No input file |
| `util` |  | `` | Load StAnalysisUtilities |
| `StUtilities` |  | `` | Load StUtilities |
| `FieldOn` | MagF | `` | Constant nominal field |
| `FieldOff` | MagF | `` | No Field option |
| `HalfField` | MagF | `` | Half Field option |
| `ReverseField` | MagF | `` | Reverse Field option |
| `NoCintDb` |  | `` | Switch off all Cint Db |
| `NoStarCintDb` |  | `` | Switch off standard Cint Db |
| `NoLocalCintDb` |  | `` | Switch off local Cint Db |
| `NoMySQLDb` |  | `` | Switch off MySQL Db |
| `NoCintCalDb` | NoLocalCintDb | `` | Switch off local Cint Db |
| `dbSnapshot` |  | `` | Create?use dbSnapshot |
| `NoEvent` | -event,-analysis | `` | Switch Off StEvent and StAnalysis Makers |
| `MakeDoc` |  | `` | Make HTML documentation for the given Chain |
| `Debug` |  | `` | Set debug flag = 1 |
| `Debug1` |  | `` | Set debug flag = 1 |
| `Debug2` |  | `` | Set debug flag = 2 |
| `IdTruth` |  | `` | Enable IdTruth association in StAssociationMaker |
| `OldMiniMc` |  | `` | Keep pre-2008 convention for MiniMc |
| `useInTracker` |  | `` | switch from EGR to ITTF global tracks in StAssociationMaker |
| `noRepeat` |  | `` | No repeat in Messenger |
| `noHistos` |  | `` | Disables Attributes histos |
| `noRunco` |  | `` | Disables Attributes runco |
| `Higz` |  | `` | Pop Higz window |
| `big` |  | `` | Set NwGEANT =20Mwords |
| `bigbig` |  | `` | Set NwGEANT =40Mwords |
| `huge` |  | `` | Set NwGEANT =80Mwords |
| `clearmem` |  | `` | Obsolete |
| `adcOnly` |  | `` | DAQMaker selects only TPC ADC events |
| `InTree` | in | `` | bfcTree Input Tree name |
| `OutTree` | Tree | `` | bfcTree Output Tree name |
| `DstOut` |  | `` | WARNING *** Option is OBSOLETE *** |
| `McEvOut` | StMcEvent,Tree | `` | Write StMcEvent to StTree |
| `EvOut` | Tree | `` | Write StEvent to StTree |
| `GeantOut` | Tree | `` | Write g2t tables to StTree |
| `Simu` |  | `` | Simulated Data, force to use Db time stamp from used geometry |
| `HitsBranch` |  | `` | take out points from dst branch and put them into HitsBranch |
| `paw` |  | `` | Allocate memory for pawc |
| `AllEvent` | Tree | `` | Write whole event to StTree |
| `AllTables` |  | `` | Load Star Tables |
| `FXT` | btofFXT | `` | enable anything FXT-specific |
| `btofFXT` |  | `` | enable FXT mode for btof |
| `Corr1` | AlignSectors,ExB,OBmap,OClock,OPr13 | `` | ... AlignSectors,ExB,OBmap,OClock,OPr13 ... |
| `Corr2` | Corr1,OTwist,OIFC | `` | ...Corr1+OTwist,OIFC ... |
| `Corr3` | AlignSectors,ExB,OBmap2D,OClock,OPr13,OTwist,OIFC | `` | ... AlignSectors,ExB,OBmap2D,OClock,OPr13,OTwist,OIFC ... |
| `Corr4` | Corr3,OShortR | `` | ... Corr3+OShortR... |
| `Corr5` | Corr4,SCEbyE,OGridLeak3D,OSpaceZ2 | `` | ... Corr4+SCEbyE,OGridLeak3D,OSpaceZ2... |
| `CorrX` | ExB,OShortR,OBmap,OPr13,OIFC,OSectorAlign,NewTpcAlignment,-AlignSectors,-OBmap2D,-OClock,-OTwist | `` | New Tpc Alignment |
| `CorrY` | ExB,OShortR,OBmap,OPr40,OIFC,OSectorAlign,NewTpcAlignment,OSpaceZ2,OGridLeakFull,-AlignSectors,-OBmap2D,-OClock,-OTwist | `` | New Tpc Alignment |
| `ExB` |  | `` | Activate ExB correction |
| `EB1` |  | `` | Force ExB configuration 1 |
| `EB2` |  | `` | Force ExB configuration 2 |
| `OBmap` |  | `` | ExB shape correction |
| `OBmap2D` |  | `` | ExB 2 D shape correction |
| `OTwist` |  | `` | ExB twist correction |
| `OClock` |  | `` | Clock/tpc rot. correction |
| `OPr13` |  | `` | PadRow 13 distortion |
| `OPr40` |  | `` | PadRow 40 distortion |
| `OCentm` |  | `` | Central membrane correction |
| `OECap` |  | `` | EndCap (curved) correction |
| `OIFC` |  | `` | Field Cage correction |
| `OSpaceZ` |  | `` | Space Charge corrections |
| `OSpaceZ2` |  | `` | Space Charge corrections R2 |
| `OShortR` |  | `` | Shorted Ring correction |
| `OGridLeak` |  | `` | Grid Leak correction |
| `OGridLeak3D` |  | `` | 3D Grid Leak correction |
| `OGridLeakFull` |  | `` | Full Grid Leak correction |
| `OGGVoltErr` |  | `` | GG voltage error correction |
| `OSectorAlign` |  | `` | Sector alignment distortion correction |
| `ODistoSmear` |  | `` | Distortion smearing accounting for calibration resolutions |
| `OAbortGap` |  | `` | Abort gap cleaning event distortion correction |
| `AlignSectors` |  | `` | WARNING *** Option is OBSOLETE *** |
| `EbyET0` |  | `` | Event-by-event T0 corrections |
| `DbRichSca` | detdb | `` | Force reading of Rich scalers from DB |
| `EastOff` |  | `` | Disactivate East part of tpc |
| `WestOff` |  | `` | Disactivate West part of tpc |
| `AllOn` |  | `` | Activate both East and West parts of tpc |
| `ReadAll` |  | `` | Activate all branches to read |
| `pp` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppOpt` | TrsPileUp | `` | pp option without enabling special cases |
| `TrsPileUp` |  | `` | Trs pile up mode |
| `TrsToF` |  | `` | Trs account for particle time of flight |
| `SvtMatchVtx` |  | `` | WARNING *** Option is OBSOLETE *** |
| `VtxOffSet` |  | `` | Account Primary Vertex offset from y2000 data |
| `Calibration` |  | `` | Calibration mode |
| `beamLine` |  | `` | LMV Beam line constrain |
| `beamLine3D` |  | `` | Use beamline in 3D vertex fit |
| `min2trkVtx` |  | `` | ...only 2 tracks needed for vertex finding |
| `hitreuseon` |  | `` | ...do re-use hits for other tracks in Sti |
| `usePct4Vtx` |  | `` | Use Post-Crossing Tracks for vertex finding |
| `useBTOF4Vtx` |  | `` | Use BTOF track matching for vertex ranking |
| `useBTOFmatchOnly` | useBTOF4Vtx | `` | Use BTOF track matching only |
| `svt1hit` |  | `` | Use 1 SVT hit only combination |
| `onlcl` |  | `` | Read/use TPC DAQ100 clusters |
| `onlraw` |  | `` | Read/use TPC raw hits |
| `ezTree` |  | `` | Create ezTree branch |
| `BEmcDebug` |  | `` | Turn OFF B-EMC hit reconstruction cuts |
| `BEmcChkStat` |  | `` | Turn ON status checking in raw data |
| `useLDV` |  | `` | ... uses laserDV database flavor |
| `useCDV` |  | `` | ... uses ofl database flavor |
| `useNewLDV` |  | `` | ... uses ofl database flavor |

</details>

<details>
<summary><b>Tables</b> (17 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `tables` | StDbT,ebyeT,emc_T,ftpcT,gen_T,geomT,globT,l3_T,sim_T,svt_T,tpc_T | `` |  |
| `StDbT` |  | `` | Load StDb_Tables |
| `ctf_T` |  | `` | Load ctf_Tables |
| `ebyeT` |  | `` | Load ebye_Tables |
| `emc_T` |  | `` | Load emc_Tables |
| `ftpcT` |  | `` | Load ftpc_Tables |
| `gen_T` |  | `` | Load gen_Tables |
| `geomT` |  | `` | Load geometry_Tables |
| `globT` |  | `` | Load global_Tables |
| `l3_T` |  | `` | Load l3_Tables |
| `mwc_T` |  | `` | WARNING *** Option is OBSOLETE *** |
| `sim_T` |  | `` | Load sim_Tables |
| `svt_T` |  | `` | Load svt_Tables |
| `tpc_T` |  | `` | WARNING *** Option is OBSOLETE *** |
| `trg_T` |  | `` | WARNING *** Option is OBSOLETE *** |
| `vpd_T` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Embedding` | -Simu | `` | Embedding run |

</details>

<details>
<summary><b>Geometry+Mag</b> (51 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `pgf77` |  | `` | Fortran |
| `rootcern` | geant3 | `` | ROOT minicern OBSOLETE |
| `StarMiniCern` | geant3 | `` | STAR addition to minicern OBSOLETE |
| `mysql` |  | `` | MySQL |
| `libPhysics` |  | `` | TVector |
| `geant3vmc` | -usexgeom,-xgeometry | `` | VMC geant3 |
| `geant3` | geant3vmc | `` | VMC geant3 plus ROOT EG,pythia6 |
| `geometry` |  | `` | geometry+Mag.Field |
| `StarMagField` | magF | `` | Load StarMagField |
| `geomNoField` | -geometry,StarMagField | `` | geometry-Mag.Field |
| `xgeometry` | -geometry,-geomNoField | `` | AgML geometry-Mag.Field |
| `UseProjectedVertex` |  | `` | Run StBTofCalibMaker w/wo Primary Vertex |
| `ImpBToFt0Mode` |  | `` | pppAMode in StVpdCalibMaker and StBTofCalibMaker |
| `setOutlierRej4BToft0` |  | `` | setPPPAOutlierRej is set in StBTofCalibMaker |
| `vpd` |  | `` | WARNING *** Option is OBSOLETE *** |
| `tls` |  | `` | WARNING *** Option is OBSOLETE *** |
| `rts` |  | `` | load libRTS |
| `daq` | rts | `` | Load StDAQMaker |
| `SCL` |  | `` | Load StarClassLibrary |
| `SvtCL` |  | `` |  |
| `TbUtil` | sim_T,tpc_t,globT,SCL | `` | Load StTableUtilities |
| `TofUtil` |  | `` | Load StTofUtil |
| `BTofUtil` |  | `` | Load StBTofUtil |
| `ETofUtil` |  | `` | Load StETofUtil |
| `MtdUtil` |  | `` | Load StMtdUtil |
| `StBichsel` |  | `` | Load Bichsel model for dE/dx |
| `StEvent` | globT,SCL,TRGDef,StBichsel,EmcUtil,TbUtil,detDb | `` | Load StEvent |
| `PxlUtil` |  | `` | Load StPxlUtil |
| `IstUtil` |  | `` | Load StIstUtil |
| `FstUtil` |  | `` | Load StFstUtil |
| `SsdUtil` | StarMagField,StEvent | `` | Load SSD Util |
| `SstUtil` | StarMagField,StEvent | `` | Load SST Util |
| `EmcUtil` | emc_T,geomT,StDbT | `` | Load StEmcUtil |
| `EEmcUtil` |  | `` | Load StEEmcUtil |
| `FmsUtil` |  | `` | Load StFmsUtil |
| `RhicfUtil` |  | `` | Load StRHICfUtil |
| `FgtUtil` |  | `` | Load StFgtUtil |
| `GmtUtil` |  | `` | Load StGmtUtil |
| `l3Util` |  | `` | WARNING *** Option is OBSOLETE *** |
| `PmdUtil` |  | `` | Load StPmdUtil |
| `QUtils` | PmdUtil,EmcUtil | `` | Load QA Libs dependencies |
| `Stu` |  | `` | Load StEventUtilities |
| `MuDSTDeps` | StEvent,Stu,StBichsel | `` | Load MuDST misc. dependencies |
| `MuDST` | MuDSTDeps,EmcUtil,TofUtil,BTofUtil,PmdUtil | `` | Load MuDST library |
| `MuDSTDeps` | StEvent,Stu,StBichsel | `` | Load MuDST misc. dependencies (all) |
| `MuDST` | MuDSTDeps,EmcUtil,TofUtil,BTofUtil,PmdUtil | `` | Load MuDST library |
| `picoEvt` | StEvent,Stu | `` | Load picoEvent and dependencies |
| `picoDst` | picoEvt,EmcUtil,TofUtil,BTofUtil,PmdUtil | `` | Load PicoDST library |
| `geantL` | geomT,gen_T,sim_T,StarMagField | `` | Load GeantLibs |
| `gstarLib` |  | `` | Load gstar lib |
| `flux` | simu | `` | Load flux lib |

</details>

<details>
<summary><b>Generators</b> (6 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `stargen` | gen_T,sim_T | `` | STAR Generator BASE |
| `pythia8.1.86` | stargen | `` | Load Pythia 8.1.86 generator |
| `pythia8.2.35` | stargen | `` | Load Pythia 8.1.86 generator |
| `hijing1.383` | stargen | `` | Load Hijing  1.383 generator |
| `kinematics` | stargen | `` | Load STAR Particle Gun |
| `genreader` | stargen | `` | Load STAR Gen Event Reader |

</details>

<details>
<summary><b>GEANT4 Libs</b> (4 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `g4interfaces` |  | `` | Load G4 libs |
| `g4physics` |  | `` | Load G4 |
| `g4geant3` |  | `` | Load g3 to g4 support |
| `geant4` | g4physics,g4interfaces | `` | Load G4 libs |

</details>

<details>
<summary><b>I/O Makers</b> (9 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `gstar` | -fzin,-ntin,-geant,Simu,geantL | `St_geant_Maker` | gstar for 80 muon tracks with pT = 1GeV in \|eta\|<4 |
| `pythia` | -gstar,-fzin,-ntin,-geant,geantL | `St_geant_Maker` | Load Pythia in starsim, set pp 510 GeV |
| `Wenu` | pythia | `` | set pp 510 GeV -> W+/- -> e+/- nu/nubar |
| `hijing` | -gstar,-fzin,-ntin,-geant,geantL | `St_geant_Maker` | Load Hijing in starsim, set AuAu200 GeV |
| `tdaq` |  | `` | WARNING *** Option is OBSOLETE *** |
| `miniDAQ` |  | `` | WARNING *** Option is OBSOLETE *** |
| `fzin` | Simu,-gstar,-ntin,-geant,geantL | `St_geant_Maker` | read gstar fz-file |
| `UseXgeom` | -geometry,-geomNoField,xgeometry | `` | suppress mortran version of geometry |
| `in` |  | `StIOMaker` | Read [DAQ\|ROOT] input file |

</details>

<details>
<summary><b>Db makers</b> (22 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `db` | StDbT | `St_db_Maker` |  |
| `magF` | StDbT,db,detDb | `StMagFMaker` | Mag.field map with scale factor from Db |
| `detDb` | db | `StDetectorDbMaker` | Load StDetectorDbMaker library |
| `mtin` | -fzin,-geant,-gstar,geantL,gstarLib,-magF | `St_geant_Maker` | read event from MuDst |
| `tpcDB` | tpc_T,dbutil,detDb,StarMagField,magF,StEvent | `StTpcDbMaker` |  |
| `dbutil` | detDb,StDbT | `` | Load StDbUtilities |
| `svtDb` | tpcDb,SvtCL | `StSvtDbMaker` | Load and run SvtDbMaker |
| `ssdDb` | tpcDb,SsdUtil | `StSsdDbMaker` | Load and run SsdDbMaker |
| `sstDb` | tpcDb,SstUtil | `StSstDbMaker` | Load and run SstDbMaker |
| `svtCalDb` | svtDb | `` | Declare Calibrations/svt as while list member |
| `ssdCalDb` | ssdDb | `` | Declare Calibrations/ssd as while list member |
| `sstCalDb` | sstDb | `` | Declare Calibrations/sst as while list member |
| `eemcDb` | db,EEmcUtil | `StEEmcDbMaker` | Load EEmcDbMaker |
| `fmsDb` | db,fmsutil | `StFmsDbMaker` | Load FmsDbMaker |
| `rhicfDb` | db | `StRHICfDbMaker` | Load RHICfDbMaker |
| `fcsDb` |  | `StFcsDbMaker` | Load FcsDbMaker |
| `fttDb` |  | `StFttDbMaker` | Load FttDbMaker |
| `fgtDb` | db,fgtutil | `StFgtDbMaker` | Load FgtDbMaker |
| `pxlDb` | tpcDb PxlUtil | `StPxlDbMaker` | Load PxlDbMaker |
| `istDb` | tpcDb | `StIstDbMaker` | Load IstDbMaker |
| `fstDb` | tpcDb | `StFstDbMaker` | Load FstDbMaker |
| `epdDb` | tpcDb | `StEpdDbMaker` | Load EpdDbMaker |

</details>

<details>
<summary><b>MAKERS</b> (381 options)</summary>


| Key | Opts (dependencies) | Maker | Comment |
|-----|---------------------|-------|---------|
| `ntin` | paw,-fzin,-geant,-gstar,Simu,geantL,gstarLib | `St_geant_Maker` | read event generated Hbook nt-file |
| `PrepEmbed` | geantEmb | `StPrepEmbedMaker` | Prepare kinematics for embedding |
| `PythiaEmbed` | pythia,-Simu | `` | Prepare kinematics for embedding |
| `geant` | geantL | `St_geant_Maker` | passive GEANT |
| `geantEmb` | geantL | `St_geant_Maker` | GEANT embedding mode |
| `RootVMC` | -geant,-fzin,-ntin,StarMagField,-geantL,-geometry,-geomNoField,geant3 | `` |  |
| `VMCAppl` | geomT,gen_t,sim_T,tpcDB,RootVMC, | `` | VMC G3 |
| `VMC` | Simu,VMCAppl,-geant | `StVMCMaker` | VMC Maker |
| `VMCPassive` | VMCAppl | `StVMCMaker` | VMC Maker in Passive Mode |
| `trg` | trg_T,globT,db | `St_trg_Maker` | trigger analysis for Year 2001-2005 data |
| `TRGDef` |  | `` | Load StTriggerData |
| `trgd` | TRGDef | `StTriggerDataMaker` | Get trigger data |
| `MakeEvent` | StEvent,detDb | `StEventMaker` | <Early StEvent creation> |
| `LaserAvEv` |  | `StLaserAvEventMaker` | Hejrad |
| `LaserAvCl` |  | `StLaserAvClusterMaker` |  |
| `l0` | globT,ctf,trg | `StMaker` |  |
| `ctf` | ctf_T,db | `St_ctf_Maker` | ToF simulation |
| `mwc` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppMCTrig` |  | `` | WARNING *** Option is OBSOLETE *** |
| `pp2pp` |  | `St_pp2pp_Maker` | pp->pp |
| `mtdFilt` |  | `StMtdEvtFilterApplyMaker` | MTD-based event filtering |
| `mtd` | mtdDat,mtdMatch | `StMaker` | MTD Chain |
| `mtdDat` | db,MuDst | `StMtdHitMaker` | MTD hit maker |
| `mtdTrkMask` | db | `StMtdTrackingMaskMaker` | MTD track masking |
| `gmt` | gmtDat,gmtClu | `` | Gmt data Chain |
| `gmtDat` | event | `StGmtRawMaker` | GMT Data reader |
| `gmtClu` | gmtutil | `StGmtClusterMaker` | GMT cluster maker |
| `gmtCosmics` | Cosmics,gmt | `` | Save only events with GMT clusters and Cosmic tracks |
| `gmtClusTree` |  | `` | WARNING *** Option is OBSOLETE *** |
| `epdHit` | epdDb,event | `StEpdHitMaker` | EPD hit maker |
| `tpc` | TpxRaw,TpxClu,tpcI | `` | WARNING *** Option is OBSOLETE *** use TpxClu instead |
| `tpcI` | db,tpcDB,TpcHitMover | `StMaker` | tpc with ITTF |
| `tpcX` | -tpcI,tpx,MakeEvent | `StMaker` | tpc+tpcx with ITTF |
| `Trs` | scl,tpcDB,TrsToF,StEvent,EmbeddingShortCut | `StTrsMaker` |  |
| `TpcRS` | scl,tpcDB,-Trs,-EmbeddingShortCut,Stu | `StTpcRSMaker` | New Tpc Response Simulator |
| `tfs` | TpcFastSim | `` | WARNING *** Option is OBSOLETE *** use TpcFastSim instead |
| `TpcFastSim` | MakeEvent,Simu,-trs,-TpcRS,-tcl,-tpxclu,-fcf,-tpc_daq,StEvent,-TpcHitMover,EmbeddingShortCut | `StTpcFastSimMaker` | use tfs (no Trs or TpcRS) |
| `EmbeddingShortCut` |  | `` | Short Cut for StdEdxY2Maker and StTpcHitMover |
| `StMcEvent` | gen_t,sim_T | `` |  |
| `McEvent` | StEvent,tpcDb,EEmcUtil,EmcUtil,StMcEvent | `StMcEventMaker` |  |
| `Mixer` | daq | `StMixerMaker` |  |
| `St_tpc` |  | `` | WARNING *** Option is OBSOLETE *** |
| `St_svt` |  | `` | WARNING *** Option is OBSOLETE *** |
| `tpc_daq` | TpxRaw | `` | WARNING *** Option is OBSOLETE *** use TpxRaw instead |
| `tcl` | TpxRaw,TpxClu,MakeEvent | `` | WARNING *** Option is OBSOLETE *** use option TpxRaw,TpxClu,MakeEvent instead |
| `fcf` | -tcl,tpcX | `` | WARNING *** Option is OBSOLETE *** use tpcX instead |
| `tpx` | MakeEvent,tpc_T,StEvent,rts,detDb | `StTpcHitMaker` | TPC hit reader for tpc + tpx via EVP_READER |
| `gmt` | gmtDat,gmtClu | `StMaker` | Gmt data Chain |
| `gmtDat` | event | `StGmtRawMaker` | GMT Data reader |
| `gmtClu` | gmtutil | `StGmtClusterMaker` | GMT cluster maker |
| `TpxPulser` | rts,detDb | `StTpcHitMaker` | TPC+TPX pulser analysis |
| `TpxPadMonitor` |  | `` | WARNING *** Option is OBSOLETE *** |
| `TpxAvLaser` | rts,detDb | `StTpcHitMaker` | TPC+TPX averaging laser events into Sparse histogram on pixels level |
| `TpxDumpPxls2Nt` | rts,detDb | `StTpcHitMaker` | TPC+TPX pixel dump to NTuple |
| `TpxRaw` | rts,detDb,StEvent | `StTpcHitMaker` | TPC+TPX convert DAQ Tpc Raw Data to TpcRawEvent used by TpcMixer and/or TpxClu |
| `TpcMixer` | StEvent,rts,-Mixer,-tpx,TpxClu | `StTpcMixerMaker` |  |
| `TpxClu` | rts,tpcDb,detDb,-tpx,-tpc_daq,-fcf | `StTpcRTSHitMaker` | RTS(online) cluster maker |
| `TPC23` | TpxClu | `` | RTS(online, daq5k) tpc23-cluster maker |
| `TpxClu2D` | TpxClu | `` | RTS(online) 2D-cluster maker |
| `TpcAvCluster` | rts,detDb | `StTpcAvClusterMaker` | TPC+TPX averaging laser events into Sparse histogram on cluster level |
| `Velo` | tpc_T | `StVeloMaker` |  |
| `TpcHitMover` | tpcDb,StEvent,Stu | `StTpcHitMover` | TPC hits coord transform + corrections |
| `tpt` |  | `` | WARNING *** Option is OBSOLETE *** use ITTF |
| `tpt_old` |  | `` | WARNING *** Option is OBSOLETE *** use ITTF |
| `TpcT0` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ChargeStep` |  | `` | WARNING *** Option is OBSOLETE *** |
| `laser` |  | `` | WARNING *** Option is OBSOLETE *** |
| `PreVtx` |  | `` | WARNING *** Option is OBSOLETE *** |
| `svt` | svt_T,SvtCL | `StMaker` |  |
| `svt_daq` | daq,SvtCL | `StSvtDaqMaker` |  |
| `sss` | SvtSlowSim | `` | Short cut for SvtSlowSim |
| `SvtSlowSim` | SvtSSim,SvtOnlSeq | `` | Short cut for SvtSlowSim and SvtOnlSeq |
| `SvtSSim` | svtCalDb,SvtCL,Simu,SvtSeqAdj,SvtClu,SvtCluAnal,SvtHit | `StSvtSimulationMaker` |  |
| `SvtEmbed` | SvtSSim,SvtEm,SvtOnlSeq | `` | Short cutfor SvtSlowSim and SvtOnlSeq |
| `SvtEm` | svtCalDb,SvtCL,Simu,SvtSeqAdj,SvtClu,SvtCluAnal,SvtHit | `StSvtEmbeddingMaker` |  |
| `SvtOnlSeq` | svtCalDb,SvtCL,Simu,SvtSeqAdj,SvtClu,SvtCluAnal,SvtHit | `StSvtOnlineSeqAdjSimMaker` |  |
| `srs` | sfs | `` | WARNING *** Option is OBSOLETE use sfs instead *** |
| `sfs` | svtDb,Simu,SvtCL,-sss,-SvtSlowSim,StEvent,MakeEvent | `St_sfs_Maker` | Very fast simulator for SVT |
| `SvtSeqAdj` | SvtCL | `StSvtSeqAdjMaker` |  |
| `SvtClu` | svt_T,StEvent,SvtCL | `StSvtClusterMaker` |  |
| `SvtCluAnal` | SvtCL | `StSvtClusterAnalysisMaker` |  |
| `SvtHit` | SvtCL | `StSvtHitMaker` |  |
| `SvtVtx` |  | `` | WARNING *** Option is OBSOLETE *** |
| `stk` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Est` | SvtIT | `` | WARNING *** Option is OBSOLETE *** use SvtIT instead |
| `global` | ITTF | `` | WARNING *** Option is OBSOLETE use ITTF instead*** |
| `Match` | ITTF | `` | WARNING *** Option is OBSOLETE use ITTF instead*** |
| `Vertex` | ITTF | `` | WARNING *** Option is OBSOLETE use ITTF instead*** |
| `Primary` | ITTF | `` | WARNING *** Option is OBSOLETE use ITTF instead*** |
| `V0` | V02 | `` | WARNING *** Option is OBSOLETE use V02 instead*** |
| `Xi` | Xi2 | `` | WARNING *** Option is OBSOLETE use Xi2 instead*** |
| `Kink` | Kink2 | `` | WARNING *** Option is OBSOLETE use Kink2 instead*** |
| `dst` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Fglobal` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Fprimary` |  | `` | WARNING *** Option is OBSOLETE *** |
| `dEdx` |  | `` | WARNING *** Option is OBSOLETE *** |
| `svtdEdx` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Event` | MakeEvent | `` | Request to initialize event |
| `pxlRaw` | pxlDb | `StPxlRawHitMaker` | PXL raw hit maker |
| `pxlFastSim` | pxlRaw,StMcEvent,StEvent | `StPxlSimMaker` | PXL Fast Simulator |
| `pxlSlowSim` | pxlRaw,StMcEvent,StEvent | `StPxlSimMaker` | PXL Slow Simulator |
| `pxlCluster` | pxlRaw | `StPxlClusterMaker` | PXL cluster maker |
| `pxlHit` | event pxlCluster | `StPxlHitMaker` | PXL hit maker |
| `istSlowSim` | StMcEvent,StEvent,IstUtil | `StIstSlowSimMaker` | IST Slow simulator |
| `istFastSim` | StMcEvent,StEvent | `StIstFastSimMaker` | IST Fast simulator |
| `istRawHit` | istUtil,istDb | `StIstRawHitMaker` | IST RAWhit maker |
| `istCluster` | istRawHit | `StIstClusterMaker` | IST Cluster maker |
| `istHit` | event,istCluster | `StIstHitMaker` | IST Hit Maker |
| `fst` | fstRawHit,fstCluster,fstHit | `StMaker` | FST chain |
| `fstFastSim` | StMcEvent,StEvent | `StFstFastSimMaker` | FST fast simulator |
| `fstRawHit` | FstUtil,fstDb | `StFstRawHitMaker` | FST raw hit maker |
| `fstCluster` | fstRawHit | `StFstClusterMaker` | FST Cluster maker |
| `fstHit` | event,fstCluster | `StFstHitMaker` | FST Hit Maker |
| `fstEvtRawHit` | fstRawHit | `` | Store FST Raw Hit in StEvent |
| `fstMuRawHit` | CMuDST,fstEvtRawHit | `` | Store FST Raw Hit in MuDst |
| `ssddat` | ssd_daq | `` | SSD full chain for Real Data |
| `sstdat` | sst_daq | `` | SST full chain for Real Data |
| `ssd_daq` | ssdCalDb,svt_T,-sls,-spa,ssdUtil | `StSsdDaqMaker` | ... SSD Daq |
| `sst_daq` | sstCalDb,svt_T,-sls,-spa,sstUtil | `StSstDaqMaker` | ... SST Daq |
| `ssdfast` | ssdDb,StMcEvent,StEvent | `StSsdFastSimMaker` | ... SSD fast simulator |
| `ssd` | ssdCalDb,sls,spa,sptd | `` | SSD full chain for simulation |
| `sls` | McEvent,Simu,svt_T,SvtCL | `St_sls_Maker` | ... SSD slow simulator |
| `spa` | Simu,svt_T,SvtCL,ssdUtil | `St_spa_Maker` | ... SSD Pedestal Annihilator |
| `SsdEmbed` |  | `StSsdEmbeddingMaker` | ... SSD Mixing geom Maker |
| `spt` | sptd | `` | Alias for sptd - please replace in chain |
| `sptd` | ssdUtil,svt_T | `StSsdPointMaker` | ... SSD Point Creator |
| `sptt` | sstUtil | `StSstPointMaker` | ... SST Point Creator |
| `sstHit` | sst_daq,sptt | `` | SST reconstruction chain |
| `ssdpre` | ssdEmbed,spa | `` | SSD full chain for pre-embedding |
| `ssdAdd` | ssd_daq | `StSsdAddMaker` | ... SSD merge maker |
| `ssdE` | ssdpre,ssdAdd | `` | SSD full chain for embedding |
| `ZDCVtx` | db | `StZdcVertexMaker` |  |
| `emcDY2` | daq,eemcDb,EEmcUtil,emc_T,EmcUtil,StEvent,PreEcl,Epc | `StEmcRawMaker` | B/E EMC data common maker |
| `eemcD` |  | `` | WARNING *** Option is OBSOLETE *** |
| `emcY2` | emc_T,tpc_T,db,emcSim,PreEcl,epc | `` | EMC Chain for Y2A (must be before makers which include in this chain) |
| `emcSim` | emc_T,EmcUtil,McEvent,MuDST | `StEmcSimulatorMaker` | New simulator for BEMC |
| `EEfs` | eemcDb,EEmcUtil,MuDst | `StEEmcFastMaker` | EEMC fast simulator |
| `EEss` | -eefs,eemcDb,EEmcUtil,MuDst | `StEEmcSlowMaker` | EEMC slow simulator |
| `BEmcMixer` |  | `StEmcMixerMaker` | BEMC mixer |
| `emcAtoE` | db | `StEmcADCtoEMaker` | B-EMC ADC to E converter  OBSOLETE for data in Run 9 or later |
| `PreEcl` |  | `StPreEclMaker` | B-EMC Cluster finder |
| `Epc` | PreEcl,EmcUtil | `StEpcMaker` | B-EMC point maker |
| `EEmcMixer` |  | `StEEmcMixerMaker` | EEMC mixer |
| `eemcA2E` | db | `StEEmcA2EMaker` | E-EMC ADC to E converter |
| `eemCl` | db | `StEEmcClusterMaker` | E-EMC clustering |
| `btof` | btofDat,vpdCalib,btofMatch,btofCalib | `StMaker` | BTOF Chain |
| `BtofDat` | db,BTofutil | `StBTofHitMaker` | BTOF hit maker |
| `vpdSim` | BTofUtil | `StVpdSimMaker` | Vpd Simulator |
| `vpdCalib` | db,BTofUtil | `StVpdCalibMaker` | VPD calibration |
| `btofSim` | BTofUtil | `StBTofSimMaker` | BTOF Simulator |
| `btofMixer` | BTofUtil | `StBTofMixerMaker` | BTof Mixer |
| `ETofDat` | db, ETofUtil | `StETofDigiMaker` | ETOF digi maker |
| `ETofCalib` | db, ETofUtil, muDst | `StETofCalibMaker` | ETOF calibration |
| `ETofSim` |  | `StETofSimMaker` | ETOF simulator |
| `mtdSim` |  | `StMtdSimMaker` | MTD Simulator |
| `ToF` | tofDat,tofrMatch,tofpMatch,tofCalib,geant | `StMaker` | ToF Chain |
| `ToFx` | tofXDat,tofrMatch,tofCalib,geant | `StMaker` | ToFx Chain |
| `tofDat` | db,Tofutil | `StTofMaker` | TOF Data base chain |
| `tofXDat` | db,Tofutil | `StTofHitMaker` | TOF hit maker |
| `tofsim` | TofUtil | `StTofSimMaker` | TOF Simulator, Tof version before y2009 |
| `tofrMatch` | db,TofUtil | `StTofrMatchMaker` | TPC to TOFr track matching |
| `tofpMatch` | db,TofUtil | `StTofpMatchMaker` | TPC to TOFp track matching |
| `tofCalib` | db,TofUtil | `StTofCalibMaker` | TOF calibration |
| `FiltGamma` | StEvent,StMcEvent,EmcUtil | `StGammaFilterMaker` | BEmc Gamma filtering |
| `FiltEemcGamma` | StEvent,StMcEvent,EmcUtil | `StEemcGammaFilterMaker` | EEmc Gamma filtering |
| `rich` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Rrs` |  | `` | WARNING *** Option is OBSOLETE *** |
| `rch` |  | `` | WARNING *** Option is OBSOLETE *** |
| `RichPiD` |  | `` | WARNING *** Option is OBSOLETE *** |
| `l3` |  | `` | WARNING *** Option is OBSOLETE *** |
| `l3cl` |  | `` | WARNING *** Option is OBSOLETE *** |
| `l3t` |  | `` | WARNING *** Option is OBSOLETE *** |
| `l3onl` |  | `Stl3RawReaderMaker` |  |
| `l3count` |  | `Stl3CounterMaker` |  |
| `l3onl` |  | `` | WARNING *** Option is OBSOLETE *** |
| `l3count` |  | `` | WARNING *** Option is OBSOLETE *** |
| `bbcSim` | db | `StBbcSimulationMaker` | BBC Simulator |
| `fmsSim` | StEvent,fmsDb | `StFmsFastSimulatorMaker` | FMS Fast Simulator |
| `fmsDat` | StEvent,fmsDb | `StFmsHitMaker` | Fill FMS struct and zero TRG |
| `fpsDat` | StEvent,fmsdb | `StFpsRawHitMaker` | Fill FPS struct |
| `fmsPoint` | event,fmsDb | `StFmsPointMaker` | Fill FMS clusters and points |
| `fmsfps` | event,fmsDb | `StFmsFpsMaker` | Fill FPS association in FMS points |
| `rhicfDat` | StEvent,RhicfUtil,rhicfdb | `StRHICfRawHitMaker` | Fill RHICf raw data |
| `fcs` | fcsDat,fcsWFF,fcsCluster,fcsPoint | `StMaker` | FCS chain |
| `fcsSim` | StEvent,fcsDb | `StFcsFastSimulatorMaker` | FCS Fast Simulator |
| `fcsDat` | StEvent,fcsDb | `StFcsRawHitMaker` | FCS daq file reader/hit maker |
| `fcsWFF` | StEvent,fcsDb | `StFcsWaveformFitMaker` | Fit FCS waveform |
| `fcsCluster` | StEvent,fcsDb | `StFcsClusterMaker` | Fill FCS clusters |
| `fcsPoint` | StEvent,fcsDb | `StFcsPointMaker` | Fill FCS points |
| `fcsPi0Libs` | MuDst | `` | Libs for FCS Pi0 Finder |
| `ftt` | FttDat,FttHitCalib,FttClu,FttPoint | `StMaker` | FST chain |
| `FttDat` | StEvent | `StFttRawHitMaker` | sTGC Raw hit maker |
| `FttHitCalib` | StEvent,MuDST | `StFttHitCalibMaker` | sTGC hit calib maker |
| `FttClu` | StEvent,fttDb | `StFttClusterMaker` | sTGC Cluster maker |
| `FttPoint` | StEvent,fttDb | `StFttPointMaker` | sTGC Point maker |
| `FttQA` |  | `StFttQAMaker` | sTGC Raw hit QA maker |
| `FwdTrack` | fcsDb | `StFwdTrackMaker` | Forward Track Maker |
| `fpd` |  | `StFpdMaker` | FPD/BBC Data base chain |
| `fpd` |  | `` | WARNING *** Option is OBSOLETE *** |
| `fgt` | fgtDat,fgtClu,fgtAtoC | `StMaker` | Fgt data Chain |
| `fgtDat` | event,fgtdb | `StFgtRawMaker` | FGT Data reader |
| `fgtAtoC` | fgtdb | `StFgtA2CMaker` | FGT ADC to Charge maker |
| `fgtClu` | fgtutil | `StFgtClusterMaker` | FGT cluster maker |
| `fgtAtoC` | fgtdb | `StFgtA2CMaker` | FGT ADC to Charge maker |
| `fgtPoint` | event | `StFgtPointMaker` | Creates FGT points |
| `genvtx` | EEmcUtil | `StGenericVertexMaker` | Generic Vertex Finder library (default is MinuitVertexFinder) |
| `VFMinuit` | genvtx | `` | ... Generic VF will use Minuit method |
| `CtbMatchVtx` | VFMinuit | `` | ... CTB Matching ON in Vertex Finding |
| `VFMinuit2` | VFMinuit | `` | ... Generic VF will use Minuit method w/rank mode 2 |
| `VFMinuit3` | VFMinuit | `` | ... Generic VF will use Minuit method w/rank mode 3 |
| `VFFV` | genvtx | `` | ... Fixed dummy VF method |
| `VFMCE` | genvtx | `` | ... Fixed vertex from MCEvent |
| `VFppLMV` | genvtx | `` | ... VertexMaker will use ppLMV method |
| `VFppLMV5` | VFppLMV | `` | ... VertexMaker will use ppLMV method (tuned) |
| `VFStoreX` |  | `` | ... VertexMaker will use save at least 100 vertices |
| `StiPulls` |  | `` | Request to make Sti Pulls |
| `StiLib` |  | `` | Load Sti |
| `StiCALib` |  | `` | Load Sti+CA |
| `StiTpc` | TpcDb,ITTF,tpc_T,dbutil,detDb,StarMagField,magF | `` | Load StiTpc |
| `StiSvt` |  | `` | Load StiSvt |
| `StiSsd` |  | `` | Load StiSsd |
| `StiSst` |  | `` | Load StiSsd(t) |
| `StiPxl` |  | `` | Load StiPxl |
| `StiIst` |  | `` | Load StiIst |
| `StiLibs` | StiTpc,StiSvt,StiSsd,BTofUtil | `` | Load Sti Detector libs |
| `StiLibsHft` | StiLibs,StiSst,StiPxl,StiIst | `` | Load Sti Detector libs for HFT |
| `StiHftP` | StiLibsHft,-SvtIT,-SsdIT,PxlIT,pxlDb | `` | Enables partial  Hft (PXL) |
| `StiHftC` | StiLibsHft,-SvtIT,-SsdIT,PxlIT,pxlDb,IstIT,istDb,SstIT,sstDb | `` | Enables complete Hft (PXL+IST+SST) |
| `VFPPV` | eemcDb,StiLib,-VFMinuit,-VFMinuit2,-VFMinuit3, | `StGenericVertexMaker` | ... Pile-up proof vertex finder |
| `VFPPVnoCTB` | VFPPV | `` | ... Pile-up proof vertex finder, noCTB |
| `VFPPVEv` | eemcDb | `StGenericVertexMaker` | ... StEvent based VFPPV, noCTB |
| `VFPPVEvNoBtof` | VFPPVEv | `` | ...VFPPVEv with no Btof |
| `Sti` | StiLib,StiLibs,SCL,StEvent,StDbT,TpcIT,compend,sim_T,tbutil | `StiMaker` | Sti tracker |
| `StiCA` | Sti | `StiMaker` | StiCA tracker |
| `StiCAPerf` |  | `` | CA Performance |
| `HLTCA` |  | `StHLTCAMaker` | HLT reconstruction |
| `KFVertex` | -genvtx,-VFMinuit,-VFFV,-VFMCE,-VFppLMV,-VFPPVnoCTB,-VFPPV,-Kink2,-V02,-Xi2 | `StKFVertexMaker` | ...KFParticle based multi vertex reconstruction |
| `StiVMC` | -Sti,SCL,StEvent,StDbT,TpcDb,compend | `StiVMCMaker` | ITTF VMC tracker |
| `StiVMCLibs` | detDb,StarMagField | `` | ITTF:load StiVMC libraries |
| `laserIT` |  | `` | use Sti for laser reconstruction |
| `TpcIT` | ITTF | `` | Sti tracking: TPC |
| `SvtIT` | ITTF | `` | Sti tracking: SVT |
| `SsdIT` | ITTF | `` | Sti tracking: SSD |
| `HpdIT` | ITTF | `` | Sti tracking: Hpd geom |
| `PixelIT` | PxlIT | `` | Alias for PxlIT |
| `PxlIT` | ITTF | `` | Sti tracking: Pixel geom |
| `IstIT` | ITTF | `` | Sti tracking: Ist geom |
| `SstIT` | ITTF | `` | Sti tracking: Sst geom |
| `iTpcIT` | ITTF | `` | Sti tracking: iTpc geom + hits |
| `BTofIT` | ITTF | `` | Sti tracking: BTof geom |
| `NoSvtIT` | -SvtIT | `` | ITTF: track with switch off SVT geom |
| `NoSsdIT` | -SsdIT | `` | ITTF: track with switch off SSD geom |
| `NoSstIT` | -SstIT | `` | ITTF: track with switch off SST geom |
| `NoPxlIT` | -PxlIT | `` | ITTF: track with switch off PXL geom |
| `NoIstIT` | -IstIT | `` | ITTF: track with switch off IST geom |
| `skip1row` |  | `` | ITTF: skip the first pad row in TPC |
| `StiRnD` |  | `` | Load StiRnD shared library |
| `Alignment` |  | `` | Sti Tpc Alignment, reconstruction per sector |
| `Cosmics` |  | `` | Ignore events without recostructed tracks |
| `StiPulls` | Sti | `` | Request to make Sti Pulls |
| `BeamBack` | StEvent | `StBeamBackMaker` | Beam background tracker in the TPC |
| `BeamBack` | StEvent | `` | WARNING *** Option is OBSOLETE *** |
| `dEdxY2` | tpcDb,StEvent | `StdEdxY2Maker` | Bichsel method used for dEdx |
| `CalcdNdx` | dEdxY2 | `` | Option for StdEdxY2Maker to calculate dN/dx |
| `btofMatch` | db,BTofUtil,vpdCalib,btofCalib | `StBTofMatchMaker` | TPC-BTOF track matching |
| `btofCalib` | db,BTofUtil | `StBTofCalibMaker` | BTOF calibration |
| `ETofHit` | db, ETofUtil, muDst | `StETofHitMaker` | ETOF hit maker |
| `ETofMatch` | db, ETofUtil, muDst | `StETofMatchMaker` | ETOF match maker |
| `ETofQa` | db, ETofUtil, muDst | `StETofQAMaker` | ETOF QA maker |
| `ETofA` | etofdat,ETofCalib,etofhit,ETofMatch | `` | ... ETOF chain options for data |
| `mtdMatch` | db,MtdUtil | `StMtdMatchMaker` | TPC-MTD track matching |
| `mtdCalib` | db | `StMtdCalibMaker` | MTD calibration |
| `mtdEvtFilt` | db | `StMtdEvtFilterMaker` | MTD event filter |
| `FindVtxSeed` | globT,MuDSTDeps,picoDst | `StVertexSeedMaker` | Performs vertex seed finding |
| `FindEvtVtxSeed` | MuDSTDeps,picoDst | `StEvtVtxSeedMaker` | Performs vertex seed finding using StEvent |
| `Ftpc` | ftpcT,fcl,fpt | `StMaker` |  |
| `fss` | SCL,Simu | `StFtpcSlowSimMaker` | FTPC Slow simulator |
| `Fcl` | SCL,daq | `StFtpcClusterMaker` | FTPC cluster finder |
| `fpt` | SCL | `StFtpcTrackMaker` | FTPC Track Maker |
| `fgain` | fcl,fpt | `` | StFtpcClusterMaker and StFtpcTrackMaker will produce gain scan histograms |
| `fdbg` | fcl,fpt | `` | StFtpcClusterMaker and StFtpcTrackMaker will write debugfile |
| `flaser` | fcl,fpt | `` | StFtpcClusterMaker and StFtpcTrackMaker for LASERTRACKING |
| `pmdReco` | PmdUtil,pmdClust | `StMaker` | PMD Reco chain |
| `pmdRaw` | pmdReco,pmdRead | `` | PMD Reco chain giving raw data |
| `pmd` | pmdReco,pmdSim,pmdDis | `` | PMD Simulation chain |
| `pmdRead` | PmdUtil | `StPmdReadMaker` | DAQ reader for PMD |
| `pmdSim` | PmdUtil | `StPmdSimulatorMaker` | Hit Simulator for PMD |
| `pmdClust` |  | `StPmdClusterMaker` | ClusterMaker for PMD |
| `pmdDis` | PmdClust | `StPmdDiscriminatorMaker` | Discriminator for PMD |
| `Kink2` | db,MuDST,-kink | `StKinkMaker` | Find Kinks from StEvent |
| `V02` | db,MuDST,-V0 | `StV0FinderMaker` | Find V0s from StEvent |
| `Xi2` | db,MuDST,-V02,-Xi | `StXiFinderMaker` | Xis AND V0s from StEvent |
| `Kink2` |  | `` | WARNING *** Option is OBSOLETE *** |
| `V02` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Xi2` |  | `` | WARNING *** Option is OBSOLETE *** |
| `V0svt` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Xisvt` |  | `` | WARNING *** Option is OBSOLETE *** |
| `SCEbyE` | MuDSTDeps,picoDst | `StSpaceChargeEbyEMaker` | Determine EbyE SpaceCharge using StEvent |
| `SCScalerCal` | MuDSTDeps,picoDst | `StSpaceChargeEbyEMaker` | Calibrate SpaceCharge scalers |
| `compend` | event,detDb | `StEventCompendiumMaker` | Fill event summary in ITTF Chain |
| `shadow` | event,compend | `StShadowMaker` |  |
| `TpcAligner` | Alignment | `StTpcAlignerMaker` | TpcAligner |
| `pec` | Event | `StPeCMaker` | PCollAnalysis |
| `RichSpectra` |  | `` | WARNING *** Option is OBSOLETE *** |
| `TagsChain` |  | `StMaker` |  |
| `TpcTag` |  | `` | WARNING *** Option is OBSOLETE *** |
| `Flow` | , | `` | WARNING *** Option is OBSOLETE *** |
| `FlowTag` |  | `` | WARNING *** Option is OBSOLETE *** |
| `FlowAnalysis` |  | `` |  |
| `SpectraTag` |  | `` | WARNING *** Option is OBSOLETE *** |
| `HeavyTags` | StEVent | `StHeavyTagMaker` |  |
| `EbyeScaTags` |  | `` | WARNING *** Option is OBSOLETE *** |
| `HighPtTags` | StEVent | `StHighPtTagsMaker` |  |
| `PCollTag` |  | `` | WARNING *** Option is OBSOLETE *** |
| `tags` | globT,Event,HeavyTags,HighPtTags | `StTagsMaker` | Collect all tags to TTree |
| `noTags` | -tags,-HeavyTags,-HighPtTags | `` | Turn Off tags |
| `MuDSTChain` |  | `StMaker` |  |
| `StrngMuDST` |  | `StStrangeMuDstMaker` | Creates Stangeness MuDST |
| `StrngMuDST` |  | `` | WARNING *** Option is OBSOLETE *** |
| `EMCmDST` | MuDst | `StEmcMicroDstMaker` | Creates EMC MuDST |
| `CMuDST` | MuDst,StrngMuDST,Tree | `StMuDstMaker` | Writes Common MuDST |
| `CMuDST` | MuDst,Tree | `StMuDstMaker` | Writes Common MuDST |
| `RMuDST` | CMuDST | `` | reads Common MuDST, do not disactivate if no output files |
| `trgSimu` |  | `StTriggerSimuMaker` | trigger simu maker |
| `picoWrite` | trgSimu,picoDst | `StPicoDstMaker` | Writes picoDST format |
| `picoRead` | picoDst | `StPicoDstMaker` | WritesRead picoDST format |
| `PicoVtxDefault` |  | `` | pico Vtx default mode |
| `PicoVtxVpd` | -PicoVtxDefault | `` | pico Vtx cut on Tof and VPD mode |
| `PicoVtxVpdOrDefault` | -PicoVtxDefault | `` | pico Vtx cut on Tof and VPD or default |
| `PicoVtxFXT` | -PicoVtxDefault | `` | pico Vtx constraint on FXT [198,202] mode |
| `PicoVtxMtd` | -PicoVtxDefault | `` | pico Vtx using MTD matching mode |
| `PicoVtxless` | -PicoVtxDefault | `` | pico Vtx NOT required for FWD |
| `PicoCovMtxSkip` |  | `` | Do not write covariance matrices to picoDst (default) |
| `PicoCovMtxWrite` | -PicoCovMtxSkip | `` | Write track covariance matrices to picoDst |
| `PicoBEmcSmdSkip` |  | `` | Do not write BSMD to picoDst (default) |
| `PicoBEmcSmdWrite` | -PicoBEmcSmdSkip | `` | Write BSMD to picoDst |
| `St_geom` |  | `St_geom_Maker` |  |
| `Display` | TbUtil,St_geom | `StEventDisplayMaker` | Event Display |
| `Display` |  | `` | WARNING *** Option is OBSOLETE *** use Ed.C macro instead |
| `Mc` | sim_T,globT,McAss,McAna | `StMaker` |  |
| `McAss` | McEvent | `StAssociationMaker` |  |
| `McAnaTpc` | McAna | `` | Mc Analysis for Tpc |
| `McAnaSvt` | McAna | `` | Mc Analysis for Svt |
| `McAnaSsd` | McAna | `` | Mc Analysis for Ssd |
| `McAna` | McEvent,McAss | `StMcAnalysisMaker` |  |
| `McQa` | McEvent | `StMcQaMaker` | QA histogramms for McEvent |
| `McTpcAna` | tpcDb,dbutil,McEvent,StEvent | `StTpcMcAnalysisMaker` |  |
| `MiniMcEvent` |  | `` | Loads StMiniMcEvent library only |
| `MiniMcMk` | McAss,MiniMcEvent | `StMiniMcMaker` | Creates tree in minimc.root file |
| `SvtMatTree` |  | `SvtMatchedTree` | Create SvtMatchedTree |
| `LAna` | in,detDb,StEvent,tpcDb | `StLaserAnalysisMaker` | Laser data Analysis |
| `gmtAligner` | detDb | `StGmtAlignmentMaker` | GMT cluster plotting |
| `EandBDir` | in,StEvent,TpcHitMover,nodefault | `StEandBDirMaker` | get E&B direction |
| `SpinTag` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppLPfind1` |  | `` | WARNING *** Option is OBSOLETE *** |
| `SpinSortA` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppLPprojectA` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppDAQfilter1` |  | `` | WARNING *** Option is OBSOLETE *** |
| `ppLPeval1` |  | `` | WARNING *** Option is OBSOLETE *** |
| `QA` |  | `` | WARNING *** Option is OBSOLETE *** |
| `EventQA` | QUtils,Event,fcsPi0Libs | `StEventQAMaker` | Filling Y2/Y3 Qa histo |
| `QAC` |  | `StQACosmicMaker` |  |
| `QAalltrigs` |  | `` | Analyze all triggers in QA |
| `QAallevents` |  | `` | Analyze all events in QA |
| `HitFilt` |  | `StHitFilterMaker` | Hit filter Maker |
| `SvtHitFilt` | HitFilt | `` | SVT Hit filter Maker |
| `TpcHitFilt` | HitFilt | `` | filter out TPC Hits not on tracks |
| `HftHitFilt` | HitFilt | `` | filter out non-HFT hits |
| `KeepTpcHit` | HitFilt | `` | Keep all TPC hits in StHitFilterMaker |
| `KeepSvtHit` | HitFilt | `` | Keep all SVT hits in StHitFilterMaker |
| `KeepFgtHit` | HitFilt | `` | Keep all FGT hits in StHitFilterMaker |
| `Tree` |  | `StTreeMaker` | Write requested branches into files |
| `logger` |  | `` | Use log4cxx package to manage the program messages |
| `NoSimuDb` |  | `` | Switch off Simu Option for DB |
| `SimuDb` | -NoSimuDb | `` | Switch on Simu Option for DB |
| `NoOutput` | -Tree,-EvOut,noTags | `` | Suppress Output root files |
| `EvOutOnly` | EvOut,Tree,noTags | `` | Only event.root files |
| `NoDefault` |  | `` | No Default consistency check |
| `Notpc_daq` | -tpc_daq | `` | No tpc_daq |
| `analysis` | StEvent | `StAnalysisMaker` | Example of Analysis |
| `NewTpcAlignment` |  | `` | Switch on new Tpc Alignment schema |

</details>
