appName=RMixtComp
version=0.1
 
archiveName=$appName\_$version.tar.gz
 
mkdir -p temp
 
mv $archiveName temp
cd temp
tar xzf $archiveName
rm $archiveName
 
find ./ -follow -type f -name '*.o' -delete
find ./ -follow -type f -name '*.a' -delete
find ./ -follow -type f -name '*.so' -delete
 
tar czf $archiveName $appName
 
cd ..
 
mv temp/$archiveName .
 
rm -Rf temp
