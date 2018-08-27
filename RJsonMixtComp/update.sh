cp ../RMixtComp/R/MIXTCOMP_getter.R ./R/MIXTCOMP_getter.R
cp ../RMixtComp/R/MIXTCOMP_misc.R ./R/MIXTCOMP_misc.R
cd ../MixtComp/
./clean.sh
./buildDebug.sh
cp debug/json/jmc ../RJsonMixtComp/inst/exeMixtComp/jmc
