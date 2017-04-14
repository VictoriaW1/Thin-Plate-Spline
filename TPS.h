#include <iostream>
#include <string>
#include <math.h>
#include <opencv.hpp>
#include <core/mat.hpp>

using namespace std;
using namespace cv;

//points: (p1_x, p1_y) (p2_x, p2_y)
float sFunction(float p1_x, float p1_y, float p2_x, float p2_y) {
	if (p1_x == p2_x && p1_y == p2_y) {
		return 0;
	}
	else {
		float r = sqrt(pow((p1_x - p2_x), 2) + pow((p1_y - p2_y), 2));
		
		return (pow(r, 2) * log(pow(r, 2)));
	}

}
class TPS {
public:
	TPS(Mat x, Mat y) {
		X = x;
		N = x.rows;
		gamma = Mat(N + 3, N + 3, CV_32FC1);
		cout << x << endl;
		//up left N*N matrix of gamma
		for (int r = 0; r < N; ++r) {
			
			for (int c = 0; c < N; ++c) {
				
				cout << "r" << r << endl;
				cout << "c" << c << endl;
				gamma.at<float>(r, c) = sFunction(x.at<float>(r, 0), x.at<float>(r, 1), x.at<float>(c, 0), x.at<float>(c, 1));
			}
		}

		for (int r = 0; r < N; ++r) {
			gamma.at<float>(r, N) = 1;
			gamma.at<float>(r, N + 1) = x.at<float>(r, 0);
			gamma.at<float>(r, N + 2) = x.at<float>(r, 1);
		}

		for (int c = 0; c < N; ++c) {
			gamma.at<float>(N, c) = 1;
			gamma.at<float>(N + 1, c) = x.at<float>(c, 0);
			gamma.at<float>(N + 2, c) = x.at<float>(c, 1);
		}

		for (int r = N; r < N + 3; ++r) {
			for (int c = N; c < N + 3; ++c) {
				gamma.at<float>(r, c) = 0;
			}
				
		}


		Mat Y(N + 3, 2, CV_32FC1);
		for (int r = 0; r < N; ++r) {
			for (int c = 0; c < 2; ++c) {
				Y.at<float>(r, c) = y.at<float>(r, c);
			}
		}
		cout << "y" << y << endl;
		for (int r = N; r < N + 3; ++r) {
			Y.at<float>(r, 0) = 0;
			Y.at<float>(r, 1) = 0;
		}
		Mat tmp = gamma.inv() * Y;
		w = tmp(Rect(0, 0, 2, N));
		c = tmp(Rect(0, N, 2, 1));
		a = tmp(Rect(0, N + 1, 2, 2));
		cout << "Y" << Y << endl;
	}
	
	float* phi(float x, float y) {
		float* sum = weightedSum(x, y);
		float map_x = c.at<float>(0, 0) + a.at<float>(0, 0) * x + a.at<float>(1, 0) * y + *sum;
		float map_y = c.at<float>(0, 1) + a.at<float>(0, 1)*x + a.at<float>(1, 1) * y + *(sum + 1);
		float* point;
		*point = map_x;
		*(point + 1) = map_y;
		return point;
	}
	Mat fitToImage(Mat img) {
		Mat new_img(img.rows, img.cols, CV_8UC3);
		for (int r = 0; r < img.rows; ++r) {
			uchar* new_p = new_img.ptr<uchar>(r);
			uchar* p = img.ptr<uchar>(r);
			for (int c = 0; c < img.cols; ++c) {
				float* map_point = phi(c, r);
				float map_x = *map_point;
				float map_y = *(map_point + 1);
				for (int i = 0; i < 3; ++i) {
					new_p[3 * c + i] = p[3 * c + i];
				}
			}
		}
		return new_img;
	}

	Mat getW() {
		return w;
	}
	Mat getC() {
		return c;
	}
	Mat getA() {
		return a;
	}
	Mat getGamma() {
		return gamma;
	}
private:
	float* weightedSum(float x, float y) {
		float *sum = new float;
		float sum_x = 0;
		float sum_y = 0;
		for (int n = 0; n < N; ++n) {
			sum_x += w.at<float>(n, 0) * sFunction(x, y, X.at<float>(n, 0), X.at<float>(n, 1));
			sum_y += w.at<float>(n, 1) * sFunction(x, y, X.at<float>(n, 1), X.at<float>(n, 1));
		}
		*sum = sum_x;
		*(sum + 1) = sum_y;
		return sum;
	}
private:
	int N;
	Mat w;
	Mat c;
	Mat a;
	Mat gamma;
	Mat X;
};



