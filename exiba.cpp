#include "exiba.h"
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QMouseEvent>
#include <QColorDialog>
#include <algorithm>
#include <QDebug>
#include <math.h>
#include<QFileDialog>
#include<QMessageBox>

#include<stdlib.h>
#include<iostream>
using namespace std;



Exiba::Exiba(QWidget *parent) : QWidget(parent)
{
    // Dimensões do Escultor
    nx = ny = nz = 0;
    // Instanciando um escultor zerado
    sculptor = new Sculptor(nx,ny,nz);
    // Indices do escultor no momento de um click
    id_plano = id_linha = id_coluna = 0;
    // Espacamentos entre as linhas do paint
    h_largura = h_altura = 0;
    // Acao Selecionada
    acao = " ";
    // Caraceristicas da Caixa
    x_caixa = y_caixa = z_caixa = 0;
    // Caracteristica da Esfera
    raioEsfera = 0;
    // Caracteristica do Elipsoide
    raioXEllipsoid = raioYEllipsoid = raioZEllipsoid = 0;
    // Cor do desenho
    cor = QColor(0,0,0,255);

}

void Exiba::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;

    painter.setRenderHint(QPainter::Antialiasing);

    // Configurando o preenchimento da area de desenho
    brush.setColor(QColor(255,255,255));
    brush.setStyle(Qt::SolidPattern);

    // Configrando o Contorno da área de desenho
    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);

    // Area de desenho
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height());

    // Desenhando o gradeado para o escultor
    if(nx !=0 && ny!=0 && nz !=0){
         h_altura = (double)height()/nx;
         h_largura = (double)width()/ny;

         pen.setWidth(2);
         painter.setPen(pen);

         for (int i=1;i<=nx;i++){
            painter.drawLine(0,i*h_altura,width(),i*h_altura);
        }

         for (int i=1;i<=ny;i++){
            painter.drawLine(i*h_largura,0,i*h_largura,height());
        }

    }

    // Atualizando o plano com os Voxels que foram ativados
    brush.setColor(QColor(0,0,0));
    painter.setBrush(brush);
    for (int i=0;i<nx;i++) {
        for(int j=0; j<ny; j++){
            if (plano_atual[i][j].isOn == true){
                qDebug() << "entrou";
                int pos_linha = i*h_altura;
                int pos_coluna = j*h_largura;

                brush.setColor(QColor(211, 215, 207));
                painter.setBrush(brush);
                painter.drawRect(pos_coluna,pos_linha,h_largura,h_altura);

                Voxel aux = plano_atual[i][j];
                brush.setColor(QColor(aux.r,aux.g,aux.b,aux.a));
                painter.setBrush(brush);
                painter.drawEllipse(pos_coluna,pos_linha,h_largura,h_altura);
            }
        }

    }


}

