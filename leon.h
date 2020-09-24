#ifndef LEON_H
#define LEON_H

#include <QString>
#include <QTableWidget>
#include <QtDebug>
#include <QEventLoop>
#include <QSound>
#include <Qtimer>

class Gacela;

class Leon{
    public:
        Leon(QString nom, int dir);
        QString nombre;
        int direccion, distancia;
        int posx, posy;
        QList<int> visionx;
        QList<int> visiony;
        void setTam(int t);
        void setGen(char g);
        void setEdad(int e);
        int getTam();
        char getGen();
        int getEdad();
        void Ver(QTableWidget* pradera, QList<Gacela*> gacelas);
        void Atacar(Gacela *g,QTableWidget* pradera, int gacelax, int gacelay);
        void LlenarVision(QTableWidget* pradera, int posx, int posy, int distancia);
        void VaciarVision(QTableWidget* pradera);
    private:
        int tamanio;
        char genero;
        int edad;
};
#endif //LEON_H
