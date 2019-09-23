cits3402
assignment1
jradich - 22744833

files required: 
	assig1c.c 
	helperFunctiosn.h
	helperFunctions.c
	coordFunctions.h
	coordFunctions.c 
	csrFunctions.h
	csrFunctions.c 
	matrixStructures.h
	
compiling requires all of the above mentioned .c files and the openmp flag: 

gcc -fopenmp assig1c.c helperFunctions.c coordFunctions.c csrFunctions.c -o assig1c 


the following options are valid commands 

--sc X 	(scalar multiply  X: scalar value) 
--tr  	(trace calculation)
--ts	(transpose calculation)
--ad	(addition calculation - requires 2 files) 
--mm	(multiplication calculation - requires 2 files) 

-f  file flag, enter file(s) after this flag separated by a space 
-t thread flag, enter number of threads to use after this flag. Defaults to 1 if omitted  
-l  log flag - include if result matrix is required in output file. 

example: 

assig1c --mm -f m1.in m2.in -t 4 -l 
assig1c --sc 4 -f m1.in -t 8 -l 

 	
	