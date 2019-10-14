#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>

//Medidas de la escena
#define WIDTH 500
#define HEIGHT 700

Game::Game(QWidget *parent){
    // Creando escena
    scene = new QGraphicsScene();
    // Tamaño de la escena
    scene->setSceneRect(0,0,WIDTH,HEIGHT); // make the scene 800x600 instead of infinity by infinity (default)
    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    // Desactivar barras de desplazamiento
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Tamaño fijo
    setFixedSize(WIDTH,HEIGHT);

    // Dibujar bordes de la escena
    QPen mypen = QPen(Qt::red);
    mypen.setWidth(10);
    QLineF TopLine(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF LeftLine(scene->sceneRect().topLeft(), scene->sceneRect().bottomLeft());
    QLineF RightLine(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());
    QLineF BottomLine(scene->sceneRect().bottomLeft(), scene->sceneRect().bottomRight());
    scene->addLine(TopLine,mypen);
    scene->addLine(LeftLine,mypen);
    scene->addLine(RightLine,mypen);
    scene->addLine(BottomLine,mypen);

    // Crear bola
    ball = new Ball();
    scene->addItem(ball);

    QPen mypen_rect=QPen(Qt::red);
    mypen_rect.setWidth(5);

    // Obstaculo 1
    //Agregar obstaculos
    obs1 = new Obstaculo();
    obs1->setPixmap(QPixmap(":/imagenes/obstaculo1.png"));
    obs1->setPos(40,HEIGHT-280);
    scene->addItem(obs1);

//    //Agregar obstaculos
//    obs2 = new Obstaculo();
//    obs2->setPixmap(QPixmap(":/imagenes/obstaculo2.png"));
//    obs2->setPos(300,300);
//    scene->addItem(obs2);

    // Bonus Estrella
    Obstaculo *estrella = new Obstaculo();
    estrella->setPixmap(QPixmap(":/imagenes/estrella.png"));
    estrella->setPos(50,250);
    scene->addItem(estrella);

    // JUGADOR 1
    // Paredes
    scene->addLine(0,HEIGHT-120,WIDTH/3,HEIGHT-120,mypen_rect);
    scene->addLine(WIDTH-WIDTH/3,HEIGHT-120,WIDTH,HEIGHT-120,mypen_rect);

    // Jugador 1
    // Crear palanca izquierda
    left_lever_player_1= new Levers();
    left_lever_player_1->setPixmap(QPixmap(":/imagenes/palanca.png"));
    left_lever_player_1->setPos(WIDTH/3,HEIGHT-120);
    scene->addItem(left_lever_player_1);
    QTransform t_left_player_1;
    t_left_player_1.translate(0,0).rotate(45);
    left_lever_player_1->setTransform(t_left_player_1);

    left_lever_player_1->setFlag(QGraphicsItem::ItemIsFocusable);
    left_lever_player_1->setFocus();
    // Crear palanca derecha
    right_lever_player_1= new Levers();
    right_lever_player_1->setPixmap(QPixmap(":/imagenes/palanca.png"));
    right_lever_player_1->setPos(WIDTH-WIDTH/3,HEIGHT-120);
    scene->addItem(right_lever_player_1);

    QTransform t_right_player_1;
    t_right_player_1.translate(0,0).rotate(135);
    right_lever_player_1->setTransform(t_right_player_1);

    // Jugador 2
    // Paredes
    scene->addLine(0,120,WIDTH/3,120,mypen_rect);
    scene->addLine(WIDTH-WIDTH/3,120,WIDTH,120,mypen_rect);
    // Crear palanca izquierda
    left_lever_player_2= new Levers();
    left_lever_player_2->setPixmap(QPixmap(":/imagenes/palanca.png"));
    left_lever_player_2->setPos(WIDTH/3,120);
    scene->addItem(left_lever_player_2);
    QTransform t_left_player_2;
    t_left_player_2.translate(0,0).rotate(-45);
    left_lever_player_2->setTransform(t_left_player_2);

    left_lever_player_2->setFlag(QGraphicsItem::ItemIsFocusable);
    left_lever_player_2->setFocus();
    // Crear palanca derecha
    right_lever_player_2= new Levers();
    right_lever_player_2->setPixmap(QPixmap(":/imagenes/palanca.png"));
    right_lever_player_2->setPos(WIDTH-WIDTH/3,120);
    scene->addItem(right_lever_player_2);

    QTransform t_right_player_2;
    t_right_player_2.translate(0,0).rotate(-135);
    right_lever_player_2->setTransform(t_right_player_2);

    // Crear numero de vidas
    health = new Health();
    health->setPos(health->x()+10,health->y()+10);
    scene->addItem(health);

    show();

    //Conectar
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),scene,SLOT(advance()));
    timer->start(20);
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left){
        right_angle_player_1+=45;
        left_lever_player_1->setRotation(-right_angle_player_1);
    }
    if (event->key() ==Qt::Key_Right ) {
       right_angle_player_1=45;
       right_lever_player_1->setRotation(right_angle_player_1);
    }
    if (event->key() == Qt::Key_4){
        right_angle_player_2+=-45;
        left_lever_player_2->setRotation(-right_angle_player_2);
    }
    if (event->key() ==Qt::Key_6 ) {
        right_angle_player_2=-45;
        right_lever_player_2->setRotation(right_angle_player_2);
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{

    if (event->key() ==Qt::Key_Left) {
        right_angle_player_1=0;
        left_lever_player_1->setRotation(right_angle_player_1);
    }
    if (event->key() ==Qt::Key_Right ) {
        right_angle_player_1=0;
        right_lever_player_1->setRotation(right_angle_player_1);
     }
     if (event->key() ==Qt::Key_4) {
         right_angle_player_2=0;
         left_lever_player_2->setRotation(right_angle_player_2);
     }
     if (event->key() ==Qt::Key_6 ) {
         right_angle_player_2=0;
         right_lever_player_2->setRotation(right_angle_player_2);
     }
}
