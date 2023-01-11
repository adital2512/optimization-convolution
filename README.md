# optimization-convolution
optimized version of a image convolution program in c

to run the program:
1. open linux terminal
2. type "make"
3. type ./showBMP <image name> <index> (while index is 1/2/3) - exemple (./showBMP 200_pic/Heritage_200.bmp 1)

or 

(*****not created by me*****)
1. open linux terminal 
2. type python3 compare.py <number of runs> (while the number typed, is the the number of runs will be executed) - exemple (python3 compare.py 20)
3. after the run, you can see the avg runtime of each image and process and the correctness of it.



*****original runtime - 61.2ms*****

*****current runtime - 6.4ms*****
  
  **89.54% improvement**

files changed; 
1. myfunction.h
2. myfunction.c
3. myfunction1.h
4. myfunction1.c
all other files were given to me and whern't allowed to be changed.

original files : ex4_files (4).zip

the folders:
contain the original image and the images after convolusion process (ordered by the size of images).

