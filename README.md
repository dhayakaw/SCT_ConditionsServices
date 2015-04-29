# SCT_ConditionsServices

These codes are for masking SCT RODs from RDO to xAOD (reconstructing) with release 20.0.0.2.

## Usage
asetup 20.0.0.2,here
cmt co -r SCT_ConditionsServices-00-04-27 InnerDetector/InDetConditions/SCT_ConditionsServices
pkgco.py InnerDetector/InDetExample/InDetRecExample
setupWorkArea.py
cd WorkArea/cmt
cmt config; source setup.sh; cmt bro gmake

> Modify InDetRecExample/share/InDetRecConditionsAccess.py like
> https://github.com/dhayakaw/InDetRecExample/blob/master/share/InDetRecConditionsAccess.py

cd ../run
Reco_tf.py, athena, etc..

