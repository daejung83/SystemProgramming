#!/bin/bash

# exit if anything fails
set -e

rm -f -r reference_imgs
rm -f -r user_imgs
mkdir -p reference_imgs/one
mkdir -p reference_imgs/two
mkdir -p reference_imgs/three
mkdir -p reference_imgs/four
mkdir -p user_imgs/one
mkdir -p user_imgs/two
mkdir -p user_imgs/three
mkdir -p user_imgs/four

echo Making and running reference
make clean
make reference

# don't exit if any of the testers fail
set +e

echo ./poisson_test-reference 256 4 50 0.000005
time ./poisson_test-reference 256 4 50 0.000005
mv *.bmp reference_imgs/one
echo ./poisson_test-reference 512 4 100 0.00005
time ./poisson_test-reference 512 4 100 0.00005
mv *.bmp reference_imgs/two
echo ./poisson_test-reference 1024 8 150 0.0005
time ./poisson_test-reference 1024 8 150 0.0005
mv *.bmp reference_imgs/three
#echo ./poisson_test-reference 2048 8 500 0.0005
#time ./poisson_test-reference 2048 8 500 0.0005
#mv *.bmp reference_imgs/four

echo Making and running your implementation
make clean
make

echo ./poisson_test 256 4 50 0.000005
time ./poisson_test 256 4 50 0.000005
mv *.bmp user_imgs/one
echo ./poisson_test 512 4 100 0.00005
time ./poisson_test 512 4 100 0.00005
mv *.bmp user_imgs/two
echo ./poisson_test 1024 8 150 0.0005
time ./poisson_test 1024 8 150 0.0005
mv *.bmp user_imgs/three
#echo ./poisson_test 2048 8 500 0.0005
#time ./poisson_test 2048 8 500 0.0005
#mv *.bmp user_imgs/four

diff -r user_imgs reference_imgs
echo Test completed!
