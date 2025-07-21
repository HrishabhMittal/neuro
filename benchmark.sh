echo multiplying 2 1000x1000 matrices
echo ================== no flags ================
g++ src/main.cpp -o main
time ./main
rm main

FLAGS="-O2" 
echo ================== O2 ================
g++ $FLAGS src/main.cpp -o main
time ./main
rm main

FLAGS="-O3"
echo ================== O3 ================
g++ $FLAGS src/main.cpp -o main
time ./main
rm main

FLAGS="$FLAGS -funroll-loops"
echo ================== +unroll loops ================
g++ $FLAGS src/main.cpp -o main
time ./main
rm main

FLAGS="$FLAGS -ffast-math"
echo ================== +fast math ================
g++ $FLAGS src/main.cpp -o main
time ./main
rm main


FLAGS="$FLAGS -mavx2"
echo ================== +simd ================
g++ $FLAGS src/main.cpp -o main
time ./main
rm main
