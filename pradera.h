#ifndef PRADERA_H
#define PRADERA_H

#include <leon.h>
#include <Gacela.h>
#include <QMainWindow>
#include <QDesktopWidget>

namespace Ui {
class Pradera;
}

class Pradera : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pradera(QWidget *parent = nullptr);
    ~Pradera();

private slots:
    void on_pb_start_clicked();

private:
    Ui::Pradera *ui;
    QList<Leon*> listaLeones;
    QList<Gacela*> listaGacelas;
    int col,fil;
    void GenerarLeones(int cantL, int distancia);
    void GenerarGacelas(int cantL, int distancia);
    void inicializar();
    void ajustarCeldas();

};

#endif // PRADERA_H
