#ifndef CONEXAOSCULPTOR_H
#define CONEXAOSCULPTOR_H

#include <QDialog>

namespace Ui {
class ConexaoSculptor;
}

/**
 * @brief A Classe ConexaoSculptor
 * É a caixa de interação na qual o usuario escolhe as dimensões do escultor
 */
class ConexaoSculptor : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief ConexaoSculptor: Construtor da classe ConexaoSculptor
     */
    explicit ConexaoSculptor(QWidget *parent = nullptr);
    /**
      @brief ~ConexaoSculptorr: Destrutor da classe ConexaoSculptor
     */
    ~ConexaoSculptor();
    /**
     * @brief NumDeLinhas : metodo que retorna o numero de linhas(X) da matriz 3D do escultor
     * @return numero de linhas do escultor (matriz 3D)
     */
    int getNumDeLinhas();
    /**
     * @brief getNumDeColunas : método que retorna o numero de colunas(Y) da matriz 3D do escultor
     * @return numero de colunas do escultor (matriz 3D)
     */
    int getNumDeColunas();
    /**
     * @brief getNumDePlanos : método que retorna o numero de planos(Z) da matriz 3D do escultor
     * @return numero de planos do escultor (matriz 3D)
     */
    int getNumDePlanos();

private:
    Ui::ConexaoSculptor *ui;
};

#endif // CONEXAOSCULPTOR_H
