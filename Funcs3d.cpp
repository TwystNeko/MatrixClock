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

   Temp.ax = ax - p.ax;
   Temp.ay = ay - p.ay;
   Temp.az = az - p.az;

   Temp.wx = wx - p.wx;
   Temp.wy = wy - p.wy;
   Temp.wz = wz - p.wz;

   Temp.sx = sx - p.sx;
   Temp.sy = sy - p.sy;
   Temp.sz = sz - p.sz;
   return Temp;
  }

Point3d Point3d::operator + ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx + p.lx;
   Temp.ly = ly + p.ly;
   Temp.lz = lz + p.lz;

   Temp.ax = ax + p.ax;
   Temp.ay = ay + p.ay;
   Temp.az = az + p.az;

   Temp.wx = wx + p.wx;
   Temp.wy = wy + p.wy;
   Temp.wz = wz + p.wz;

   Temp.sx = sx + p.sx;
   Temp.sy = sy + p.sy;
   Temp.sz = sz + p.sz;
   return Temp;
  }
Point3d Point3d::operator * ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx * p.lx;
   Temp.ly = ly * p.ly;
   Temp.lz = lz * p.lz;

   Temp.ax = ax * p.ax;
   Temp.ay = ay * p.ay;
   Temp.az = az * p.az;

   Temp.wx = wx * p.wx;
   Temp.wy = wy * p.wy;
   Temp.wz = wz * p.wz;

   Temp.sx = sx * p.sx;
   Temp.sy = sy * p.sy;
   Temp.sz = sz * p.sz;
   return Temp;
  }
Point3d Point3d::operator / ( Point3d &p )
  {Point3d Temp;
   Temp.lx = lx / p.lx;
   Temp.ly = ly / p.ly;
   Temp.lz = lz / p.lz;

   Temp.ax = ax / p.ax;
   Temp.ay = ay / p.ay;
   Temp.az = az / p.az;

   Temp.wx = wx / p.wx;
   Temp.wy = wy / p.wy;
   Temp.wz = wz / p.wz;

   Temp.sx = sx / p.sx;
   Temp.sy = sy / p.sy;
   Temp.sz = sz / p.sz;
   return Temp;
  }

Point3d &Point3d::operator -= ( Point3d &p )
  {lx -= p.lx;
   ly -= p.ly;
   lz -= p.lz;

   ax -= p.ax;
   ay -= p.ay;
   az -= p.az;

   wx -= p.wx;
   wy -= p.wy;
   wz -= p.wz;

   sx -= p.sx;
   sy -= p.sy;
   sz -= p.sz;
   return *this;
  }
Point3d &Point3d::operator += ( Point3d &p )
  {lx += p.lx;
   ly += p.ly;
   lz += p.lz;

   ax += p.ax;
   ay += p.ay;
   az += p.az;

   wx += p.wx;
   wy += p.wy;
   wz += p.wz;

   sx += p.sx;
   sy += p.sy;
   sz += p.sz;
   return *this;
  }
Point3d &Point3d::operator *= ( Point3d &p )
  {lx *= p.lx;
   ly *= p.ly;
   lz *= p.lz;

   ax *= p.ax;
   ay *= p.ay;
   az *= p.az;

   wx *= p.wx;
   wy *= p.wy;
   wz *= p.wz;

   sx *= p.sx;
   sy *= p.sy;
   sz *= p.sz;
   return *this;
  }
Point3d &Point3d::operator /= ( Point3d &p )
  {lx /= p.lx;
   ly /= p.ly;
   lz /= p.lz;

   ax /= p.ax;
   ay /= p.ay;
   az /= p.az;

   wx /= p.wx;
   wy /= p.wy;
   wz /= p.wz;

   sx /= p.sx;
   sy /= p.sy;
   sz /= p.sz;
   return *this;
  }

