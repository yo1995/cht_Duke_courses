#include <stdlib.h>
#include <stdio.h>

struct _retire_info{
	int months;
	double contribution;
	double rate_of_return;
};
typedef struct _retire_info retire_info;
//two different ways of writing it. one is to write a function to calculate all.
// second is to write seperate function that calculate once.
double calculate_account_balance(double initial, double rate, double contribution, int startAge){
	int months = startAge % 12;
	int years = (startAge - months) / 12;
	printf("Age %3d month %2d you have $%.2lf\n", years, months, initial); 
	initial += initial * rate;
	initial += contribution;
	return initial;
}

void retirement (int startAge, double initial, retire_info working, retire_info retired) {
	int i = 0;
	int j = 0;
	double balance;
	balance = 0 + initial; //add the initial savings to balance to start calculate.
	// first calculate working part, then retired part. notice the both 0 case.
	int Age_in_month = startAge;
	do{
		balance = calculate_account_balance(balance, working.rate_of_return, working.contribution, Age_in_month);
		i++;
		Age_in_month += 1;
	}while(i < working.months);
	
	startAge += working.months;
	// to recaliberate it?
	Age_in_month = startAge;
	
	do{
		balance = calculate_account_balance(balance, retired.rate_of_return, retired.contribution, Age_in_month);
		j++;
		Age_in_month += 1;
	}while(j < retired.months);
	
	startAge += retired.months;
	
}

int main(){
	retire_info working;
	retire_info retired;
	working.contribution = 1000;
	working.months = 0; //489
	working.rate_of_return = 0.045/12;
	retired.contribution = -4000;
	retired.months = 0; //384
	retired.rate_of_return = 0.01/12;
	
	int startAge = 327; //what if startAge < 0?
	//int startAge = -1; // turned out to have money before born... alright?
	long initial = 21345; //the amount will exceed 32767; what if initial < 0?
	//long initial = INT_MAX; seems to work fine with long, but I wonder if LONG_MAX will cause problem to it?
	
	retirement(startAge, initial, working, retired);
	
	return EXIT_SUCCESS;
}
