if [! -e cigSmokers.out] 
then
	gcc cigSmokers.c queue.c -o cigSmokers.out
fi
./cigSmokers.out
