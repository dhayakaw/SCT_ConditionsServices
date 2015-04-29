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

## About SCT ROD id
See  
https://svnweb.cern.ch/trac/atlasgroups/browser/Detectors/SCT/AnalysisCode/SCT_AxAODAnalysis/dat/cabling_Mar2015.dat  
The meaning of 5 numbers at the end of each line  
bec layer phi eta side  

