#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//Quantum Well

#define V_0 0.64    //y range of QM Well
#define A  4     //x range of QM Well
#define DELTAE V_0/100.0      
#define EPSILONE V_0/100000.0  


double x_min, x_max;

double FunctionEven(double epsilon)
{
    double k = sqrt(2.0*(V_0+epsilon));
    double F = sin(k*A*0.5)-cos( k*A*0.5 )*sqrt(-2.0*epsilon)/k;

    return F;
}
double FunctionOdd(double epsilon)
{
    double k = sqrt(2.0*(V_0+epsilon));
    double F = sin(k*A*0.5)+cos(k*A*0.5)*k/sqrt(-2.0*epsilon);

    return F;
}
double Bisection(double xmin, double xmax, double epsilon, double (*Function)(double))
{
    double Fmax, Fmin, xcenter;
    Fmax = Function(xmin);
    Fmin = Function(xmax);

    if(Fmin == 0.0)  return xmin;
    if(Fmax == 0.0)  return xmax;

    if(Fmin*Fmax > 0.0)
    {
        cout<<"No zero point"<<endl;
    }
    else if(Fmin*Fmax == 0.0)
    {
        cout<<"Zero point = 0.0"<<endl;
    }
    else
    {
        while(xmax-xmin > epsilon)
        {
            xcenter = (xmax+xmin)/2.0;

            if(Function(xcenter) == 0.0)
                return xcenter;

            if(Function(xmin)*Function(xcenter) < 0.0)
                xmax = xcenter;
            else
                xmin = xcenter;
        }
        return ((xmin+xmax)/2.0);
    }
}
void find_energy(double epsilon, double V0, double deltaE)
{
    double E, E_zero;
    int counter;

    fstream file1, file2;
    file1.open("Fun.txt", ios::out);
    file2.open("Zero.txt", ios::out);
    file1.precision(7);
    file2.precision(7);

    E = -V_0+0.001*V_0;
    while(E<0)
    {
        file1<<E<<"   "<<FunctionEven(E)<<"   "<<FunctionOdd(E)<<"   "<<endl;
        E += deltaE;
    }
    file1.close();
    // finding zero energy points (bisection)
    E = -V_0+0.001*V_0;
    while(E<0)
    {
        if(FunctionEven(E)*FunctionEven(E+deltaE)<0)
        {
            counter++;
            E_zero = Bisection(E,E+deltaE,epsilon,FunctionEven);
            file2<<"Zero point :x"<<counter<<" "<<E_zero<<endl;
        }

        if(FunctionOdd(E)*FunctionOdd(E+deltaE)<0)
        {
            counter++;
            E_zero = Bisection(E,E+deltaE,epsilon,FunctionOdd);
            file2<<"Zero point :x"<<counter<<" "<<E_zero<< endl;
        }

        E += deltaE;
    }
    file2.close();
}
int main()
{
    //save to files and find energy
    find_energy(EPSILONE,V_0,DELTAE);

    return 0;
}
