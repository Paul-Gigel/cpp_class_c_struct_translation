#include <iostream>
#include <cfloat>
double zahl1 = 2.3;
double zahl2 = 1.2;
double zahl3 = 1.99999999998;

double fixSubNumber(double left, double right) {
	double ergebniss = left - right;
	for (double trim = 1; !(ergebniss + left == right); trim++) {
		if (ergebniss + left < right) {
			ergebniss += trim;
			if ((ergebniss + left > right))	{
				ergebniss -= trim;
				trim /= 10;
			}
		}
		else if (ergebniss + left > right) {
			ergebniss -= trim;
			if ((ergebniss + left < right)) {
				ergebniss += trim;
				trim /= 10;
			}
		}
	}
	return 0;
}

int main()	{
	//fixSubNumber(zahl1, zahl2);
	std::cout << 3+((double)2.2250738585072014e-308) << "\n";
}