void Exiba::mousePressEvent(QMouseEvent *event)
{

    if(nx !=0 && ny !=0 && nz !=0){
        if(event->button() == Qt::LeftButton){

            // Posicao do Click
            int pos_coluna = event->x();
            int pos_linha = event->y();

            // Posição na Vector do Escultor
            id_linha = pos_linha/h_altura;
            id_coluna = pos_coluna/h_largura;

            //QColor cor(0,0,0,255);

            sculptor->setColor(cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f,cor.alpha()/255.0f);

            if(acao.compare("PutVoxel",Qt::CaseInsensitive) == 0){
                if (dentroDosLimites(id_linha,id_coluna,id_plano)){
                    ativaVoxel(painter_sculptor[id_plano][id_linha][id_coluna],cor);
                    sculptor->putVoxel(id_linha,id_coluna,id_plano);
                }
                //sculptor->print_sculptor();
            }

            else if (acao.compare("CutVoxel",Qt::CaseInsensitive) == 0) {
                if (dentroDosLimites(id_linha,id_coluna,id_plano)){
                    desativaVoxel(painter_sculptor[id_plano][id_linha][id_coluna]);
                    sculptor->cutVoxel(id_linha,id_coluna,id_plano);
                }
            }
            else if (acao.compare("PutBox",Qt::CaseInsensitive) == 0) {
                for(int k=0; k<z_caixa;k++){
                    for(int i=0;i<x_caixa;i++){
                        for(int j=0; j<y_caixa;j++){
                            if (dentroDosLimites(id_linha+i,id_coluna+j,id_plano+k)){
                                ativaVoxel(painter_sculptor[id_plano+k][id_linha+i][id_coluna+j],cor);
                                sculptor->putVoxel(id_linha+i,id_coluna+j,id_plano+k);
                            }
                        }
                    }
                }
            }

            else if (acao.compare("CutBox",Qt::CaseInsensitive) == 0) {
                for(int k=0; k<z_caixa;k++){
                    for(int i=0;i<x_caixa;i++){
                        for(int j=0; j<y_caixa;j++){
                            if (dentroDosLimites(id_linha+i,id_coluna+j,id_plano+k)){
                                desativaVoxel(painter_sculptor[id_plano+k][id_linha+i][id_coluna+j]);
                                sculptor->cutVoxel(id_linha+i,id_coluna+j,id_plano+k);
                            }
                        }
                    }
                }
            }

            else if (acao.compare("PutSphere",Qt::CaseInsensitive) == 0) {
                double dist;
                for(int k=id_plano - raioEsfera; k<=id_plano + raioEsfera; k++){
                    for (int i=id_linha - raioEsfera; i<=id_linha + raioEsfera; i++) {
                        for (int j=id_coluna - raioEsfera; j<=id_coluna + raioEsfera; j++){
                           dist = pow(i-id_linha,2) + pow(j-id_coluna,2) + pow(k-id_plano,2);
                           if (dist <= pow(raioEsfera,2)){
                               if (dentroDosLimites(i,j,k)){
                                   ativaVoxel(painter_sculptor[k][i][j],cor);
                                   sculptor->putVoxel(i,j,k);
                               }
                           }
                        }
                    }
                }
            }

            else if (acao.compare("CutSphere",Qt::CaseInsensitive) == 0) {
                double dist;
                for(int k=id_plano - raioEsfera; k<=id_plano + raioEsfera; k++){
                    for (int i=id_linha - raioEsfera; i<=id_linha + raioEsfera; i++) {
                        for (int j=id_coluna - raioEsfera; j<=id_coluna + raioEsfera; j++){
                           dist = pow(i-id_linha,2) + pow(j-id_coluna,2) + pow(k-id_plano,2);
                           if (dist <= pow(raioEsfera,2)){
                               if (dentroDosLimites(i,j,k)){
                                   desativaVoxel(painter_sculptor[k][i][j]);
                                   sculptor->cutVoxel(i,j,k);
                               }
                           }
                        }
                    }
                }
            }

            else if (acao.compare("PutEllipsoid", Qt::CaseInsensitive)==0) {
                double dist;

                if (raioXEllipsoid ==0){
                    for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                            dist =  pow(j-id_coluna,2)/pow(raioYEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(id_linha,j,k)){
                                    ativaVoxel(painter_sculptor[k][id_linha][j],cor);
                                    sculptor->putVoxel(id_linha,j,k);
                                }
                                //t.putVoxel(id_linha,j,k);
                          }
                        }
                    }
                }

                else if(raioYEllipsoid==0){
                    for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                        for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                            dist =  pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,id_coluna,k)){
                                    ativaVoxel(painter_sculptor[k][i][id_coluna],cor);
                                    sculptor->putVoxel(i,id_coluna,k);
                                }
                                //t.putVoxel(i,id_coluna,k);
                          }
                        }
                    }
                }
                else if (raioZEllipsoid==0) {
                    for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                            dist =  pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(j-id_coluna,2)/pow(raioYEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,j,id_plano)){
                                    ativaVoxel(painter_sculptor[id_plano][i][j],cor);
                                    sculptor->putVoxel(i,j,id_plano);
                                }
                                //t.putVoxel(i,j,id_plano);
                          }
                        }
                    }
                }
                else{
                for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                    for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                              dist = pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(j-id_coluna,2)/pow(raioYEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,j,k)){
                                    ativaVoxel(painter_sculptor[k][i][j],cor);
                                    sculptor->putVoxel(i,j,k);
                                }
                                //t.putVoxel(i,j,k);
                            }

                        }

                    }
                 }
              }


            }

            else if (acao.compare("CutEllipsoid", Qt::CaseInsensitive)==0) {
                double dist;

                if (raioXEllipsoid ==0){
                    for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                            dist =  pow(j-id_coluna,2)/pow(raioYEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(id_linha,j,k)){
                                    desativaVoxel(painter_sculptor[k][id_linha][j]);
                                    sculptor->cutVoxel(id_linha,j,k);
                                }
                                //t.putVoxel(id_linha,j,k);
                          }
                        }
                    }
                }

                else if(raioYEllipsoid==0){
                    for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                        for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                            dist =  pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,id_coluna,k)){
                                    desativaVoxel(painter_sculptor[k][i][id_coluna]);
                                    sculptor->cutVoxel(i,id_coluna,k);
                                }
                                //t.putVoxel(i,id_coluna,k);
                          }
                        }
                    }
                }
                else if (raioZEllipsoid==0) {
                    for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                            dist =  pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(j-id_coluna,2)/pow(raioYEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,j,id_plano)){
                                    desativaVoxel(painter_sculptor[id_plano][i][j]);
                                    sculptor->cutVoxel(i,j,id_plano);
                                }
                                //t.putVoxel(i,j,id_plano);
                          }
                        }
                    }
                }
                else{
                for (int k=id_plano - raioZEllipsoid;k<=id_plano + raioZEllipsoid;k++) {
                    for (int i=id_linha - raioXEllipsoid;i<=id_linha + raioXEllipsoid;i++) {
                        for (int j=id_coluna - raioYEllipsoid;j<=id_coluna + raioYEllipsoid;j++) {
                              dist = pow(i-id_linha,2)/pow(raioXEllipsoid,2) + pow(j-id_coluna,2)/pow(raioYEllipsoid,2) + pow(k-id_plano,2)/pow(raioZEllipsoid,2);
                            if(dist<=1){
                                if (dentroDosLimites(i,j,k)){
                                    desativaVoxel(painter_sculptor[k][i][j]);
                                    sculptor->cutVoxel(i,j,k);
                                }
                                //t.putVoxel(i,j,k);
                            }

                        }

                    }
                 }
              }

            }



            qDebug() << "Pos Plano: " << id_plano;
            qDebug() << "Pos Linha: " << id_linha;
            qDebug() << "Pos Coluna: " << id_coluna;

            plano_atual = painter_sculptor[id_plano];
            repaint();

        }
    }

}

