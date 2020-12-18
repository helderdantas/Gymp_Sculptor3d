#ifndef SCULPTOR_H
#define SCULPTOR_H

#include<iostream>
#include<cstring>

using namespace std;
/**
 * @brief O tipo struct Voxel:
     * Voxels são como pixels, mas utilizados em imagens 3d (possuem 3 dimensões).
     * Os voxels tem como parametros suas  informações relativas a cor (red, green e blue) e transparência, além de um boleano que define se o voxel eh visivel ou nao.
     * @param r : float que define a intensidade de vermelho, podendo estar entre [0,1]
     * @param g : float que define a intensidade de verde, podendo estar entre [0,1]
     * @param b : float que define a intensidade de azul, podendo estar entre [0,1]
     * @param a : canal alfa, que define a transparencia do voxel, podendo estar entre [0,1]
     * @param isOn : define se o voxel esta ativo ou nao dentro da escultura, sendo 0 par desativado ou 1 para ativado
 */
struct Voxel{
    float r,g,b; /// Cores
    float a; /// Canal alpha
    bool isOn; /// Parametro que define se O voxel será exibido ou nao
};

/**
 * @brief A classe Sculptor
 * constroi um esqueleto onde serao adicionados os voxels e contem os metodos para manipular os pixels de uma matriz tridimensional
 */
class Sculptor
{

protected:
    /**
     * @brief voxel: representa a matriz tridimensional alocada dinamicamente - armazena todos os voxels
     */
    Voxel ***voxel;
    /**
     * @brief nx: define a dimensão no eixo x (numero de linhas da matriz)
     */
    int nx;
    /**
     * @brief ny: define a dimensão no eixo y (numero de colunas da matriz)
     */
    int ny;
    /**
     * @brief nz: define a dimensão no eixo z (numero de planos gerados)
     */
    int nz;
    /**
     * @brief r: representa a intensidade de vermelho, no intervalo de [0,1]
     */
    float r;
    /**
     * @brief g: intensidade atual da cor verde, varia entre [0,1]
     */
    float g;
    /**
     * @brief b: intensidade atual da cor azul, varia entre [0,1]
     */
    float b;
    /**
     * @brief a: representa a opacidade/transparencia, no intervalo de [0,1]
     */
    float a;
public:

     /**
     * @brief Sculptor: Construtor da classe Sculptor
     * @param _nx : dimensao em x (numero de linhas)
     * @param _ny : dimensao em y (numer de colunas)
     * @param _nz : dimensao em z (numero de planos)
     */
    Sculptor(int _nx, int _ny, int _nz);

    /**
      * @brief ~Sculptor: Destrutor da classe Sculptor
    */
    ~Sculptor();

    /**
     * @brief setColor : método que a cor atualmente selecionada do desenho
     * @param r : intensidade de vermelho, no intervalo [0,1]
     * @param g : intensidade de verde, no intervalo [0,1]
     * @param b : intensidade de azul, no intervalo [0,1]
     * @param alpha : transparencia do voxel, no intervalo [0,1]
     */
    void setColor(float r, float g, float b, float alpha);

    /**
     * @brief putVoxel : Define um voxel nas coordenadas (x,y,z). Torna isOn = true e voxel eh apresentado na cor de setColor.
     * @param x : coordenada em x
     * @param y : coordenada em y
     * @param z : coordenada em z
     */
    void putVoxel(int x, int y, int z);

    /**
     * @brief cutVoxel : Remove a exibição do voxel nas coordenadas (x,y,z) Torna isOn = false
     * @param x : coordenada em x
     * @param y : coordenada em y
     * @param z : coordenada em z
     */
    void cutVoxel(int x, int y, int z);

   /**
     * @brief putBox : Ativa todos os voxels nos intervalo [x0,x1], [y0,y1], [z0,z1] usando as cores e transparencia setadas previamente
     * @param x0 : coordenada inicial em x
     * @param x1 : coordenada final em x
     * @param y0 : coordenada inicial em y
     * @param y1 : coordenada final  em y
     * @param z0 : coordenada inicial em z
     * @param z1 : coordenada final  em z
     */
    void putBox(int x0, int x1,int y0, int y1,int z0, int z1);

    /**
     * @brief cutBox : Complementar ao putBox, desativa os voxels (isOn = false) no intervalo x[x0,x1], y[y0,y1], z[z0,z1] e atribui cor e transparencias setadas previamente
     * @param x0 : coordenada inicial em x
     * @param x1 : coordenada final em x
     * @param y0 : coordenada inicial em y
     * @param y1 : coordenada final  em y
     * @param z0 : coordenada inicial em z
     * @param z1 : coordenada final  em z
     */
    void cutBox(int x0, int x1,int y0, int y1,int z0, int z1);

    /**
     * @brief putSphere : Ativa os voxels que satisfazem a equação da esfera
     * @param xcenter : coordenada x do centro da esfera
     * @param ycenter : coordenada y do centro da esfera
     * @param zcenter : coordenada z do centro da esfera
     * @param radius : raio da esfera
     */
    void putSphere(int xcenter, int ycenter, int zcenter, int radius);

    /**
     * @brief cutSphere : Complementar a putSphere. Desativa todos os voxels que satisfazem a equação da esfera
     * @param xcenter : coordenada x do centro da esfera
     * @param ycenter : coordenada y do centro da esfera
     * @param zcenter : coordenada z do centro da esfera
     * @param radius : raio da esfera
     */
    void cutSphere(int xcenter, int ycenter, int zcenter, int radius);

  /**
     * @brief putEllipsoid : Ativa os voxels que satisfazem a equação do elipsoide, atribuindo cor e transparencia
     * @param xcenter : coordenada x do centro do elipsoide
     * @param ycenter : coordenada y do centro do elipsoide
     * @param zcenter : coordenada z do centro do elipsoide
     * @param rx : raio na direcao x
     * @param ry : raio na direcao y
     * @param rz : raio na direcao z
     */
    void putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);

      /**
     * @brief cutEllipsoid : Desativa todos os voxels que satisfazem a equação do elipsoide
     * @param xcenter : coordenada x do centro do elipsoide
     * @param ycenter : coordenada y do centro do elipsoide
     * @param zcenter : coordenada z do centro do elipsoide
     * @param rx : raio na direcao x
     * @param ry : raio na direcao y
     * @param rz : raio na direcao z
     */
    void cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);

     /**
     * @brief writeOFF : metodo que grava a escultura de acordo com o formato OFF
     * @param filename : caminho do arquivo OFF
     */
    void writeOFF(string filename);

    // Funções auxiliares

    /**
     * @brief elementoForadaMatriz: verifica se o elemento esta dentro dos limites do sculptor
     * @param x : coordenada em relacao ao eixo x
     * @param y : coordenada em relacao ao eixo y
     * @param z : coordenada em relacao ao eixo z
     */
    bool elementoForaDaMatriz(int x, int y, int z);

     /**
     * @brief constroiMatriz3D : Seta a matriz 3D com voxels com RGB e alpha = 0
     */
    void constroiMatriz3D();

     /**
     * @brief otimizar : Verifica quais voxels estão completamente rodeados por outros voxels e assinala isOn = false,
     * para otimizar a visualização e o desempenho
     */

    void print_sculptor();
       /**
        * @brief otimizar : Verifica quais voxels estão completamente rodeados por outros voxels e assinala isOn = false,
        * para otimizar a visualização e o desempenho
        */
};

#endif // SCULPTOR_H
