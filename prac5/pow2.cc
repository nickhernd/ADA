//JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
using namespace std;

unsigned int STEPS1 = 0;
unsigned int STEPS2 = 0;
unsigned int STEPS3 = 0;

// Coste: \Theta(n)
unsigned long pow2_1(unsigned n) {
    STEPS1++;

    if (n == 0)
        return 1;
        
    return pow2_1(n - 1) * 2;
}

// Coste: \Omicron(log n)
unsigned long pow2_2(unsigned n) {
    STEPS2++; 

    if (n == 0) return 1;
    if (n == 1) return 2;

    if (n % 2 == 0) {
        return pow2_2(n / 2) * pow2_2(n / 2);
    } 

    return pow2_2((n - 1) / 2) * pow2_2((n - 1) / 2) * 2;
}

// Coste: \Theta (2^n)
unsigned long pow2_3(unsigned n){
    STEPS3++;

	if(n == 0)
		return 1;
	
   return pow2_3(n-1) + pow2_3(n-1);
    
}

int main (){

	for (unsigned exp = 10; exp <= 30; exp++) {
		
        pow2_1(exp);
        cout << exp << "\t" << STEPS1*100 << "\t";
        pow2_2(exp);
        cout << STEPS2*100 << "\t";
        pow2_3(exp);
        cout << STEPS3/1000 << endl;
    }

	return 0;
}
