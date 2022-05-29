$ gcc -c coordinate.c distance.c dms.c
$ ar -rs libassignment.a coordinate.o distance.o dms.o
$ gcc -static main.c libassignment.a 

Usage
a.out [file path]



