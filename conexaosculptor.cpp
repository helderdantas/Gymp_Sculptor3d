#include "conexaosculptor.h"
#include "ui_conexaosculptor.h"


ConexaoSculptor::ConexaoSculptor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConexaoSculptor)
{
    ui->setupUi(this);
}

ConexaoSculptor::~ConexaoSculptor()
{
    delete ui;
}

int ConexaoSculptor::getNumDeLinhas()
{
    return ui->spinBoxLinhas->value();
}

int ConexaoSculptor::getNumDeColunas()
{
    return ui->spinBoxColunas->value();
}

int ConexaoSculptor::getNumDePlanos()
{
    return ui->spinBoxPlanos->value();
}
