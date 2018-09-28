#include <cstdlib>
#include<stack>


using namespace std;

#ifndef Site_h
#define Site_h 1


class Site
{
	public:
    Site();
    Site(int, int);
    void calculateEnergy(void);
    void totalEnergy(void);
    void resetMark(void);
    static stack<int*> arrMarks;
    int changeConf(void);
    ~Site();
    static int numberOfSites;
    static double T;
    static double TEnergy,totalMag,Prob;
    int spin,mark;
    static int L;
    void setPtr(void);
    void setLattice(Site** lattice,int L1,double T1);
	private:


    static Site** theLattice;
    Site* left;
    Site* right;
    Site* up;
    Site* down;
    double energy,dE;
    int i,j;


};

#endif
