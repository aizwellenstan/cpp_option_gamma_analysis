rm -rf out/build
mkdir -p out/build
cmake -S . -B out/build
cd out/build
make
cd ../..
./out/build/Cmake data/in.csv data/out.csv