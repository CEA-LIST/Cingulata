NOISE :

In Python3 :
from noise import noise
then use test_parameter_set to get the resulting log2 of the noise for each method in the paper.

TIME :

In the CMakeLists in Time/src change the path to tfhe library (tfhelib path in TFHE_PREFIX).

Place yourself in folder ComBoPaperTime.

In command line write :
mkdir build
mkdir bin
cmake -S . -B ./build
cd build
make
./../bin/executable

Then enter the parameters in order.

NOTE: set offset to (temp1 * halfBg) + (1 << (31 - l * Bgbit)) in
tfhe/src/libtfhe/tgsw
