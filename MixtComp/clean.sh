cd debug
find . ! -name 'empty' -type f -exec rm -f {} +
find . ! -name '.' -type d -exec rm -r -f {} +
cd ../release 
find . ! -name 'empty' -type f -exec rm -f {} +
find . ! -name '.' -type d -exec rm -r -f {} +
cd ..