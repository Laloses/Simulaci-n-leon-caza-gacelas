#include "leon.h"
#include "pradera.h"

Leon::Leon(QString nom, int dir)
{
    nombre=nom;
    direccion=dir;
    edad=1;
    tamanio=0;
    genero='M';
    posx=posy=0;
}

//Setters
void Leon::setTam(int t){
    tamanio=t;
}
void Leon::setGen(char g){
    genero=g;
}
void Leon::setEdad(int e){
    edad=e;
}
//Getters
int Leon::getTam(){
    return tamanio;
}
char Leon::getGen(){
    return genero;
}
int Leon::getEdad(){
    return edad;
}

void Leon::Ver(QTableWidget *pradera, QList<Gacela*> gacelas){
    int i,j;
    QString nombreGacela;
    //Revisamos en nuestro rango de vision si hay una gacela
    for (i=0; i<visionx.size(); i++) {
        nombreGacela=pradera->item( visionx.at(i),visiony.at(i) )->text();
        if(nombreGacela.split(" ")[0] == "Gacela" ){

            //Imprimimos la deteccion
            qDebug()<<nombre+" ve a "+pradera->item( visionx.at(i),visiony.at(i) )->text();

            //Buscamos la gacela
            Gacela *gacela = nullptr;
            for (j=0;j<gacelas.size();j++) {
                //Si lo encontramos
                if( gacelas.at(j)->nombre == nombreGacela ){
                    gacela = gacelas.at(j);
                    break;
                }
            }
            //Buscamos las características de la gacela y hacer las Reglas
            //Edades: 1-20, 21-59,60-100
            //Tamaños: 0,1,2 (P,M,G)
            // Gacelas pequeñas y medianas, tamaños pequeños y medianos, el género da igual
            if( gacela->getEdad()<60  && gacela->getTam()<2 ){
                Atacar(gacela,pradera,visionx.at(i),visiony.at(i));
                QIcon rip(":/rip.png");
                pradera->item( visionx.at(i),visiony.at(i) )->setIcon(rip);
            }
        }
    }
}

void Leon::Atacar(Gacela *g,QTableWidget* pradera, int gacX, int gacY){
    qDebug()<<nombre+" ataca a "+g->nombre;

    //Movemos hacia el lado contrario de la vision
    //En visionx y "y" se encuentra la posicion hacia donde está viendo, en el 0,0
    //Encontes comparamos si es menos en las x o en las y
    int i,j;
    i=posx; j=posy;

    //Cosas para detener la ejecución
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(800);
    QTableWidgetItem *aux= new QTableWidgetItem;
    if( gacX < posx){
        //Entonces ve hacia arriba
        while( gacX < i-1 ){
            aux=pradera->item(i,j)->clone();
            pradera->setItem(i-1,j, aux);
            pradera->setItem(i,j, new QTableWidgetItem);
            i--;
            loop.exec();
        }
    }
    if( gacX > posx){
        //Entonces ve hacia abajo
        while(gacX > i+1 ){
            aux=pradera->item(i,j)->clone();
            pradera->setItem(i+1,j, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            i++;
            loop.exec();
        }
    }
    if( gacY < posy){
        //Entonces ve hacia izq
        while(gacY < j-1){
            aux=pradera->item(i,j)->clone();
            pradera->setItem(i,j-1, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            j--;
            loop.exec();
        }
    }
    if( gacY > posy){
        //Entonces ve hacia derecha
        while(gacY > j+1){
            aux=pradera->item(i,j)->clone();
            pradera->setItem(i,j+1, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            j++;
            loop.exec();
        }
    }
    QSound::play(":/leon.wav");
}

void Leon::LlenarVision(QTableWidget *pradera, int posx, int posy, int distancia){
    distancia+=1;
    //Llenamos su campo de vision
    int cont=1, aux=1,i=0;
    QColor color(Qt::red);
    switch (direccion) {
    //Arriba
    case 0:
        while(posx-cont>=0 && cont<distancia){
            if(pradera->item(posx-cont,posy)->backgroundColor()!=Qt::blue)
                pradera->item(posx-cont,posy)->setBackgroundColor(color);
                pradera->item(posx-cont,posy)->setText("||");
                //Guardamos el valor
                visionx.append(posx-cont);
                visiony.append(posy);

            if(cont > 1) {
                //Llenamos toda la linea derecha
                while(i<=aux && posy+i<pradera->columnCount()){
                    pradera->item(posx-cont,posy+i)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx-cont);
                    visiony.append(posy+i);
                    i++;
                } i=0;

                //Llenamos toda la linea izquierda
                while(i<=aux && posy-i>=0){
                    pradera->item(posx-cont,posy-i)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx-cont);
                    visiony.append(posy-i);
                    i++;
                } i=0;
                aux++;
            }
            cont++;
        }
        aux=1;
        break;
    //Abajo
    case 1:
        while(posx+cont<pradera->rowCount() && cont<distancia){
            pradera->item(posx+cont,posy)->setBackgroundColor(color);
            pradera->item(posx+cont,posy)->setText("||");
            //Guardamos el valor
            visionx.append(posx+cont);
            visiony.append(posy);

            if(cont > 1) {
                //Llenamos toda la linea derecha
                while(i<=aux && posy+i<pradera->columnCount()){
                    pradera->item(posx+cont,posy+i)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx+cont);
                    visiony.append(posy+i);
                    i++;
                } i =0;
                //Llenamos toda la linea izquierda
                while(i<=aux && posy-i>=0){
                    pradera->item(posx+cont,posy-i)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx+cont);
                    visiony.append(posy-i);
                    i++;
                }i=0;
                aux++;
            }
            cont++;
        }
        aux=1;
        break;
    //Derecha
    case 2:
        while(posy+cont<pradera->columnCount() && cont<distancia){
            pradera->item(posx,posy+cont)->setBackgroundColor(color);
            pradera->item(posx,posy+cont)->setText("=");
            //Guardamos el valor
            visionx.append(posx);
            visiony.append(posy+cont);

            if(cont > 1) {
                //Llenamos toda la linea derecha
                while(i<=aux && posx+i<pradera->rowCount()){
                    pradera->item(posx+i,posy+cont)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx+i);
                    visiony.append(posy+cont);
                    i++;
                }i=0;
                //Llenamos toda la linea izquierda
                while(i<=aux && posx-i>=0){
                    pradera->item(posx-i,posy+cont)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx-i);
                    visiony.append(posy+cont);
                    i++;
                }i=0;
                aux++;
            }
            cont++;
        }
        aux=1;
        break;
    //Izquierda
    case 3:
        while(posy-cont>=0 && cont<distancia){
            pradera->item(posx,posy-cont)->setBackgroundColor(color);
            pradera->item(posx,posy-cont)->setText("=");
            //Guardamos el valor
            visionx.append(posx);
            visiony.append(posy-cont);

            if(cont > 1) {
                //Llenamos toda la linea derecha
                while(i<=aux && posx+i<pradera->rowCount()){
                    pradera->item(posx+i,posy-cont)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx+i);
                    visiony.append(posy-cont);
                    i++;
                }i=0;
                //Llenamos toda la linea izquierda
                while(i<=aux && posx-i>=0){
                    pradera->item(posx-i,posy-cont)->setBackgroundColor(color);
                    //Guardamos el valor
                    visionx.append(posx-i);
                    visiony.append(posy-cont);
                    i++;
                }i=0;
                aux++;
            }
            cont++;
        }
        aux=1;
        break;

    }
}
