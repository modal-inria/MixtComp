mkdir -p build
cd build
if [ $# -eq 0 ]
then
	cmake -DCMAKE_BUILD_TYPE=Debug ../src
else
	cmake -DCMAKE_BUILD_TYPE=$1 ../src
fi
cmake --build .