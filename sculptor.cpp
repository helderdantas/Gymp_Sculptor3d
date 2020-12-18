#include "sculptor.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;


// Construtor da classe Sculptor
Sculptor::Sculptor(int _nx, int _ny, int _nz){
    nx = _nx;
    ny = _ny;
    nz = _nz;
    // Verifica se as quantidades de linhas, colunas e planos sao positivas
    if (nx <= 0 || ny <= 0|| nz <= 0){
        nx = ny = nz = 0;
    }
    // Solicita um bloco de memoria com a quantidade de planos
    voxel = new Voxel**[nz];
    // Verifica se o bloco foi armazenado
    if(voxel == nullptr){
        exit(0);
    }
    // Solicita um bloco que armazena o indice as linhas dos planos
    voxel[0] = new Voxel*[nz*nx];
    // Verifica se o bloco foi armazenado
    if(voxel[0] == nullptr){
        exit(0);
    }

    // Distribui os planos
    for (int k=1;k<nz;k++){
        voxel[k] = voxel[k-1] + nx;
    }
    // Solicita um bloco de memoria que armazena todos os voxels na matriz 3D
    voxel[0][0] = new Voxel[nz*nx*ny];
    // Verifica se o bloco foi armazenado
    if(voxel[0][0] == nullptr){
        exit(0);
    }
    //Distribui as colunas entre as linhas e os planos
    int x = 0;
    for (int k=0; k<nz;k++){
        for(int i=0; i<nx;i++){
            voxel[k][i] = voxel[0][0] + x*ny;
            x++;
        }
    }
    constroiMatriz3D();

}

// Destrutor da classe Sculptor
Sculptor::~Sculptor(){
    if (nx == 0 || ny == 0|| nz == 0){
        return;
    }
    delete [] voxel[0][0];
    delete [] voxel[0];
    delete [] voxel;
}

// Define a cor atual do desenho
void Sculptor::setColor(float _r, float _g, float _b, float alpha){
    r = _r;
    g = _g;
    b = _b;
    a = alpha;
}

// Ativa o voxel na posição (x,y,z) (fazendo isOn = true) e atribui ao mesmo a cor atual de desenho
void Sculptor::putVoxel(int x, int y, int z){
    if(elementoForaDaMatriz(x, y, z) == true){ // verifica se há algum elemento fora dos limites da matriz
        voxel[z][x][y].isOn = true;
        voxel[z][x][y].r = r;
        voxel[z][x][y].g = g;
        voxel[z][x][y].b = b;
        voxel[z][x][y].a = a;
    }

}

//Desativa o voxel na posição (x,y,z) (fazendo isOn = false)
void Sculptor::cutVoxel(int x, int y, int z){
    if(elementoForaDaMatriz(x, y, z) == true){ // verificando se o usuário não está acessando algum elemento da matriz que não existe
        voxel[z][x][y].isOn = false;
    }
}

// Ativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1] e atribui aos mesmos a cor atual de desenho
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
    int i,j,k;
    for (k=z0; k<=z1; k++){
        for (i=x0; i<=x1; i++) {
            for (j=y0; j<=y1; j++) {
                putVoxel(i,j,k);
            }
        }
    }
}

// Desativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1] e atribui aos mesmos a cor atual de desenho
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1){
    int i,j,k;
    for (k=z0; k<=z1; k++){
        for (i=x0; i<=x1; i++) {
            for (j=y0; j<=y1; j++) {
                cutVoxel(i,j,k);
            }
        }
    }
}

//Ativa todos os voxels que satisfazem à equação da esfera e atribui aos mesmos a cor atual de desenho
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
    int i,j,k;
    double dist;
    for(k=0; k<nz; k++){
        for (i=0; i<nx; i++) {
            for (j=0; j<ny; j++){
               dist = pow(i-xcenter,2) + pow(j-ycenter,2) + pow(k-zcenter,2);
               if (dist <= pow(radius,2)){
                   putVoxel(i,j,k);
               }
            }

        }
    }
}

