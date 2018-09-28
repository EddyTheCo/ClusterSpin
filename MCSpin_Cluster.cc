#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include<vector>
#include <random>
#include"constants.hh"
#include "site.hh"
#include"myTGraph.hh"
#include<TH1D.h>
#include<stack>
using namespace std;

TH1* h1 = new TH1I("h1", "h1 title", 30, 1, 15);
int main()
{
void crearMuestra(int , Site**,double);
double makeMeasur(Site**,double);
double calculateStd(stack <double> ,double);
Site** lattice;
int l,binsconst=1  ;
double aver,T;
vector <double> theMeas,theLeng,theMeasErr;
stack <double> errorSamp;


cout<<"key in the temperature"<<endl;
cin>>T;
char title[20];
sprintf(title,"Temp= %.2f K",T);



    for ( l=8;l<140;l+=10)
    {
        aver=0;
        for(int bins=0;bins<binsconst;bins++)
        {
            lattice=new Site*[l];
            for(int i=0;i<l;i++)
            {
                lattice[i]=new Site[l];
                for(int j=0;j<l;j++)
                {
                    lattice[i][j]=Site(i,j);
                }
            }

            crearMuestra(l,lattice,T);
            //cout<<lattice[0][0].TEnergy<<endl;
            double samp=makeMeasur(lattice,l);
            aver+=samp;
            errorSamp.push(samp);
       }
        //cout<<aver/binsconst<<endl;
        theMeas.push_back(aver/binsconst);
        theMeasErr.push_back(calculateStd(errorSamp,aver/binsconst));
        theLeng.push_back(l);

    }

        MyTGraph * he=new MyTGraph(theMeas.size(),&theLeng[0],&theMeas[0],0,0,title,T);

        TCanvas *c1 = new TCanvas("c1","Root",200,100,900,600);
        c1->SetLogy();
        h1->SetTitle(" ");
        h1->Scale(100/h1->GetEntries());
        h1->SetStats(kFALSE);
        h1->GetXaxis()->SetTitle("b");
        h1->GetYaxis()->SetTitle("%");
        h1->Draw("E1");
        c1->Update();
        c1->GetFrame()->SetFillColor(21);
        c1->GetFrame()->SetBorderSize(12);
        c1->Modified();
        c1->SaveAs("picture2.jpg");






}


void crearMuestra(int l,Site** lattice,double T)
{
    uniform_int_distribution<int> dist2(0,l-1);

    lattice[0][0].setLattice(lattice,l,T);
    lattice[0][0].TEnergy=0;
    int Number_equi=1000000;
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<l;j++)
        {
            lattice[i][j].setPtr();
            lattice[i][j].totalEnergy();

        }
    }
//cout<<lattice[0][0].TEnergy<<endl;
    for(int steps=0;steps<Number_equi;steps++)
    {
      int myvar= lattice[dist2(Constants::mt)][dist2(Constants::mt)].changeConf();
      if(l==128)
      {
        h1->Fill(log2(myvar)+1);
        //cout<<myvar<<endl;
      }
       lattice[0][0].resetMark();
    }
//cout<<lattice[0][0].TEnergy<<endl;
}

double makeMeasur(Site** lattice,double l)
{
    vector <double> energy;
    uniform_int_distribution<int> dist2(0,l-1);
    int N_sweeps = 10000000;
    int N_flips = 1000;
    int N_meas=N_sweeps/N_flips;
    double E=0;

//cout<<lattice[0][0].TEnergy<<endl;
    for(int step=0;step<N_sweeps;step++)
    {
        int myvar=lattice[dist2(Constants::mt)][dist2(Constants::mt)].changeConf();
        if(l==128)
        {
            h1->Fill(myvar);
           // cout<<myvar<<endl;
        }
        lattice[0][0].resetMark();
        if(step%N_flips==0)
        {
            //cout<<lattice[0][0].TEnergy<<endl;
            E+=lattice[0][0].TEnergy;

            energy.push_back(lattice[0][0].TEnergy);
        }
    }
    E=E/(N_meas);
    //cout<<E<<endl;
    for(int k=0;k<N_meas;k++)
    {
        energy[k]-=E;

    }

    double gamma=0;
    double gamma2=12340;

    for(int j=0;j<N_meas;j++)
    {

        double varA2=0,varA=0;
        for(int i=0;i<N_meas-j;i++)
        {
            varA2+=energy[i]*energy[i];

            varA+=energy[i]*energy[i+j];
        }
        gamma+=varA/varA2;

        if(abs(gamma2-gamma)>0.0001&&gamma>0)
        {
            gamma2=gamma;
        }
        else
        {
            //cout<<gamma2<<endl;

            break;
        }

   // cout<<j<<" "<<varA/varA2<<" "<<gamma<<endl;



     }



    //cout<<gamma+0.5<<endl;
    return gamma+0.5;

}
double calculateStd(stack<double> vect, double ave)
{
    double sum=0;
    int k=vect.size();
    while(!vect.empty())
    {
        sum+=pow(vect.top()-ave,2);
        vect.pop();
    }
    return sqrt(sum/k);

}
