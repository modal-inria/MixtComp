cp ../RMixtComp/R/MIXTCOMP_getter.R ./R/MIXTCOMP_getter.R
cp ../RMixtComp/R/MIXTCOMP_getParam.R ./R/MIXTCOMP_getParam.R
cp ../RMixtComp/R/MIXTCOMP_misc.R ./R/MIXTCOMP_misc.R
cp ../RMixtComp/R/MIXTCOMP_formatData.R ./R/MIXTCOMP_formatData.R
cp ../RMixtComp/R/MIXTCOMP_formatFunctions.R ./R/MIXTCOMP_formatFunctions.R
cd ../MixtComp/
./clean.sh
./buildRelease.sh
cp release/json/jmc ../RJMixtComp/inst/exeMixtComp/jmc
