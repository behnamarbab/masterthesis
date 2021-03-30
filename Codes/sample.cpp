#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
using namespace std;

#define pi M_PI
#define XTEND 1000

long long non_important_counter = 0;

double normalize(double d, double mx=1e9, double l=15){
    return d/mx*l;
}

long long f1(double x){
    double res = pow(cos(2*x-pi/2), 3);
    res += 2*sin(x/2);
    return (long long)abs(res*XTEND);
}

long long f2(double x){
    double res = pow(cos(2*(x-2)-pi/3), 3);
    res += 2*sin((3*x+4)/2);
    return (long long)abs(res*XTEND);
}

double parse1(char *str) {
    
    return 1;
}

double parse2(char *str) {
    return 2;
}

int do_nothing() {
    non_important_counter ++;
}

int main(int argc, char** argv) {
    ifstream infile;
    if(argc<2) {
        printf("You must provide the inputfile");
        return 0;
    }

    infile.open(argv[1]);

    long long y1, y2;
    y1 = f1(0);
    y2 = f2(0);

    for(long long i = 0; i<y1; i++){
        for(long long j = 0; j<y2; j++){
            do_nothing();
        }
    }

    cout << "Number of executions " << non_important_counter << endl;

    return 0;
}