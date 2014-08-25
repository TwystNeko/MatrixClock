#include "Funcs3d.h"
#include "math.h"


Point3d::Point3d()
 { lx=ly=lz=ax=ay=az=at=wx=wy=wz=sx=sy=sz=0;
   lt=wt=st=1;
 }

 int Point3d::operator == (Point3d &p)
{int rvalue=0;
 if(p.lx==lx)
  if(p.ly==ly)
   if(p.lz==lz)
     rvalue=1;
 return rvalue;
}

int Point3d::operator != (Point3d &p)
{ int rvalue=0;
  if((p.lx!=lx)||(p.ly != ly)||(p.lz != lz))
     rvalue=1;
  return rvalue;
}

Point3d Point3d::operator - ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx - p.lx;
   Temp.ly = ly - p.ly;
   Temp.lz = lz - p.lz;
   return Temp;
  }
Point3d Point3d::operator + ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx + p.lx;
   Temp.ly = ly + p.ly;
   Temp.lz = lz + p.lz;
   return Temp;
  }
Point3d Point3d::operator * ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx * p.lx;
   Temp.ly = ly * p.ly;
   Temp.lz = lz * p.lz;
   return Temp;
  }
Point3d Point3d::operator / ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx / p.lx;
   Temp.ly = ly / p.ly;
   Temp.lz = lz / p.lz;
   return Temp;
  }

Point3d &Point3d::operator -= ( Point3d &p )
  {lx -= p.lx;
   ly -= p.ly;
   lz -= p.lz;
   return *this;
  }
Point3d &Point3d::operator += ( Point3d &p )
  {lx += p.lx;
   ly += p.ly;
   lz += p.lz;
   return *this;
  }
Point3d &Point3d::operator *= ( Point3d &p )
  {lx *= p.lx;
   ly *= p.ly;
   lz *= p.lz;
   return *this;
  }
Point3d &Point3d::operator /= ( Point3d &p )
  {lx /= p.lx;
   ly /= p.ly;
   lz /= p.lz;
   return *this;
  }

Point3d Point3d::operator - ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx- Value);
   Temp.ly = (long)(ly - Value);
   Temp.lz = (long)(lz - Value);
   return Temp;
  }
Point3d Point3d::operator + ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx + Value);
   Temp.ly = (long)(ly + Value);
   Temp.lz = (long)(lz + Value);
   return Temp;
  }
Point3d Point3d::operator * ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx * Value);
   Temp.ly = (long)(ly * Value);
   Temp.lz = (long)(lz * Value);
   return Temp;
  }
Point3d Point3d::operator / ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx / Value);
   Temp.ly = (long)(ly / Value);
   Temp.lz = (long)(lz / Value);
   return Temp;
  }
  
Point3d &Point3d::operator -= ( double Value )
  {lx -= (long)Value;
   ly -= (long)Value;
   lz -= (long)Value;
   return *this;
  }
Point3d &Point3d::operator += ( double V )
  {lx += (long)V;
   ly += (long)V;
   lz += (long)V;
   return *this;
  }
Point3d &Point3d::operator *= ( double Value )
  {lx *= (long)Value;
   ly *= (long)Value;
   lz *= (long)Value;
   return *this;
  }
Point3d &Point3d::operator /= ( double V )
  {lx /= (long)V;
   ly /= (long)V;
   lz /= (long)V;
   return *this;
  }



Matrix3d::Matrix3d() 
{
  MatrixIdentity();
}

void Matrix3d::MatrixReset() { 
  for(int i=0;i<4;i++)
  {
    for(int j=0;j<4;j++)
    {
      Matrix[i][j]=0;
    }
  }
}

void Matrix3d::MatrixIdentity() { 
  MatrixReset();
  Matrix[0][0] = Matrix[1][1] = Matrix [2][2] = Matrix[3][3] = 1;
}

void Matrix3d::MatrixCopy(Matrix3d &NewM){
  Matrix3d temp;
  int i,j;
  for(i = 0;i<4;i++){
    for(j =0; j<4; j++) {
      temp.Matrix[i][j] = (Matrix[i][0]*NewM.Matrix[0][j]) + (Matrix[i][1]*NewM.Matrix[1][j]) + (Matrix[i][2]*NewM.Matrix[2][j]) + (Matrix[i][3]*NewM.Matrix[3][j]);
    }
  }
  for(i=0;i<4;i++) { 
    for(j=0;j<4;j++) {
      Matrix[i][j] = temp.Matrix[i][j];
    }
  }
}

