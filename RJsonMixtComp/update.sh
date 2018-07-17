cp ../RMixtComp/R/MIXTCOMP_getData.R ./R/MIXTCOMP_getData.R
cp ../RMixtComp/R/MIXTCOMP_getter.R ./R/MIXTCOMP_getter.R
cp ../RMixtComp/R/MIXTCOMP_misc.R ./R/MIXTCOMP_misc.R
cd ../JsonMixtcomp/
./clean.sh
./buildRelease.sh
cp release/exec/JsonMixtComp ../RJsonMixtComp/inst/exeMixtComp/JsonMixtComp