void Exiba::abreDialogEscultor()
{
    ConexaoSculptor e;
    if(e.exec() == QDialog::Accepted){
        // Pegando as dimensões do escultor
        nx = e.getNumDeLinhas();
        ny = e.getNumDeColunas();
        nz = e.getNumDePlanos();
        if(nx !=0 && ny !=0 && nz !=0){
            // Voxel com todas as proprieades iguais a zero
            Voxel voxel_zero;
            voxel_zero.r = voxel_zero.b = voxel_zero.g = 0;
            voxel_zero.a = 0.0f;
            voxel_zero.isOn = false;

            painter_sculptor.clear();
            // Craindo o escultor 3D com Voxels zerados
            for (int k=0;k<nz;k++) {
                vector<vector<Voxel>> aux1;
                for(int i=0;i<nx; i++){
                    vector<Voxel> aux2;
                    for(int j=0;j<ny; j++){
                        aux2.push_back(voxel_zero);
                    }
                    aux1.push_back(aux2);
                }
                painter_sculptor.push_back(aux1);
            }

            // Definido como a primeira tela de desenho o plano zero(XY)
            plano_atual = painter_sculptor[0];

            // Removendo o escultor anterior anterior
            delete sculptor;
            // Instanciando o escultor atual
            sculptor = new Sculptor(nx,ny,nz);
            sculptor->print_sculptor();

            // Redefinindo as propriedades dos sliders (emitindo sinais para mainwindow)
            emit alteraSlidersX(0,nx-1);
            emit alteraSlidersY(0,ny-1);
            emit alteraSlidersZ(0,nz-1);

            int re[] = {nx-1,nz-1,ny-1};
            emit alteraSliderRaioEsfera(0,*min_element(re,re+3));

            qDebug() << "Num Linhas: " << nx;
            qDebug() << "Num Colunas: " << ny;
            qDebug() << "Num Planos: " << nz;

            repaint();
            }
        else {
            nx = nz = ny = 0;
            QMessageBox box;
            box.setText("Dimensoes Invalidas: Todas as dimensoes devem ser diferentes de zero!!");
            box.exec();
        }
    }
}

