#include "pradera.h"
#include "ui_pradera.h"


Pradera::~Pradera()
{
    delete ui;
}
Pradera::Pradera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pradera)
{
    ui->setupUi(this);
    srand(unsigned(time(nullptr)));
    inicializar();
    QDesktopWidget desk;
    this->setFixedWidth(desk.height()*0.9);
    this->setFixedHeight(desk.height()*0.9);
}
void Pradera::ajustarCeldas(){
    int i;
    int wTable = ui->tb_pradera->width();
    int hTable = ui->tb_pradera->height();
    for(i=0; i<=col; i++){
        ui->tb_pradera->setColumnWidth(i,wTable/ col);
        ui->tb_pradera->setRowHeight(i,hTable / col);
    }
    ui->tb_pradera->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tb_pradera->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tb_pradera->setIconSize(QSize(ui->tb_pradera->rowHeight(col-1),ui->tb_pradera->columnWidth(col-1)));
}
void Pradera::inicializar(){
    ui->tb_pradera->clear();
    fil=col=ui->colFil->value();
    ui->tb_pradera->setRowCount(fil);
    ui->tb_pradera->setColumnCount(col);
    for(int i=0; i<fil; i++){
        for(int j=0; j<col; j++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setBackgroundColor(Qt::white);
            item->setText("");
            ui->tb_pradera->setItem(i,j,item);
            item->setTextAlignment(Qt::AlignVCenter);
            //item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEditable);

        }
    }
    ajustarCeldas();
}
void Pradera::GenerarLeones(int cantL, int distancia){
    int posx, posy,dir;
    QColor color(Qt::red);
    //Hacemos un ciclo por la cantidad de leones que queremos
    for(int i=1; i<=cantL; i++){
        posx=1+rand()%(col-1);
        posy=1+rand()%(fil-1);

        //si no hay un leon u gacela en esa casilla
        if(ui->tb_pradera->item(posx,posy)->icon().isNull() ){

            //Generamos una dirección a donde estará viedo el leon
            dir=rand()%4;
            //Generamos nuestro leon
            Leon *leon= new Leon("Leon "+QString::number(i),dir);
            leon->posx=posx;
            leon->posy=posy;
            listaLeones.append(leon);
            ui->tb_pradera->item(posx,posy)->setText(leon->nombre);
            ui->tb_pradera->item(posx,posy)->setBackgroundColor(color);

            //Generamos su tamaño, edad y genero
            leon->setTam(rand()%2);
            leon->setEdad(1+rand()%100);
            if(rand()%1==0)
                leon->setGen('M');
            else leon->setGen('F');

            //Ponemos el icono dependiendo su edad
            switch(leon->getEdad()){
                case 1 ... 20:{
                    QIcon icoL(":/leonP.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoL);
                }
                break;
                case 21 ... 59 :{
                    QIcon icoL(":/leonM.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoL);
                }
                break;
                case 60 ... 100:{
                    QIcon icoL(":/leonG.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoL);
                }
                break;
            }

            //Llenamos la vision del leon
            leon->LlenarVision(ui->tb_pradera,posx,posy,distancia);
        }
    }
}
void Pradera::GenerarGacelas(int cantG, int distancia){
    int posx, posy,dir;
    QColor color(Qt::blue);
    for(int i=1; i<=cantG; i++){
        posx=1+rand()%(col-1);
        posy=1+rand()%(fil-1);

        if(ui->tb_pradera->item(posx,posy)->icon().isNull() ){

            //Generamos una dirección a donde estará viedo la gacela
            dir=rand()%4;
            //Generamos nuestra gacela
            Gacela *gacela= new Gacela("Gacela "+QString::number(i),dir);
            listaGacelas.append(gacela);
            gacela->posx=posx;
            gacela->posy=posy;
            ui->tb_pradera->item(posx,posy)->setText(gacela->nombre);
            ui->tb_pradera->item(posx,posy)->setBackgroundColor(color);

            //Generamos su tamaño, edad y genero
            gacela->setTam(rand()%2);
            gacela->setEdad(1+rand()%100);
            if(rand()%1==0)
                gacela->setGen('M');
            else gacela->setGen('F');

            //Ponemos el icono dependiendo su edad
            switch(gacela->getEdad()){
                case 1 ... 20:{
                    QIcon icoG(":/gacelaP.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoG);
                }
                break;
                case 21 ... 59 :{
                    QIcon icoG(":/gacelaM.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoG);
                }
                break;
                case 60 ... 100:{
                    QIcon icoG(":/gacelaG.png");
                    ui->tb_pradera->item(posx,posy)->setIcon(icoG);
                }
                break;
            }

            //Llenamos la vision de la gacela
            gacela->setPradera(ui->tb_pradera);
            gacela->LlenarVision(posx,posy,distancia);
        }
    }
}

void Pradera::on_pb_start_clicked()
{
    //Vaciamos nuestras listas
    listaLeones.clear();
    listaGacelas.clear();
    qDebug()<<endl<<endl<<endl<<endl<<endl<<endl;
    inicializar();
    //Los generamos de nuevo
    GenerarGacelas(ui->cantGacelas->value(),ui->distancia->value());
    GenerarLeones(ui->cantLeones->value(),ui->distancia->value());

    //Hacemos que cada leon vea
    int i, cLeones = listaLeones.count();
    for(i=0; i<cLeones; i++){
        listaLeones.at(i)->Ver(ui->tb_pradera, listaGacelas);
    }
    //Hacemos que cada gacela vea
    int cGacela = listaGacelas.count();
    for(i=0; i<cGacela; i++){
        listaGacelas[i]->setLeones(listaLeones);
        if( listaGacelas[i]->Ver(ui->tb_pradera) ){
            listaGacelas[i]->Huir(listaGacelas[i]->leonvisto);
        }
    }
}
