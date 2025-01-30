#include <iostream>
#include <cmath>



#include "utils/math/Vectors.h"
#include "utils/math/Matrices.h"
#include "utils/math/Transform.h"
#include "utils/math/Quaternion.h"


#include "utils/geometry/Primitives.h"

void mathtests() {
	Vector2 v(7, 1);
	Vector2 w(0.5, 1.9);
	float u = v * w;
	Vector2 uu = v * 1.5;
	std::cout << u << std::endl;
	std::cout << uu.to_string() << std::endl;


	Vector3 x(1, 2, 3);
	Vector3 y(4, 5, 6);

	float dot = x.dot(y);
	Vector3 cross = x.cross(y);
	std::cout << cross.to_string() << std::endl;

	Matrix2 mat(1, 2, 3, 4);
	Vector2 vv(13, 21);
	Vector2 u2 = mat * vv;

	std::cout << u2.to_string() << std::endl;
	
	// 2x2 linear system

	Matrix2 A(-5, 2, -7, 4);
	std::cout << A.eigenvalues().to_string() << "\n";
	std::cout << A.eigenvectors().to_string() << "\n";

	// cross product

	Vector3 c1(1, 2, 3);
	Vector3 c2(5, 2, 7);
	std::cout << (c1.cross(c2)).to_string() << "\n";

	// 3x3 inverse

	Matrix3 T(2, 3, 8, 6, 0, -3, -1, 3, 2);

	Matrix3 inv = T.inv();
	std::cout << inv.to_string() << "\n";

	Matrix3 FIN = T * inv;
	std::cout << FIN.to_string() << "\n";

	// 3x3 linear system

	Matrix3 R(3, 2, -3, 4, -3, 6, 1, 0, -1);
	std::cout << R.sprintf() << "\n";
	Vector3 b(-13, 7, -5);
	Vector3 xx = R.solve(b);
	std::cout << xx.to_string() << std::endl;

	// 3x3 system with multiple solutions (homogenous)
	// Matrix3 s(2, 1, 3, 0, 1, -1, 1, 3, -1);
	Matrix3 s(0, 1, 0, 0, 0, 0, 0, 0, 0);
	Vector3 B(0, 0, 0);

	Vector3 xxx = s.solve(B);
	std::cout << xxx.to_string() << std::endl;

	// 4x4 
	// https://www.symbolab.com/solver/derivative-calculator/inverse%20%5Cbegin%7Bpmatrix%7D1%263%262%261%5C%5C%20%204%262%265%267%5C%5C%20%20-8%262%261%260%5C%5C%20%204%263%26-2%261%5Cend%7Bpmatrix%7D?or=input
	Matrix4 m4(1, 3, 2, 1, 4, 2, 5, 7, -8, 2, 1, 0, 4, 3, -2, 1);

	std::cout << m4.sprintf() << "\n";
	std::cout << m4.det() << "\n";
	std::cout << m4.solve(Vector4(10, 10, 10, 10)).to_string() << "\n";

	Matrix4 n4 = m4.inv();
	std::cout << (n4 * m4).sprintf() << "\n";

	// Transforms

	Matrix3 rot45 = Rotation3(Vector3(1, 0, 0), M_PI / 4);

	Transform r45(Vector3(0, 0, 0), rot45);

	std::cout << r45.sprintf() << "\n";
	std::cout << (r45 * r45).sprintf() << "\n";

	// Quaternions https://stackoverflow.com/questions/19956555/how-to-multiply-two-quaternions

	Quaternion q1(0.98, 0.08, 0.17, -0.01);
	Quaternion q2(0.70, 0.70, 0.0, 0.0);

	std::cout << q1.to_string() << " " << q2.to_string() << "\n";
	std::cout << (q1 * q2).to_string() << "\n";

	// Quaternion rotation

	Quaternion z90 = QuaternionAA(Vector3(0, 0, 1), M_PI / 2);
	Quaternion x90 = QuaternionAA(Vector3(1, 0, 0), M_PI / 2);

	Quaternion y90 = QuaternionAA(Vector3(0, 1, 0), M_PI / 2);

	Vector3 qup(4, 9, 6);
	Vector3 quq = x90(z90(qup));

	std::cout << quq.to_string() << "\n";

	Vector3 qur = z90(y90(y90(x90(quq))));

	std::cout << qur.to_string() << "\n";
}

void geometrytests() {
	// Point and plane

	Plane PI(3, 4, 1, -7);

	std::cout << PI.to_string() << "\n";

	std::cout << PI.distanceFromOrigin() << "\n";

	Vector3 pt(5, -2, 6);
	Vector3 qt(1, 1, 0);

	std::cout << PI.distanceFrom(pt) << "\n";

	Plane QI(2, 3, -5, 1);

	Line in = PI.intersection(QI);
	std::cout << in.to_string() << "\n";

	Line out(Vector3(-1, 4, 2), Vector3(-4, 5, 7));

	std::cout << PI.intersection(out).to_string() << "\n";

	// Plane intersection

	Plane t1(-2, 11, -12, 43);
	Plane t2(0, -1, 1, -3);
	Plane t3(-1, 6, -6, 22);
	Plane u3(-14, -6, -1, 52);

	std::cout << t1.intersection(t2, t3).to_string() << "\n";
	std::cout << t1.intersection(t2, u3).to_string() << "\n";

	// Transforming planes
}

int main() {
	// mathtests();

	// geometrytests();

	Matrix4 A(2, 1, 7, 8, -1, 1, 2, -3, 1, 3, 5, 7, 0, 0, 0, 1);
	Vector4 b(1, 2, 3, 4);

	std::cout << A.solve(b).to_string() << "\n";

	return 0;
}