void Exiba::alteraCor()
{
    QColor c;
    QColorDialog dColor;
    dColor.exec();
    c = dColor.selectedColor();

    // Atualizando a cor dos Voxels
    cor.setRed(c.red());
    cor.setGreen(c.green());
    cor.setBlue(c.blue());
    cor.setAlpha(255);

    //Atualizando os Sliders das Cores
    emit alteraSliderR(c.red());
    emit alteraSliderG(c.green());
    emit alteraSliderB(c.blue());




}

void Exiba::salvaEscultor()
{
  if (nx != 0 && ny !=0 && nz !=0){
   QString fileName = QFileDialog::getSaveFileName(this, tr("Salve o Escultor em formato .off"),"",tr("(*.off);;All Files (*)"));
   if (fileName.compare("")){
    sculptor->writeOFF(fileName.toStdString());
   }
  }
  else {
      QMessageBox box;
      box.setText("O Escultor está vazio, não é possível salvar o arquivo .off");
      box.exec();
  }

}

void Exiba::executaGeomview()
{
    if(nx !=0 && ny !=0 && nz !=0){
        std::string fileName = "\Pictures\build-Paint_Sculptor-Desktop_Qt_5_15_1_MinGW_32_bit-Debug\e.of";
        sculptor->writeOFF(fileName);
        std::string comando = "geomview "+ fileName;
        std::system(comando.c_str());

    }

}

void Exiba::limpaEscultor()
{
    if(nx !=0 && ny !=0 && nz !=0){
        // Voxel com todas as proprieades iguais a zero
        Voxel voxel_zero;
        voxel_zero.r = voxel_zero.b = voxel_zero.g = 0;
        voxel_zero.a = 0.0f;
        voxel_zero.isOn = false;

        painter_sculptor.clear();
        // Craindo o escultor 3D com Voxels zerados
        for (int k=0;k<nz;k++) {
            vector<vector<Voxel>> aux1;
            for(int i=0;i<nx; i++){
                vector<Voxel> aux2;
                for(int j=0;j<ny; j++){
                    aux2.push_back(voxel_zero);
                }
                aux1.push_back(aux2);
            }
            painter_sculptor.push_back(aux1);
        }

        plano_atual = painter_sculptor[id_plano];
         // Removendo o escultor anterior anterior
        delete sculptor;
        // Instanciando o escultor atual
        sculptor = new Sculptor(nx,ny,nz);
        repaint();

    }
    else {
        QMessageBox box;
        box.setText("O escultor nao foi inicializado!!");
        box.exec();
    }
}

void Exiba::mudaPlanoZ(int planoZ)
{
    id_plano = planoZ;
    plano_atual = painter_sculptor[id_plano];
    repaint();
}

void Exiba::acaoSelecionada(QString _acao)
{
    acao = _acao;
    qDebug() << "Acao Selecionada " << acao;
}

void Exiba::mudaXCaixa(int _x)
{
    x_caixa = _x;
}

void Exiba::mudaYCaixa(int _y)
{
    y_caixa = _y;
}

void Exiba::mudaZCaixa(int _z)
{
    z_caixa = _z;
}

void Exiba::mudaRaioEsfera(int _re)
{
    raioEsfera = _re;
}

void Exiba::mudaRaioXEllipsoid(int _rx)
{
    raioXEllipsoid = _rx;
}

void Exiba::mudaRaioYEllipsoid(int _ry)
{
    raioYEllipsoid = _ry;
}

void Exiba::mudaRaioZEllipsoid(int _rz)
{
    raioZEllipsoid = _rz;
}

void Exiba::mudaR(int _r)
{
    cor.setRed(_r);
}

void Exiba::mudaB(int _b)
{
    cor.setBlue(_b);
}

void Exiba::mudaG(int _g)
{
    cor.setGreen(_g);
}

void Exiba::ativaVoxel(Voxel &v, QColor cor)
{
    v.isOn = true;
    v.r = cor.red();
    v.g = cor.green();
    v.b = cor.blue();
    v.a = cor.alpha();
}

void Exiba::desativaVoxel(Voxel &v)
{
    v.isOn = false;
}

bool Exiba::dentroDosLimites(int linha, int coluna, int plano)
{
    if ((plano < nz && plano >= 0) && (linha < nx && linha >= 0) && (coluna < ny && coluna >=0)){
        return true;
    }

    return false;
}
