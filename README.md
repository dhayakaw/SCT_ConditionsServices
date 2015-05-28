# SCT_ConditionsServices

These codes are for masking SCT RODs from RDO to xAOD (reconstructing) with release 20.0.0.2.
I have not validated yet, so please inform me when you have a problem.

## Usage
asetup 20.0.0.2,here  
cmt co -r SCT_ConditionsServices-00-04-27 InnerDetector/InDetConditions/SCT_ConditionsServices  
pkgco.py InnerDetector/InDetExample/InDetRecExample  
setupWorkArea.py  
cd WorkArea/cmt  
cmt config; source setup.sh; cmt bro gmake  

> Modify InDetRecExample/share/InDetRecConditionsAccess.py like  
> https://github.com/dhayakaw/InDetRecExample/blob/master/share/InDetRecConditionsAccess.py  
> You would understand where I modified the python script after searching "RODVeto" in it.

cd ../run  
Reco_tf.py, athena, etc..  
Reco_tf.py \  
--autoConfiguration everything \  
--DBRelease current \  
--geometryVersion ATLAS-R2-2015-01-01-00 \  
--conditionsTag OFLCOND-RUN12-SDR-30 \  
--inputRDOFile $INPUTFILE \  
--outputAODFile test_AOD.pool.root \  
--maxEvents -1 \  
--preExec 'rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);jobproperties.Beam.bunchSpacing.set_Value_and_Lock(25);from LArROD.LArRODFlags import larRODFlags;larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4)' 'RAWtoESD:from TriggerJobOpts.TriggerFlags import TriggerFlags;TriggerFlags.triggerMenuSetup="DC14_loose_mc_prescale";from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'ESDtoAOD:TriggerFlags.AODEDMSet="AODFULL"' \  
--preInclude HITtoRDO:Digitization/ForceUseOfPileUpTools.py SimulationJobOptions/preInclude.PileUpBunchTrainsUpgradeConfig1_25ns.py RunDependentSimData/configLumi_run222222.py  \  
--postInclude PyJobTransforms/UseFrontier.py  

## About SCT ROD id
See  
https://svnweb.cern.ch/trac/atlasgroups/browser/Detectors/SCT/AnalysisCode/SCT_AxAODAnalysis/dat/cabling_Mar2015.dat  
The meaning of 5 numbers at the end of each line  
bec layer phi eta side  

