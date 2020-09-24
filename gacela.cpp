#include "gacela.h"
#include "pradera.h"

Gacela::Gacela(QString nom, int dir)
{
    nombre=nom;
    direccion=dir;
    edad=1;
    tamanio=0;
    genero='M';
    posx=posy=0;
}
//Setters
void Gacela::setTam(int t){
    tamanio=t;
}
void Gacela::setGen(char g){
    genero=g;
}
void Gacela::setEdad(int e){
    edad=e;
}
void Gacela::setLeones(QList<Leon *> leones){
    this->leones=leones;
}
void Gacela::setPradera(QTableWidget *p){
    pradera=p;
}
//Getters
int Gacela::getTam(){
    return tamanio;
}
char Gacela::getGen(){
    return genero;
}
int Gacela::getEdad(){
    return edad;
}

bool Gacela::Ver(QTableWidget *pradera){
    int i,j;
    QString nombreLeon;
    //Revisamos en nuestro rango de vision si hay un leon
    for (i=0; i<visionx.size();i++) {
        nombreLeon=pradera->item(visionx.at(i),visiony.at(i))->text();
        //Condicion de que si hay uno
        if(nombreLeon.split(" ")[0] == "Leon" ){

            //Imprimimos la deteccion
            qDebug()<<nombre+" ve a "+nombreLeon;

            //Buscamos al leon
            Leon *leon = nullptr;
            for (j=0;j<leones.size();j++) {
                //Si encontramos el leon
                if( leones.at(j)->nombre == nombreLeon ){
                    leon = leones.at(j);
                    break;
                }
            }
            //Buscamos las características del leon para huir
            //Edades: 1-20, 21-59,60-100
            //Tamaños: 0,1,2 (P,M,G)
            // Leones grandes pero no viejos, tamaños medianos y grandes
            if( leon->getEdad()>20 && leon->getTam()>0 /* && leon->getGen()=='F' */){
                leonvisto=leon;
                return true;
            }
        }
    }
    return false;
}

void Gacela::Huir(Leon *leon){
    qDebug()<<nombre+" huye de "+leon->nombre;
    //Movemos hacia el lado contrario de la vision
    //En visionx y "y" se encuentra la posicion hacia donde está viendo, en el 0,0
    //Encontes comparamos si es menos en las x o en las y
    int i,j;
    i=posx; j=posy;

    //Cosas para detener la ejecución
    QEventLoop loop;
    bool vio=false;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(500);
    QTableWidgetItem *aux= new QTableWidgetItem;

    if( visionx.at(0) < posx){
        //Entonces ve hacia arriba
        //Cambiamos la direccion
        direccion=1;
        while(i+1 < pradera->rowCount() && pradera->item(i+1,j)->icon().isNull() ){
            //Vaciamos su vision
            VaciarVision();
            //Llenamos la nueva vision con la direccion nueva
            LlenarVision(i,j,distancia);

            aux=pradera->item(i,j)->clone();
            pradera->setItem(i+1,j, aux);
            pradera->setItem(i,j, new QTableWidgetItem);

            if( Ver(pradera) ){
                vio =true;
                break;
            }
            i++;
            loop.exec();
        }
        if (vio) Huir(leonvisto);
    }
    else
    if( visionx.at(0) > posx){
        //Entonces ve hacia abajo
        //Cambiamos la direccion
        direccion=0;
        while(i-1 >= 0 && pradera->item(i-1,j)->icon().isNull()){
            //Vaciamos su vision
            VaciarVision();
            //Llenamos la nueva vision con la direccion nueva

            aux=pradera->item(i,j)->clone();
            pradera->setItem(i-1,j, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            i--;

            LlenarVision(i,j,distancia);
            if( Ver(pradera) ){
                vio =true;
                break;
            }
            loop.exec();
        }
        if (vio) Huir(leonvisto);
    }
    else
    if( visiony.at(0) < posy){

        //Entonces ve hacia izq
        //Cambiamos la direccion
        direccion=2;
        while(j+1 < pradera->columnCount() && pradera->item(i,j+1)->icon().isNull()){
            //Vaciamos su vision
            VaciarVision();
            //Llenamos la nueva vision con la direccion nueva

            aux=pradera->item(i,j)->clone();
            //aux->setText( pradera->item(i,j)->text() );
            pradera->setItem(i,j+1, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            j++;

            LlenarVision(i,j,distancia);
            if( Ver(pradera) ){
                vio = true;
                break;
            }
            loop.exec();
        }
        if (vio) Huir(leonvisto);
    }
    else
    if( visiony.at(0) > posy){

        //Entonces ve hacia derecha
        //Cambiamos la direccion
        direccion=3;
        while(j-1 >= 0 && pradera->item(i,j-1)->icon().isNull()){
            //Vaciamos su vision
            VaciarVision();
            //Llenamos la nueva vision con la direccion nueva
            LlenarVision(i,j,distancia);

            aux = pradera->item(i,j)->clone();
            pradera->setItem(i,j-1, aux);
            pradera->setItem(i,j,  new QTableWidgetItem);
            j--;

            if( Ver(pradera) ){
                vio =true;
                break;
            }
            loop.exec();
        }
        if (vio) Huir(leonvisto);
    }
}

void Gacela::LlenarVision(int posx, int posy, int distancia){
    this->distancia=distancia;
    distancia+=1;
    //Llenamos su campo de vision
    int cont=1,aux=1,i=0;
    QColor color(Qt::blue);

    switch (direccion) {
    //Arriba
    case 0:
        while(posx-cont>=0 && cont<distancia){
                pradera->item(posx-cont,posy)->setBackgroundColor(color);
                if(pradera->item(posx-cont,posy)->text().isEmpty())
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
            if(pradera->item(posx+cont,posy)->text().isEmpty())
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

void Gacela::VaciarVision(){
    int i=0;
    for(i=0; i<visionx.length(); i++){
        if( pradera->item(visionx.at(i),visiony.at(i))->icon().isNull() ){
            pradera->item(visionx.at(i), visiony.at(i))->setBackgroundColor(Qt::white);
            pradera->item(visionx.at(i), visiony.at(i))->setText("");
        }
    }
    visionx.clear();
    visiony.clear();
}
