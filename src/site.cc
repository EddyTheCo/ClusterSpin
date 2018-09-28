#include<iostream>
#include "site.hh"
#include<random>
#include"constants.hh"
#include<stack>
using namespace std;



uniform_int_distribution<int> dist(-1,0);
uniform_real_distribution<double> distd(0,1);

stack<int*> Site::arrMarks;
double Site::TEnergy=0;
double Site::totalMag=0;
int Site::numberOfSites=0;
int Site::L=0;
double Site::T=0,Site::Prob=0;
Site** Site::theLattice=NULL;
Site::Site()
{

}

Site::Site(int i,int j):mark(0),i(i),j(j)
{
    numberOfSites+=1;
    spin=dist(Constants::mt);
    if(spin==0)
        spin=1;


}
Site::~Site()
{
	
	
}
void Site::setLattice(Site** lattice,int L1,double T1)
{
    theLattice=lattice;
    L=L1;
    T=T1;
    Prob=1-exp(-2./T);
}
void Site::setPtr(void)
{

    left=&theLattice[(i-1+L)%L][j];
    right=&theLattice[(i+1+L)%L][j];
    down=&theLattice[i][(j-1+L)%L];
    up=&theLattice[i][(j+1+L)%L];
}

void Site::calculateEnergy()
{

}
void Site::totalEnergy()
{

    energy= -1*spin*(left->spin+right->spin+down->spin+up->spin);

    dE=-2*energy;


    TEnergy+=energy/2;


}

int Site::changeConf()
{
int in=0;
int var=0;
mark=1;
arrMarks.push(&(this->mark));
    spin*=-1;
    if(!(left->mark))
        if(left->spin!=spin&&(Prob)>distd(Constants::mt))
        {
            var+=left->changeConf();
            in=1;
        }

    if(!(right->mark))
        if(right->spin!=spin&&(Prob)>distd(Constants::mt))
        {
            var+=right->changeConf();
            in=1;
        }
    if(!(down->mark))
        if(down->spin!=spin&&(Prob)>distd(Constants::mt))
        {
            var+=down->changeConf();
            in=1;
        }
    if(!(up->mark))
        if(up->spin!=spin&&(Prob)>distd(Constants::mt))
        {
            var+=up->changeConf();
            in=1;
        }


    if(!(left->mark))
    {
        if(left->spin==spin)
        {
            TEnergy+=2;
        }
        else
        {
            TEnergy+=-2;
        }
    }
    if(!(right->mark))
    {
        if(right->spin==spin)
        {
            TEnergy+=2;
        }
        else
        {
            TEnergy+=-2;
        }
    }
    if(!(down->mark))
     {
        if(down->spin==spin)
        {
            TEnergy+=2;
        }
        else
        {
            TEnergy+=-2;
        }
    }
    if(!(up->mark))
    {
        if(up->spin==spin)
        {
            TEnergy+=2;
        }
        else
        {
             TEnergy+=-2;
        }
    }

if(in)
    return var;
else
    return 1;



}
void Site::resetMark()
{
cout<<arrMarks.size()<<endl;
    while(!arrMarks.empty())
    {
        *(arrMarks.top())=0;

        arrMarks.pop();
    }
}
