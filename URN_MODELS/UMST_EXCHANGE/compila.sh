g++ -c  -O3 rangen.C 
g++ -c   -O3 wr_polya_fun.C 
g++ -c   -O3 wr_polya.C 
g++ -c   -O3 wr_polya_ogg.C 
g++ -o exe wr_polya_fun.o rangen.o wr_polya.o wr_polya_ogg.o
