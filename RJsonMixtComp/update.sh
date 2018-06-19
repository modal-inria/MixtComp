cp ../RMixtComp/R/MIXTCOMP_getData.R ./R/MIXTCOMP_getData.R
cp ../RMixtComp/R/MIXTCOMP_getter.R ./R/MIXTCOMP_getter.R
cd ../JsonMixtcomp/
./clean.sh
./buildRelease.sh
cp JsonMixtComp ../RJsonMixtComp/inst/exeMixtComp/JsonMixtComp
