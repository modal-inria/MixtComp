find build ! -name 'empty' -type f -exec rm -f {} +
find build ! -name '.' -type d -exec rm -r -f {} +