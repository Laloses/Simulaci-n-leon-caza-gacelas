#ifndef GACELA_H
#define GACELA_H

#include <QString>
#include <QTableWidget>
#include <QtDebug>
#include <QEventLoop>
#include <Qtimer>

class Leon;

class Gacela{
    public:
        Gacela(QString nom, int dir);
        QString nombre;
        int posx, posy;
        int direccion, distancia;
        Leon *leonvisto;
        QTableWidget* pradera;
        QList<Leon*> leones;
        QList<int> visionx;
        QList<int> visiony;
        void setTam(int t);
        void setGen(char g);
        void setEdad(int e);
        void setPradera(QTableWidget* p);
        void setLeones(QList<Leon*> leones);
        int getTam();
        char getGen();
        int getEdad();
        //bool Ver(QTableWidget* pradera, QList<Leon*> leones);
        bool Ver(QTableWidget* pradera);
        void Huir(Leon *l);
        void LlenarVision(int posx, int posy, int distancia);
        void VaciarVision();
    private:
        int tamanio;
        char genero;
        int edad;
};
#endif //GACELA_H
