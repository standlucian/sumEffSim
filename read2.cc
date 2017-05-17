#include <string>
#include <iostream>
using namespace std;

void read2()
{
int nfile = 0;
char part[2] = "-a";
char name[10] = "780-r039-0";
stringstream ESingle,ESingleN,ESingleNorm,
               ESingleInc,ESingleIncN,ESingleIncNorm;

double y, norm = 0.80e+6, time=1., total;
TFile* in_file=new TFile("B1.root"); 


  TH1F *h1 = (TH1F*)gDirectory->Get("1");
  total = h1->Integral();
  h1->Draw();

  ESingleNorm<<"B1vals1";
  ofstream ofile1(ESingleNorm.str().c_str(), ios::out);
  for (Int_t i = 0; i < 8192; i++)
    {
     y=h1->GetBinContent(i);
     ofile1<<y<<endl;
    }
  ofile1.close();
}