//Desativa todos os voxels que satisfazem à equação da esfera
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    int i,j,k;
    double dist;
    for(k=0; k<nz; k++){
        for (i=0; i<nx; i++) {
            for (j=0; j<ny; j++){
               dist = pow(i-xcenter,2) + pow(j-ycenter,2) + pow(k-zcenter,2);
               if (dist <= pow(radius,2)){
                   cutVoxel(i,j,k);
               }
            }

        }
    }
}

//Ativa todos os voxels que satisfazem à equação do elipsóide e atribui aos mesmos a cor atual de desenho
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    int i,j,k;
    double dist;
    if (rx ==0){
        for (k=0;k<nz;k++) {
            for (j=0;j<ny;j++) {
                dist =  pow(j-ycenter,2)/pow(ry,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    putVoxel(xcenter,j,k);
              }
            }
        }
    }
    else if(ry==0){
        for (k=0;k<nz;k++) {
            for (i=0;i<nx;i++) {
                dist =  pow(i-xcenter,2)/pow(rx,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    putVoxel(i,ycenter,k);
              }
            }
        }
    }
    else if (rz==0) {
        for (i=0;i<nx;i++) {
            for (j=0;j<ny;j++) {
                dist =  pow(i-xcenter,2)/pow(rx,2) + pow(j-ycenter,2)/pow(ry,2);
                if(dist<=1){
                    putVoxel(i,j,zcenter);
              }
            }
        }
    }
    else{
    for (k=0;k<nz;k++) {
        for (i=0;i<nx;i++) {
            for (j=0;j<ny;j++) {
                  dist = pow(i-xcenter,2)/pow(rx,2) + pow(j-ycenter,2)/pow(ry,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    putVoxel(i,j,k);
                }

            }

        }
      }
    }
}

