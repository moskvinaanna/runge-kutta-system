// Runge-Kutta_system.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <locale>
#include <omp.h>
#include <fstream>

using namespace std;

double f(double t, double y, double z) {
	return sin(t) + y;
}

double g(double t, double y, double z) {
	return t+z;
}


int main()
{
	setlocale(LC_ALL, "");
	double t0, y0, z0, tn;
	int n, threadCount;
	ofstream myfile;
	myfile.open("points.txt");
	cout << "Введите начальные условия" << endl;
	cout << "Введите t0: ";
	cin >> t0;
	cout << "Введите y0: ";
	cin >> y0;
	cout << "Введите z0: ";
	cin >> z0;
	cout << "Введите точку tn в которой будет производиться вычисление: ";
	cin >> tn;
	cout << "Введите величину разбиения n: ";
	cin >> n;
	cout << "Введите количество потоков: ";
	cin >> threadCount;
	double *t, *y, *z;
	t = (double*)malloc(n * sizeof(double));
	y = (double*)malloc(n * sizeof(double));
	z = (double*)malloc(n * sizeof(double));
	double h = (tn - t0) / n;
	double k1, k2, k3, k4, l1, l2, l3, l4;
	t[0] = t0;
	y[0] = y0;
	z[0] = z0;
#pragma omp parallel for num_threads(threadCount)
	for (int i = 1; i < n; i++) {
		k1 = h * f(t[i - 1], y[i - 1], z[i - 1]);
		l1 = h * g(t[i - 1], y[i - 1], z[i - 1]);

		k2 = h * f(t[i - 1] + h / 2, y[i - 1] + k1 / 2, z[i - 1] + l1 / 2);
		l2 = h * g(t[i - 1] + h / 2, y[i - 1] + k1 / 2, z[i - 1] + l1 / 2);

		k3 = h * f(t[i - 1] + h / 2, y[i - 1] + k2 / 2, z[i - 1] + l2 / 2);
		l3 = h * g(t[i - 1] + h / 2, y[i - 1] + k2 / 2, z[i - 1] + l2 / 2);

		k4 = h * f(t[i - 1] + h, y[i - 1] + k3, z[i - 1] + l3);
		l4 = h * g(t[i - 1] + h, y[i - 1] + k3, z[i - 1] + l3);
		t[i] = t[i - 1] + h;
		y[i] = y[i - 1] + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		z[i] = z[i - 1] + (l1 + 2 * l2 + 2 * l3 + l4) / 6;
	}
	for (int i = 0; i < n; i++) {
		myfile << t[i] << " ";
	}
	myfile << endl;
	for (int i = 0; i < n; i++) {
		myfile << y[i] << " ";
	}
	myfile << endl;
	for (int i = 0; i < n; i++) {
		myfile << z[i] << " ";
	}
	myfile.close();
	return 0;
}