void Matrix3d::MatrixMult(Matrix3d &M1, Matrix3d &M2){
  Matrix3d temp;
  int i,j;
  for(i = 0;i<4;i++){
    for(j =0; j<4; j++) {
      temp.Matrix[i][j] = (M2.Matrix[i][0]*M1.Matrix[0][j]) + (M2.Matrix[i][1]*M1.Matrix[1][j]) + (M2.Matrix[i][2]*M1.Matrix[2][j]) + (M2.Matrix[i][3]*M1.Matrix[3][j]);
    }
  }
  for(i=0;i<4;i++) { 
    for(j=0;j<4;j++) {
      M1.Matrix[i][j] = temp.Matrix[i][j];
    }
  }
}

Th3dtran::Th3dtran() { 
  init();
  local = 1;
}

void Th3dtran::init() { 
  matrix.MatrixIdentity();
  objectmatrix.MatrixIdentity();
}

void Th3dtran::Translate(float x, float y, float z) {
  Rmat.MatrixIdentity();
  Rmat.Matrix[3][0]=x;
  Rmat.Matrix[3][1]=y;
  Rmat.Matrix[3][2]=z;
  if(local) { 
    objectmatrix.MatrixCopy(Rmat);
  } else {
    matrix.MatrixCopy(Rmat);
  }
}

void Th3dtran::Rotate(float x, float y, float z) { 
  rmatrix.MatrixIdentity();
  Rmat.MatrixIdentity();
  Rmat.Matrix[1][1]=cos(x); Rmat.Matrix[1][2]=sin(x);
  Rmat.Matrix[2][1]=-(sin(x)); Rmat.Matrix[2][2]=cos(x);
  rmatrix.MatrixMult(rmatrix,Rmat);
  Rmat.MatrixIdentity();
  Rmat.Matrix[0][0]=cos(y);Rmat.Matrix[0][2]=-(sin(y));
  Rmat.Matrix[2][0]=sin(y);Rmat.Matrix[2][2]=cos(y);
  Rmat.MatrixMult(rmatrix,Rmat);
  Rmat.MatrixIdentity();
  Rmat.Matrix[0][0]=cos(z); Rmat.Matrix[0][1]=sin(z);
  Rmat.Matrix[1][0]=-(sin(z)); Rmat.Matrix[1][1]=cos(z);
  Rmat.MatrixMult(rmatrix,Rmat);

  if(local)
   {
    objectmatrix.MatrixIdentity();
    objectmatrix.MatrixCopy(rmatrix);
   }
  else
   {
    matrix.MatrixCopy(rmatrix);
   } 
}

void Th3dtran::Scale(float scale) {
  Rmat.MatrixIdentity();
  Rmat.Matrix[0][0] = scale;
  Rmat.Matrix[1][1] = scale;
  Rmat.Matrix[2][2] = scale;
  if(local) { 
    objectmatrix.MatrixCopy(Rmat);
  } else { 
    matrix.MatrixCopy(Rmat);
  }
}

Point3d Th3dtran::ChangeLocalObject(Point3d &p)
{ p.wx=(long)(p.ax*matrix.Matrix[0][0]+p.ay*matrix.Matrix[1][0]+
              p.az*matrix.Matrix[2][0]+matrix.Matrix[3][0]);
  p.wy=(long)(p.ax*matrix.Matrix[0][1]+p.ay*matrix.Matrix[1][1]+
              p.az*matrix.Matrix[2][1]+matrix.Matrix[3][1]);
  p.wz=(long)(p.ax*matrix.Matrix[0][2]+p.ay*matrix.Matrix[1][2]+
              p.az*matrix.Matrix[2][2]+matrix.Matrix[3][2]);
  return p;
}

Point3d Th3dtran::ChangeObjectPoint(Point3d &p)
{p.ax=(long)(p.lx*objectmatrix.Matrix[0][0]+p.ly*objectmatrix.Matrix[1][0]+(long)
             p.lz*objectmatrix.Matrix[2][0]+objectmatrix.Matrix[3][0]);
 p.ay=(long)(p.lx*objectmatrix.Matrix[0][1]+p.ly*objectmatrix.Matrix[1][1]+(long)
             p.lz*objectmatrix.Matrix[2][1]+objectmatrix.Matrix[3][1]);
 p.az=(long)(p.lx*objectmatrix.Matrix[0][2]+p.ly*objectmatrix.Matrix[1][2]+(long)
             p.lz*objectmatrix.Matrix[2][2]+objectmatrix.Matrix[3][2]);
 return p;
}