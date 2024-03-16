//JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
using namespace std;

unsigned int STEPS1 = 0;
unsigned int STEPS2 = 0;
unsigned int STEPS3 = 0;

// Coste: \Theta(n)
unsigned long pow2_1(unsigned n) {
    unsigned long res;

    if (n == 0)
        return 1;

    res = 2 * pow2_1(n - 1);
    STEPS1++;
        
    return res;
}

// Coste: \Theta(log n)
unsigned long pow2_2(unsigned n) {
    unsigned long res;

    if (n == 0) return 1;
    if (n == 1) return 2;

    if (n % 2 == 0) {
        res = pow2_2(n / 2) * pow2_2(n / 2);
    } else {
        res = pow2_2((n - 1) / 2) * pow2_2((n - 1) / 2) * 2;
    }

    STEPS2++; 

    return res;
}

// Coste: \Theta (2^n)
unsigned long pow2_3(unsigned n){
    unsigned long res;

	if(n == 0)
		return 1;
	
    res = pow2_3(n-1) + pow2_3(n-1);
    STEPS3++;

	return res;
}

int main (){

	for (unsigned exp = 5; exp <= 30; exp++) {
		
        pow2_1(exp);
        cout << exp << "\t" << STEPS1 << "\t";
        pow2_2(exp);
        cout << STEPS2 << "\t";
        pow2_3(exp);
        cout << STEPS3 << endl;
    }

	return 0;
}