Point3d Point3d::operator - ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx- Value);
   Temp.ly = (long)(ly - Value);
   Temp.lz = (long)(lz - Value);

   Temp.ax = (long)(ax- Value);
   Temp.ay = (long)(ay - Value);
   Temp.az = (long)(az - Value);

   Temp.wx = (long)(wx- Value);
   Temp.wy = (long)(wy - Value);
   Temp.wz = (long)(wz - Value);

   Temp.sx = (long)(sx- Value);
   Temp.sy = (long)(sy - Value);
   Temp.sz = (long)(sz - Value);
   return Temp;
  }
Point3d Point3d::operator + ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx + Value);
   Temp.ly = (long)(ly + Value);
   Temp.lz = (long)(lz + Value);

   Temp.ax = (long)(ax+ Value);
   Temp.ay = (long)(ay + Value);
   Temp.az = (long)(az + Value);

   Temp.wx = (long)(wx+ Value);
   Temp.wy = (long)(wy + Value);
   Temp.wz = (long)(wz + Value);

   Temp.sx = (long)(sx+ Value);
   Temp.sy = (long)(sy + Value);
   Temp.sz = (long)(sz + Value);
   return Temp;
  }
Point3d Point3d::operator * ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx * Value);
   Temp.ly = (long)(ly * Value);
   Temp.lz = (long)(lz * Value);

   Temp.ax = (long)(ax* Value);
   Temp.ay = (long)(ay * Value);
   Temp.az = (long)(az * Value);

   Temp.wx = (long)(wx* Value);
   Temp.wy = (long)(wy * Value);
   Temp.wz = (long)(wz * Value);

   Temp.sx = (long)(sx* Value);
   Temp.sy = (long)(sy * Value);
   Temp.sz = (long)(sz * Value);
   return Temp;
  }
Point3d Point3d::operator / ( double Value )
  {Point3d Temp;
   Temp.lx = (long)(lx / Value);
   Temp.ly = (long)(ly / Value);
   Temp.lz = (long)(lz / Value);

   Temp.ax = (long)(ax/ Value);
   Temp.ay = (long)(ay / Value);
   Temp.az = (long)(az / Value);

   Temp.wx = (long)(wx/ Value);
   Temp.wy = (long)(wy / Value);
   Temp.wz = (long)(wz / Value);

   Temp.sx = (long)(sx/ Value);
   Temp.sy = (long)(sy / Value);
   Temp.sz = (long)(sz / Value);
   return Temp;
  }
  
Point3d &Point3d::operator -= ( double Value )
  {lx -= (long)Value;
   ly -= (long)Value;
   lz -= (long)Value;
    
   ax -= (long)Value;
   ay -= (long)Value;
   az -= (long)Value;
    
   wx -= (long)Value;
   wy -= (long)Value;
   wz -= (long)Value;
    
   sx -= (long)Value;
   sy -= (long)Value;
   sz -= (long)Value;
   return *this;
  }
Point3d &Point3d::operator += ( double V )
  {lx += (long)V;
   ly += (long)V;
   lz += (long)V;
    
   ax += (long)V;
   ay += (long)V;
   az += (long)V;
    
   wx += (long)V;
   wy += (long)V;
   wz += (long)V;
    
   sx += (long)V;
   sy += (long)V;
   sz += (long)V;
   return *this;
  }
Point3d &Point3d::operator *= ( double Value )
  {lx *= (long)Value;
   ly *= (long)Value;
   lz *= (long)Value;

   ax *= (long)Value;
   ay *= (long)Value;
   az *= (long)Value;
    
   wx *= (long)Value;
   wy *= (long)Value;
   wz *= (long)Value;
    
   sx *= (long)Value;
   sy *= (long)Value;
   sz *= (long)Value;
   return *this;
  }
Point3d &Point3d::operator /= ( double V )
  {lx /= (long)V;
   ly /= (long)V;
   lz /= (long)V;

   ax /= (long)V;
   ay /= (long)V;
   az /= (long)V;
    
   wx /= (long)V;
   wy /= (long)V;
   wz /= (long)V;
    
   sx /= (long)V;
   sy /= (long)V;
   sz /= (long)V;
   return *this;
  }



