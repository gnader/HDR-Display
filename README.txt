Implementation of : Seetzen et al. 2004 - “High dynamic range display systems,” ACM Trans. Graph., vol. 23, no. 3

* code tested on Ubuntu 16.04

* dependancies (included in extern/):
	Eigen  -> manipulate image data (arithmetic operations, ...)
	CImage -> for loading and writing images

* to compile :
	cd hdr_display
	mkdir build
	cd build
	cmake ..
	make

* to run:
	example 1 : ./hdr -in ../data/memorial.exr -res 768 1024 -psf 8 -dlp 5000 5 2.2 -lcd 1 0.005 2.2
	example 2 : ./hdr -in ../data/memorial.exr -> using default value in this case
	example 2 : ./hdr -in ../data/memorial.exr -res 0 0 -out jpg -> using original resolution and saving as jpg

* usage:
	./hdr <option> <values>                             
  	   -in  [filename]               : input image     
  	   -out [format]                 : format of output image (optional)  
  	   -res [width] [height]         : output resolution      (optional)
  	   -psf [sigma]                  : gaussian psf parameter (optional)
  	   -dlp [Lpeak] [Lblack] [gamma] : dlp response model     (optional)
  	   -lcd [Lpeak] [Lblack] [gamma] : lcd response model     (optional)
	