// Desativa todos os voxels que satisfazem à equação do elipsóide
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    int i,j,k;
    double dist;
    if (rx ==0){
        for (k=0;k<nz;k++) {
            for (j=0;j<ny;j++) {
                dist =  pow(j-ycenter,2)/pow(ry,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    cutVoxel(xcenter,j,k);
              }
            }
        }
    }
    else if(ry==0){
        for (k=0;k<nz;k++) {
            for (i=0;i<nx;i++) {
                dist =  pow(i-xcenter,2)/pow(rx,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    cutVoxel(i,ycenter,k);
              }
            }
        }
    }
    else if (rz==0) {
        for (i=0;i<nx;i++) {
            for (j=0;j<ny;j++) {
                dist =  pow(i-xcenter,2)/pow(rx,2) + pow(j-ycenter,2)/pow(ry,2);
                if(dist<=1){
                    cutVoxel(i,j,zcenter);
              }
            }
        }
    }
    else{
    for (k=0;k<nz;k++) {
        for (i=0;i<nx;i++) {
            for (j=0;j<ny;j++) {
                  dist = pow(i-xcenter,2)/pow(rx,2) + pow(j-ycenter,2)/pow(ry,2) + pow(k-zcenter,2)/pow(rz,2);
                if(dist<=1){
                    cutVoxel(i,j,k);
                }

            }

        }
    }
    }
}
//grava a escultura no formato OFF no arquivo filename
void Sculptor::writeOFF(string filename){
    ofstream fout;
    string pontos, faces;
    int contador;

  //  otimizar();
    // Definindo os pesos para desenhar os cubos
    vector<vector<float> > pesos;
    vector<float> v1;
    v1 = {-0.5, 0.5, -0.5};
    pesos.push_back(v1);
    v1 = {-0.5, -0.5, -0.5};
    pesos.push_back(v1);
    v1 = {0.5,- 0.5, -0.5};
    pesos.push_back(v1);
    v1 = {0.5, 0.5, -0.5};
    pesos.push_back(v1);
    v1 = {-0.5, 0.5, 0.5};
    pesos.push_back(v1);
    v1 = {-0.5, -0.5, 0.5};
    pesos.push_back(v1);
    v1 = {0.5, -0.5, 0.5};
    pesos.push_back(v1);
    v1 = {0.5, 0.5, 0.5};
    pesos.push_back(v1);

    // Definindo a sequencia inicial para as faces
    vector<vector<int> > pontos_faces;
    vector<int> v2;
    v2 = {0, 3, 2, 1};
    pontos_faces.push_back(v2);
    v2 = {4, 5, 6, 7};
    pontos_faces.push_back(v2);
    v2 = {0, 1, 5, 4};
    pontos_faces.push_back(v2);
    v2 = {0, 4, 7, 3};
    pontos_faces.push_back(v2);
    v2 = {3, 7, 6, 2};
    pontos_faces.push_back(v2);
    v2 = {1, 2, 6, 5};
    pontos_faces.push_back(v2);


    //Abre o arquivo
    fout.open(filename);
    // Verifica se o arquivo foi aberto corretamente
    if(fout.is_open()){
        cout << "Arquivo OFF aberto com sucesso" << endl;
    }
    else{
        cout << "Nao foi possivel abrir o arquivo OFF"<< endl;
        exit(0);
    }
    // Inicializa as string
    pontos = "";
    faces = "";
    contador = 0;
    // Configurando para cada voxel ser representado como um cubo de aresta igual a 1
    int i,j,k;
    unsigned int a, t;
    for (k=0;k<nz;k++) {
        for (i=0;i<nx;i++) {
            for(int j=0;j<ny;j++){
                if(voxel[k][i][j].isOn == true){
                    vector<int> coord;
                    coord = {j,-i,-k};

                    for (a=0;a<8;a++) {
                        stringstream ponto;
                        for(t=0;t<3;t++){
                            ponto << fixed << setprecision(1) << coord[t] + pesos[a][t] << " ";
                        }
                        ponto << endl;
                        pontos = pontos + ponto.str();
                    }
                    for (a=0;a<6;a++) {
                        stringstream face;
                        face << 4 << " ";
                        for(t=0;t<4;t++){
                            face << contador*8 + pontos_faces[a][t] << " ";
                        }
                        face << fixed << setprecision(1) << voxel[k][i][j].r << " "<< voxel[k][i][j].g << " "<< voxel[k][i][j].b << " " << voxel[k][i][j].a <<endl;
                        faces = faces + face.str();
                    }

                    contador++;

                }
            }

        }

    }
    //Configurando o arquivo OFF
    fout << "OFF"<<endl;
    fout << contador*8 << " " << contador*6 << " " << 0 <<endl;
    fout << pontos;
    fout << faces;

    //Fecha o arquivo
    fout.close();
}

//Funcoes Auxiliares
// consulta se há algum elemento fora da matriz do sculptor
bool Sculptor::elementoForaDaMatriz(int x, int y, int z){
    if (x>=nx || y>=ny || z>=nz || x<0 || y<0 || z<0){
       cout << "o elemento (" << x << "," << y << "," << z << ") não pertence ao sculptor \n";
       return false;
    }
    return true;
}


// Constroi a matriz 3D com os campos dos voxels r,g,b = 0 e isOn=false
void Sculptor::constroiMatriz3D(){
    int i,j,k;
    Voxel v;
    // Voxel Genérico
    v.r = v.g = v.b =0;
    v.a = 0;
    v.isOn = false;

    for(k=0; k<nz; k++){
        for(i=0; i<nx; i++){
            for (j=0; j<ny; j++) {
                    voxel[k][i][j] = v;
            }
        }
    }
}

// Imprime o conteuduo do Escultor
void Sculptor::print_sculptor(){
    for(int k=0; k<nz; k++){
        cout << "Plano " << k << endl;
        for(int i=0; i<nx; i++){
            for (int j=0; j<ny; j++) {
                    cout << voxel[k][i][j].isOn << " ";

            }
            cout << endl;
        }
        cout << "---------------------------" << endl;
    }

}


