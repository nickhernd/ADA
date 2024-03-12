//JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
using namespace std;

unsigned int STEPS1 = 0;
unsigned int STEPS2 = 0;
unsigned int STEPS3 = 0;

unsigned long pow2_1(unsigned number){
	STEPS1++;
	if(number == 0)
		return 1;
	return (2 * pow2_1(number-1));
}

unsigned long pow2_2(unsigned number){
	STEPS2++;
	if(number == 0)
		return 1;

	if(number%2 == 0)
		return (pow2_2(number/2)) * (pow2_2(number/2));
 	return 2 * (pow2_2(number/2)) * (pow2_2(number/2));
}

unsigned long pow2_3(unsigned number){
	STEPS3++;
	if(number == 0)
		return 1;
	return pow2_3(number-1) + pow2_3(number-1);
}

int main (){

	for(unsigned exp = 1; exp< 32; exp++){
		STEPS1 = STEPS2 = STEPS3 = 0;
		pow2_1(exp);
		cout << STEPS1 << "\t";
		pow2_2(exp);
		cout << STEPS2 << "\t";
		pow2_3(exp);
		cout << STEPS3 << endl;
	}

	return 0;
}
