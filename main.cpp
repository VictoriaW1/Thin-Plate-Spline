#include "TPS.h"

int main() {
	float x[8] = { 0, 1, -1, 0, 0, -1, 1, 0 };
	float y[8] = { 0, 0.75, -1, 0.25, 0, -1.25, 1, 0.25 };
	Mat X(4, 2, CV_32FC1, x);
	Mat Y(4, 2, CV_32FC1, y);
	TPS model(X, Y);
	cout <<"Gamma"<< model.getGamma() <<endl;
	cout <<"w"<< model.getW() << endl;
	cout << "c" << model.getC() << endl;
	cout << "a" << model.getA() << endl;

	return 0;
}
