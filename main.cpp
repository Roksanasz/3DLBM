#include <iostream>
#include <fstream>
#include <string>

using namespace std;
const int L=15;
float U[2][L][L][L], V[2][L][L][L], W[2][L][L][L], R[2][L][L];
int F[L][L][L];
const int ex[19] = {0,1,-1,0,0, 0,0,1,-1,-1,1, 1,1,-1,-1,0,0,0,0};
const int ey[19] = {0,0,0,1,-1, 0,0,1,1,-1,-1, 0,0,0,0,1,1,-1,-1};
const int ez[19] = {0,0,0,0,0, 1,-1,0,0,0,0, 1,-1,-1,1,1,-1,-1,1};
const int inv[19] = {0,2,1,4,3,6,5,9,10,7,8,13,14,11,12,17,18,15,16};//tablica odwrotnych kierunków
const float w[19] = {0.,1/18.,1/18.,1/18.,1/18.,1/18.,1/18.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.,1/36.};
float U0 = 0.5;



void init()
{
    for(int i=0; i<L; i++)
    {
        for(int j=0; j<L; j++)
        {
             for(int k=0; k<L; k++)
        {
            U[0][i][j][k] = V[0][i][j][k] =  W[0][i][j][k] = 0;
            U[1][i][j][k]= V[1][i][j][k] =  W[1][i][j][k] = 0;
            R[0][i][j]= R[1][i][j] = 1;
            F[i][j][k] = 0;

            if(i==0 or j==0 or i==L-1 or i==L*1/2 or k==0 or k==L-1)//tu sobie przemyśleć warunki moze k==0
                F[i][j][k] = 1;
            if(j==L-1)
               U[0][i][j][k] = U[1][i][j][k] = U0;
        }
    }
}
}

void LBMTau1 (int c)
{

    float r,u,v,w1,f;

        for(int i=0; i<L; i++)
        {
            for(int j=0; j<L-1; j++)
            {
                for(int k=0; k<L; k++)
            {
                 cout << k<< " " << j<< " " << i << endl;
                if(F[i][j][k]==0)
                {
                    U[c][i][j][k] = V[c][i][j][k] = W[c][i][j][k] = R[c][i][j] = 0;

                    for(int m = 0; m < 9; m++)
                    {
                        int ip = i+ex[m], jp = j+ey[m],kp = k+ez[m], ik = inv[m];//xp, yp - zbieramy z zewnatrz od sasiadow- skupia to co do niego zmierza

                        if(F[ip][jp][kp]==0)
                        {
                            r = R[1-c][ip][jp];
                            u = U[1-c][ip][jp][kp]/r;
                            v = V[1-c][ip][jp][kp]/r;
                            w1 = W[1-c][ip][jp][kp]/r;

                           f = w[ik]*r*(1-(3/2.)*(u*u+v*v+w1*w1)+3.*(ex[ik]*u+ey[ik]*v+ez[ik]*w1)+(9/2.)*(ex[ik]*u+ey[ik]*v+ez[ik]*w1)*(ex[ik]*u+ey[ik]*v+ez[ik]*w1));
                            //funkcja stanu w stanie rówowagi w danym wezle - dla danych z poprzedniego kroku

                        }

                        else
                            f = w[ik]*R[1-c][i][j];

                        R[c][i][j] += f;
                        U[c][i][j][k] += ex[ik]*f;
                        V[c][i][j][k] += ey[ik]*f;
                        W[c][i][j][k] += ez[ik]*f;
                    }
                    }
            }

        }

}
}



int main()
{
    init();
    for(int i=0; i<=10; i++)
    {
        LBMTau1(0);
        LBMTau1(1);
       // cout << u << " " << v << endl;
    }


 std::ofstream plik;
    plik.open ("dane3D2.vtk", std::fstream::app);
    plik<<  "# vtk DataFile Version 2.0" <<endl;
    plik << "KG NN0" <<endl;
    plik << "ASCII"<< endl;
    plik << "DATASET STRUCTURED_POINTS" << endl;
    plik << "DIMENSIONS 15 15 15" << endl;
    plik << "ORIGIN 0 0 0" << endl;
    plik << "SPACING 1 1 1" << endl;
    plik << "POINT_DATA 3750" << endl;
    plik << "VECTORS Vectors0 float" << endl;

for    (int k=0; k<L; k++)
            {
              //  cout << k << endl;
        for(int j=0; j<L; j++)
            {
              //   cout << j<<endl;
            for(int i=0; i<L; i++)
            {
               //  cout << k<< " " << j<< " " << i << endl;

                 plik << U[1][i][j][k] << " " <<  V[1][i][j][k]  << " " << W[1][i][j][k] << endl;

            }
        }
    }
 plik.close();
    return 0;

}
