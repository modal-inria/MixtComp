cp ../RMixtComp/R/MIXTCOMP_getter.R ./R/MIXTCOMP_getter.R
cp ../RMixtComp/R/MIXTCOMP_getParam.R ./R/MIXTCOMP_getParam.R
cp ../RMixtComp/R/MIXTCOMP_misc.R ./R/MIXTCOMP_misc.R
cd ../MixtComp/
./clean.sh
./buildRelease.sh
cp release/json/jmc ../RJsonMixtComp/inst/exeMixtComp/jmc
