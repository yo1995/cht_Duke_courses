unsigned power(unsigned x, unsigned y){
	if (x == 0 && y == 0)
		return 1;
	if (y == 0)
		return 1;
	return x*power(x,y-1);
}
