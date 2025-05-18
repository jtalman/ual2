//
//  Particle.cc
//

#include "Monitor/Particle.hh"
//#include "Monitor/Twiss.hh"
//#include "Monitor/Rmatrix.hh"

// ---------   Constructors & destructor  ----------

Particle::Particle()
{
   create();
   initialize();    
}


Particle::Particle(const Particle& P)
{
   create();
   initialize(P);
}
/*
Particle::Particle(const Rmatrix& R)
{
   create();
   initialize(R);
}

Particle::Particle(const Twiss& Tw)
{
   create();
   initialize(Tw);
}

*/ 
Particle::~Particle()
{
   clean();
}

// Access operators

double& Particle::operator[](int number)
{
  if(number < 0 || number > BEAM_DIM)
  {
      cerr << "Error: Particle::operator[](int number) : number(";
      cerr << number << ") out of range [0," << BEAM_DIM << "]\n";
      exit(1);
  }
  return(variable[number]);
}

// Assignment operators
   
void Particle::operator=(const Particle& P)
{
   initialize(P);
}
/*
void Particle::operator=(const Rmatrix& R)
{
   initialize(R);
}

void Particle::operator=(const Twiss& Tw)
{
   initialize(Tw);
}
*/
void Particle::operator+=(const Particle& P)
{

     for(int i=1; i <= BEAM_DIM; i++) 
              variable[i] += P.variable[i];
     return;
}

void Particle::operator-=(const Particle& P)
{

     for(int i=1; i <= BEAM_DIM; i++) 
              variable[i] -= P.variable[i];
     return;
}

void Particle::operator*=(const double v)
{

     for(int i=1; i <= BEAM_DIM; i++) 
              variable[i] *= v;
     return;
}

void Particle::operator/=(const double v)
{
     if(v == 0)
     {
        cerr << "Error : Particle::operator/=(double v) : "
             << "v == 0 \n";
        exit(1);
     }

     for(int i=1; i <= BEAM_DIM; i++) 
              variable[i] /= v;
     return;
}

// Friend operators

ostream& operator<<(ostream& out, const Particle& P)
{
  char s[80];
  static char* keyword[7] = {" ", "x", "px/p0", "y", "py/p0", "-cdt", "dE/p0c"};

  out << "Particle variables\n\n";

  for(int i=1; i <= BEAM_DIM; i++)
  {
    sprintf(s, "%13s ", keyword[i]);
    out << s ;
  }
  out << "\n";

  for(int j=1; j <= BEAM_DIM; j++)
  {
    sprintf(s, "% 12e ", P.variable[j]);
    out << s ;
  } 
  out << "\n\n"; 

  out << "Reference variables \n\n";

  sprintf(s, "%16s %16s %16s %16s", 
    "Energy (Gev)","Momentum (Gev/c)","Velocity (v0/c)","Mass (Gev/c2)");
  out << s << "\n";
  sprintf(s, " % 15e  % 15e  % 15e  % 15e", P.e0, P.p0c, P.v0byc, P.m0c2);
  out << s << "\n"; 
          
  return(out);
}

// Auxiliary functions

void Particle::create()
{
     variable = new double[BEAM_DIM + 1]; 
     if(!variable)
     {
        cerr << "Particle::Particle() : allocation failure \n"; 
        exit(1);
     }  
}  
  
void Particle::initialize()
{
     for(int i=1; i <= BEAM_DIM; i++)
        variable[i] = 0.;  
}

void Particle::initialize(const Particle& P)
{
     for(int i=1; i <= BEAM_DIM; i++) 
        variable[i] = P.variable[i];
}
/*
void Particle::initialize(const Rmatrix& cR)
{
   Rmatrix R = cR;
   initialize();

   int dim = min(4, BEAM_DIM);

   double** a;
   double*  b;

   a = new double*[dim+1];
   b = new double[dim+1];

   for(int i=1; i <= dim; i++)
   {
     a[i] = new double[dim+1];
     b[i] = 0.;
     for(int j=1; j <= dim; j++)
     {
       a[i][j] = R[i][j];
       if(i == j) a[i][j] -= 1.;
     }
   }

   solveLinearEquations(a, b, dim);

   for(i=1; i <= dim; i++)
   {
      variable[i] = b[i];
      delete [] a[i];
   }  

   delete [] a;
   delete [] b;   
}

void Particle::initialize(const Twiss& Tw)
{
   Rmatrix  r   = Tw;
   Particle tmp = r;

   initialize(tmp);
}
*/
void Particle::clean()
{
     delete [] variable;
}