Point3d Th3dtran::getNormal(Point3d a, Point3d b, Point3d c) {
  Point3d n;
  Point3d U = (b - a);
  Point3d V = (c - a);

  n.ax = (U.ay * V.az) - (U.az * V.ay);
  n.ay = (U.az * V.ax) - (U.ax * V.az);
  n.az = (U.ax * V.ay) - (U.ay * V.ax);

  n.lx = (U.ly * V.lz) - (U.lz * V.ly);
  n.ly = (U.lz * V.lx) - (U.lx * V.lz);
  n.lz = (U.lx * V.ly) - (U.ly * V.lx);

  n.wx = (U.wy * V.wz) - (U.wz * V.wy);
  n.wy = (U.wz * V.wx) - (U.wx * V.wz);
  n.wz = (U.wx * V.wy) - (U.wy * V.wx);

  n.sx = (U.sy * V.sz) - (U.sz * V.sy);
  n.sy = (U.sz * V.sx) - (U.sx * V.sz);
  n.sz = (U.sx * V.sy) - (U.sy * V.sx);
 // return Normalize(n);
  return n;
}

Point3d Th3dtran::Normalize(Point3d V) { 
  Point3d N;
  float length = sqrt((V.ax * V.ax) + (V.ay * V.ay) + (V.ax * V.az));
  N.ax = V.ax/length;
  N.ay = V.ay/length;
  N.az = V.az/length;
  length = sqrt((V.wx * V.wx) + (V.wy * V.wy) + (V.wx * V.wz));
  N.wx = V.wx/length;
  N.wy = V.wy/length;
  N.wz = V.wz/length;
  length = sqrt((V.sx * V.sx) + (V.sy * V.sy) + (V.sx * V.sz));
  N.sx = V.sx/length;
  N.sy = V.sy/length;
  N.sz = V.sz/length;
  length = sqrt((V.lx * V.lx) + (V.ly * V.ly) + (V.lx * V.lz));
  N.lx = V.lx/length;
  N.ly = V.ly/length;
  N.lz = V.lz/length;
  return N;
}

void Th3dtran::sortDepthMap(DepthMap *depthMap, int numFaces) {
  int swapped;
  int i;
  int tempID;
  float tempDepth;
   
  do {
    swapped = 0;
    for (i = 1; i < numFaces; i++) {
      if (depthMap[i-1].depth > depthMap[i].depth) {
        tempID = depthMap[i].ID;
        tempDepth = depthMap[i].depth;
        depthMap[i].ID = depthMap[i-1].ID;
        depthMap[i].depth = depthMap[i-1].depth;
        depthMap[i-1].ID = tempID;
        depthMap[i-1].depth = tempDepth;
        swapped = 1;
      }
    }
  } while(swapped != 0);
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
{ 
  p.wx=(long)(p.ax*matrix.Matrix[0][0]+p.ay*matrix.Matrix[1][0]+p.az*matrix.Matrix[2][0]+matrix.Matrix[3][0]);
  p.wy=(long)(p.ax*matrix.Matrix[0][1]+p.ay*matrix.Matrix[1][1]+p.az*matrix.Matrix[2][1]+matrix.Matrix[3][1]);
  p.wz=(long)(p.ax*matrix.Matrix[0][2]+p.ay*matrix.Matrix[1][2]+p.az*matrix.Matrix[2][2]+matrix.Matrix[3][2]);
  return p;
}

Point3d Th3dtran::ChangeObjectPoint(Point3d &p)
{
  p.ax=(long)(p.lx*objectmatrix.Matrix[0][0]+p.ly*objectmatrix.Matrix[1][0]+(long)p.lz*objectmatrix.Matrix[2][0]+objectmatrix.Matrix[3][0]);
  p.ay=(long)(p.lx*objectmatrix.Matrix[0][1]+p.ly*objectmatrix.Matrix[1][1]+(long)p.lz*objectmatrix.Matrix[2][1]+objectmatrix.Matrix[3][1]);
  p.az=(long)(p.lx*objectmatrix.Matrix[0][2]+p.ly*objectmatrix.Matrix[1][2]+(long)p.lz*objectmatrix.Matrix[2][2]+objectmatrix.Matrix[3][2]);
  return p;
}