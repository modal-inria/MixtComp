cd debug
find . ! -name 'empty' -type f -exec rm -f {} +
find . ! -name 'empty' -type f -exec rm -r -f {} +
cd ../release 
find . ! -name 'empty' -type f -exec rm -f {} +
find . ! -name 'empty' -type f -exec rm -r -f {} +
cd ..