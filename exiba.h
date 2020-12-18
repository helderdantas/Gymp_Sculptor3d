#ifndef EXIBA_H
#define EXIBA_H

#include <QWidget>
#include <vector>
#include <QColor>
#include <QString>
#include "conexaosculptor.h"
#include "sculptor.h"

using namespace std;

/**
 * @brief A Classe Exiba
 * E a area em que o escultor 3D sera desenhado;
 * a exibicao ocorre no plano XY e pode ser alterado ao mudar a dimensão do plano Z;
 * ao criar um escultor a area eh dividida em retangulos de acordo com as dimensoes especificadas.
 */
class Exiba : public QWidget
{
    Q_OBJECT

private:
    // Dimensões do escultor
    int nx, ny, nz;
    // Vector 3D que armazena todo o escultor
    vector<vector<vector<Voxel>>> painter_sculptor;
    // Plano atual do escultor que eh mostrado na tela
    vector<vector<Voxel>> plano_atual;
    // Ponteiro para o escultor
    Sculptor* sculptor;
    // Indices do escultor no momento de um click
    int id_plano, id_linha, id_coluna;
    // Espacamentos entre as linhas do paint
    double h_altura, h_largura;
    // Acao selecionada pelo usuario
    QString acao;

    // Caracteristicas da Caixa
    int x_caixa, y_caixa, z_caixa;
    // Caracterisca da Esfera
    int raioEsfera;
    // Caracteristica do Elipsoide
    int raioXEllipsoid, raioYEllipsoid, raioZEllipsoid;

    // Cor do desenho
    QColor cor;

    // Ativa um Voxel
    void ativaVoxel(Voxel &v, QColor cor);
    // Desativa um Voxel
    void desativaVoxel(Voxel &v);
    // Verifica se o Voxel estao dentro dos limites
    bool dentroDosLimites(int linha, int coluna, int plano);


public:
    /**
     * @brief Plotter : Construtor da classe Plotter
     */
    explicit Exiba(QWidget *parent = nullptr);
    /**
     * @brief paintEvent : metodo responsavel por desenhar o plano Z[i] escolhido pelo usuario
     * @param event : eventos relaconados ao paintEvent
     */
    void paintEvent(QPaintEvent *event);
    /**
     * @brief mousePressEvent : metodo resposavel por capturar o indice do escultor em que o mouse foi clicado e em seguida alterar as propriedades do escultor de acordo com a acao selecionada
     * @param event : eventos relacionados ao click do mouse
     */
    void mousePressEvent(QMouseEvent *event);

signals:
    /**
     * @brief alteraSlidersX : sinal emitido para alterar os sliders referentes a dimensao X do escultor. O valor emitido consiste na dimensao X que foi selecionada para o escultor.
     */
    void alteraSlidersX(int,int);
    /**
     * @brief alteraSlidersY : sinal emitido para alterar os sliders referentes a dimensao Y do escultor. O valor emitido consiste na dimensao Y que foi selecionada para o escultor.
     */
    void alteraSlidersY(int,int);
    /**
     * @brief alteraSlidersZ : sinal emitido para alterar os sliders referentes a dimensao Z do escultor. O valor emitido consiste na dimensao Z que foi selecionada para o escultor.
     */
    void alteraSlidersZ(int,int);
    /**
     * @brief alteraSlidersRaioEsfera : sinal emitido para alterar os slider referente ao raio da esfera. O valor emitido consiste na  menor dimensao que foi selecionada para o escultor.
     */
    void alteraSliderRaioEsfera(int,int);
    /**
     * @brief alteraSliderR : sinal emitido para alterar o slider referente ao Red quando uma cor eh selecionada.
     */
    void alteraSliderR(int);
    /**
     * @brief alteraSliderG : sinal emitido para alterar o slider referente ao Green quando uma cor eh selecionada.
     */
    void alteraSliderG(int);
    /**
     * @brief alteraSliderB : sinal emitido para alterar o slider referente ao Blue quando uma cor eh selecionada.

     */
    void alteraSliderB(int);


public slots:
    /**
     * @brief abreDialogEscultor : slot que abre a caixa de Dialogo do Escultor.
     */
    void abreDialogEscultor();
    /**
     * @brief alteraCor : slot que abre uma caixa de dialogo para selecionar a cor para desenhar.
     */
    void alteraCor();
    /**
     * @brief salvaEscultor : slot que abre uma caixa de dialogo para salvar o escultor no formato .off.
     */
    void salvaEscultor();
    /**
     * @brief executaGeomview : slot que abre o GeomView com o escultor atual.
     */
    void executaGeomview();
    /**
     * @brief limpaEscultor: limpa do escultor zerando todos os Voxels e mantendo as dimensões do escultor atual.
     */
    void limpaEscultor();
    /**
     * @brief mudaPlanoZ : altera o plano Z mostrado na tela de acordo com o sinal mandado pelo SliderZ.
     * @param planoZ : indice referente ao plano Z selecionado.
     */
    void mudaPlanoZ(int planoZ);
    /**
     * @brief acaoSelecionada : atualiza a acao selecionada pelo usuario para alterar o escultor.
     * @param _acao : nome da acao selecionada
     */
    void acaoSelecionada(QString _acao);
    /**
     * @brief mudaXCaixa : atualiza o numero de linhas da nova caixa a ser desenhada.
     * @param _x : numero de linhas da caixa.
     */
    void mudaXCaixa(int _x);
    /**
     * @brief mudaYCaixa : atualiza o numero de colunas da nova caixa a ser desenhada.
     * @param _y : numero de colunas da caixa.
     */
    void mudaYCaixa(int _y);
    /**
     * @brief mudaZCaixa : atualiza  o numero de planos da nova caixa a ser desenhada.
     * @param _z : numero de planos da caixa.
     */
    void mudaZCaixa(int _z);
    /**
     * @brief mudaRaioEsfera : atualiza o novo raio da esfera a ser desenhada.
     * @param _re : raio da esfera
     */
    void mudaRaioEsfera(int _re);
    /**
     * @brief mudaRaioXEllipsoid : atualiza o raio do Elipsoide na direcao X.
     * @param _rx : raio do Elipsoide na direcao X.
     */
    void mudaRaioXEllipsoid(int _rx);
    /**
     * @brief mudaRaioYEllipsoid : atualiza o raio do Elipsoide na direcao Y.
     * @param _ry : raio do Elipsoide na direcao Y.
     */
    void mudaRaioYEllipsoid(int _ry);
    /**
     * @brief mudaRaioZEllipsoid : atualiza o raio do Elipsoide na direcao Z.
     * @param _rz : raio do Elipsoide na direcao Z.
     */

    void mudaRaioZEllipsoid(int _rz);
    /**
     * @brief mudaR: atualiza a compononente vermelha(Red) da cor dos Voxels a serem desenhados.
     * @param _r : componente vermelha da cor.
     */
    void mudaR(int _r);
    /**
     * @brief mudaB : atualiza a componente azul(Blue) da cor dos Voxels a serem desenhados.
     * @param _b : componente azul da cor
     */
    void mudaB(int _b);
    /**
     * @brief mudaG: atualiza a componente verde(Green) da cor dos Voxels a serem desenhados.
     * @param _g: componente verde da cor
     */
    void mudaG(int _g);

};

#endif // PLOTTER_H
