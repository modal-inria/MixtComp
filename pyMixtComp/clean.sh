find ./build ! -name '.' -type f -exec rm -f {} +
find ./build ! -name '.' -type d -exec rm -r -f {} +

