rm a.out
rm scheduler
rm cpu_emulator
g++ -std=c++11 -o scheduler scheduler.cpp
g++ -std=c++11 -o cpu_emulator cpu_emulator.cpp
gcc init.c
echo "RUN a.out"
./a.out $1 $2 $3