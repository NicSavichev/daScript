rm -r -f cmake_temp
mkdir cmake_temp
cd cmake_temp
CC=gcc-9 CXX=g++-9 cmake -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=MINSIZEREL -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -G "Unix Makefiles" ../

