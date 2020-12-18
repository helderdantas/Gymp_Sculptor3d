#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class
 * Janela principal da aplicacao, onde o escultor vai interagir, por meio de uma matriz 3D composta por voxels 3D.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow : Construtor da classe MainWindow
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief ~MainWindow : Destrutor da classe MainWindow
     */
    ~MainWindow();

public slots:
    /**
     * @brief capturaAcao: slot que captura a acao definida pelo usuario na janela principal
     */
    void capturaAcao(bool);

signals:
    /**
     * @brief nomeAcao: emite um sinal com o nome da acao que foi capturada pelo metédo capturaAcao
     */
    void nomeAcao(QString);

private:
    Ui::MainWindow *ui;
    QString ultimaAcao;
};

#endif // MAINWINDOW_H
