#ifndef Funcs3d_h
#define Funcs3d_h

class Point3d
{
	public:
	long lx,ly,lz,lt;
	long wx,wy,wz,wt;
	long ax,ay,az,at;
	long sx,sy,sz,st;
	Point3d();
	int operator == (Point3d &V);
	int operator != (Point3d &V);
	Point3d operator -  (Point3d &p);
	Point3d operator +  (Point3d &p);
	Point3d operator *  (Point3d &p);
	Point3d operator /  (Point3d &p);
	Point3d &operator -= (Point3d &p);
	Point3d &operator += (Point3d &p);
	Point3d &operator *= (Point3d &p);
	Point3d &operator /= (Point3d &p);
	int operator == (double p);
	int operator != (double p);
	Point3d operator -  (double p);
	Point3d operator +  (double p);
	Point3d operator *  (double p);
	Point3d operator /  (double p);
	Point3d &operator -= (double p);
	Point3d &operator += (double p);
	Point3d &operator *= (double p);
	Point3d &operator /= (double p);
	void Print();
};

class Matrix3d
{
	public:
	double Matrix[4][4];
	Matrix3d();
	void MatrixReset();
	void MatrixIdentity();
	void MatrixCopy(Matrix3d &M);
	void MatrixMult(Matrix3d &M1, Matrix3d &M2);
};

class Th3dtran
{
public:
	Th3dtran();
	void init();
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float scale);
	Point3d ChangeLocalObject(Point3d &p);
	Point3d ChangeObjectPoint(Point3d &p);

	Matrix3d matrix, Rmat, rmatrix, objectmatrix;
	char local;

};
#endif