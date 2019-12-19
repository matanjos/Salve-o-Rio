#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

typedef struct{
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
}color;

typedef struct{
  GLfloat x;
  GLfloat y;
}vetor;

typedef struct{
    GLfloat speed;
    GLfloat yr;
    GLfloat xr;
}lixoType;

//prototipos
void LATA_METAL(void);
void livro(void);
void garrafa(void);
void Caco_Vidro(void);
void selectLIXOS();
void selectCOORD();
int randomizar(int);
int colision(vetor, int, vetor, vetor);
//variaveis

int oldTimeSinceStart = 0;
float deltaTime,  delay[8], movagua, alfaAgua, alfaAux, rodandoJogo, vidaRio = 7,coordLixo[4], personagemXR = 0.0f, personagemYR = 900.0f;;
time_t t;
char texto[30];
    int btn1_x = 0, btn1_w = 400, btn1_h = 150, btn1_y = -50,
        btn2_x = 0, btn2_w = 400, btn2_h = 150, btn2_y = -250,
        contVidas = 4, pontos = 0, dificuldade = 0,tipoLixeira = 0,
        flag = 10, selectLixo[4];
vetor Centro, aux, aux2, riop1, riop2;
lixoType plastico, papel, vidro, metal;
color colorRio, colorTerra, colorLixeira, colorBack;


void CriaMenu();
void colisionAction();

//funcao do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
         if (state == GLUT_DOWN)
            CriaMenu();


    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN){
            if((x > btn1_w) && (x < (btn1_w + 200)) && (y > 336) && (y < 416))
                if(rodandoJogo == 0){
                    rodandoJogo = 1;
                }

            if((x > btn1_w) && (x < (btn1_w + 200)) && (y > 436) && (y < 516))
                if(rodandoJogo == 0){
                    rodandoJogo = 2;

                }

         }
    glutPostRedisplay();
}

float unidade = 30.0f;//funcao usada na atualiza��o das coordenadas do objeto quando uma das setas � pressunada

//funcao teclado setas
void TeclasEspeciais(int key, int x, int y)
{
    switch (key){
        case GLUT_KEY_RIGHT:
            personagemXR+=unidade;
            break;

        case GLUT_KEY_LEFT:
            personagemXR-=unidade;
        break;

        case GLUT_KEY_UP:
            tipoLixeira += 1;
            if(tipoLixeira == 4)
                tipoLixeira = 1;

            break;

        case GLUT_KEY_DOWN:
            tipoLixeira -= 1;
            if(tipoLixeira == 0)
                tipoLixeira = 3;

        break;

        default:
         break;
    }
    glutPostRedisplay();
}

void Teclado (unsigned char key, int xmouse, int ymouse)
{
	switch (key){
		case '1':
			tipoLixeira = 0;
			break;

		case '2':
			tipoLixeira = 1;
		break;
		case '3':
			tipoLixeira = 2;
		break;
		case '4':
			tipoLixeira = 3;
		break;
		case 27: //esc
			rodandoJogo = 0;
		break;

		default:
         break;
	}
   glutPostRedisplay();
}


void plastico_f(){
    float newspeed;

    plastico.speed += 0.0002;
    newspeed = pow(2.71828183, plastico.speed)*0.1;
    if(dificuldade == 2){
        plastico.yr += (0.2 * deltaTime);
    }else{
        plastico.yr += (0.1 * deltaTime);
    }

    glPushMatrix();
    garrafa();
    glPopMatrix();
}


void metal_f(){
    float newspeed;
    metal.speed += 0.0002;
    newspeed = pow(2.71828183, metal.speed)*0.1 ;
    if(dificuldade == 2){
        metal.yr += (0.2 * deltaTime);
    }else{
        metal.yr += (0.1 * deltaTime);
    }
    glPushMatrix();
    LATA_METAL();
    glPopMatrix();
}

void papel_f(){
    float newspeed;
    papel.speed += 0.0002;

    newspeed = pow(2.71828183, papel.speed)*0.1 ;
    if(dificuldade == 2){
        papel.yr += (0.2 * deltaTime);
    }else{
        papel.yr += (0.1 * deltaTime);
    }
     glPushMatrix();
    livro();
    glPopMatrix();
}

void vidro_f(){
    float newspeed;
    vidro.speed += 0.0002;
    newspeed = pow(2.71828183, vidro.speed)*0.1 ;
    if(dificuldade == 2){
        vidro.yr += (0.2 * deltaTime);
    }else{
        vidro.yr += (0.1 * deltaTime);
    }

    glPushMatrix();
    Caco_Vidro();
    glPopMatrix();
}


void paraJogo(){
    pontos = 0;
    delay[0] =  0.0f;
    delay[1] =  0.0f;
    delay[2] =  0.0f;
    delay[3] =  0.0f;
    delay[4] =  0.0f;
    delay[5] =  0.0f;
    delay[6] =  0.0f;
    delay[7] =  0.0f;

    vidaRio = 7;
    colorRio.r = 0.4f;
	colorRio.g = 0.8f;
    colorRio.b=  0.8f;

    plastico.speed = 0.01;
    plastico.yr = 0.0;
    plastico.xr = 0.0;
    metal.speed = 0.01;
    metal.yr = 0.0;
    metal.xr = 0.0;
    papel.speed = 0.01;
    papel.yr = 0.0;
    papel.xr = 0.0;
    vidro.speed = 0.01;
    vidro.yr = 0.0;
    vidro.xr = 0.0;

    selectLIXOS();
    selectCOORD();
}


void colisaoLixo(int pos){
    vetor aux, aux2, aux3, aux4;
    int lixo = selectLixo[pos], lixo2, lixo3, pos2, pos3;
    do{
        lixo2 = (randomizar(3));
    }while(lixo2 == lixo);
    do{
        lixo3 = (randomizar(3));
    }while((lixo3 == lixo) || (lixo3 == lixo2));

    do{
        pos2 = (randomizar(3));
    }while(pos2 == pos);
    do{
        pos3 = (randomizar(3));
    }while((pos3 == pos) || (pos3 == pos2));


    aux.x=(coordLixo[pos]*100)-10.0f;
    aux2.x=(coordLixo[pos]*100)+10.0f;

    switch (lixo){
                case 0:
                    aux.y=(int)plastico.yr-10.0f;
                    aux2.y=(int)plastico.yr+10.0f;
                    break;
                case 1:
                    aux.y=(int)metal.yr-10.0f;
                    aux2.y=(int)metal.yr+10.0f;
                    break;
                case 2:
                    aux.y=(int)papel.yr-10.0f;
                    aux2.y=(int)papel.yr+10.0f;
                    break;
                case 3:
                    aux.y=(int)vidro.yr-10.0f;
                    aux2.y=(int)vidro.yr+10.0f;
                    break;

                default:
                    aux.y=(int)0;
                    aux2.y=(int)0;
                    break;
    }

    aux3.x = aux.x;
    aux3.y = aux2.y;
    aux4.x = aux2.x;
    aux4.y = aux.y;
    if((colision(Centro, 50, aux, aux2)) || colision(Centro, 50, aux3, aux4)){
        if(tipoLixeira == lixo){
            pontos = pontos + 100;
            printf("Acertou a lixeira! :) Pontos: %d \n", pontos);
        }else{
            pontos = pontos - 100;
            printf("Errou a lixeira! :( Pontos: %d \n", pontos);

        }

        switch (lixo){
                case 0:
                    plastico.speed = 0.01;
                    plastico.yr = 0.0;
                    break;
                case 1:
                    metal.speed = 0.01;
                    metal.yr = 0.0;
                    break;
                case 2:
                    papel.speed = 0.01;
                    papel.yr = 0.0;
                    break;
                case 3:
                    vidro.speed = 0.01;
                    vidro.yr = 0.0;
                    break;

                default:
                    break;
        }
        delay[pos+4] = 0.0f;

        do{
            coordLixo[pos] = (randomizar(8) * pow(-1,randomizar(2)));
        }while((coordLixo[pos] == coordLixo[pos2]) || ((coordLixo[pos] == coordLixo[pos3])));

        do{
            selectLixo[pos] = randomizar(4);
        }while((selectLixo[pos] == selectLixo[pos2]) ||
            (selectLixo[pos] == selectLixo[pos3]));
        printf("Colidiu na lixeira! \n");
    }

    if(colision(aux, 50, riop1, riop2)){
	    colorRio.g -= 0.1f;
        colorRio.b -= 0.2f;

        vidaRio -= 1;
        if(!vidaRio){
            printf("RIO MORREU\n");
            paraJogo();
            rodandoJogo = 3;

        }

        printf("Colidiu no rio! %d \n", lixo);
        switch (lixo){
                case 0:
                    plastico.speed = 0.01;
                    plastico.yr = 0.0;
                    break;
                case 1:
                    metal.speed = 0.01;
                    metal.yr = 0.0;
                    break;
                case 2:
                    papel.speed = 0.01;
                    papel.yr = 0.0;
                    break;
                case 3:
                    vidro.speed = 0.01;
                    vidro.yr = 0.0;
                    break;

                default:
                    break;
        }
        delay[pos+4] = 0.0f;

        do{
            coordLixo[pos] = (randomizar(8) * pow(-1,randomizar(2)));
        }while((coordLixo[pos] == coordLixo[pos2]) || ((coordLixo[pos] == coordLixo[pos3])));

        do{
            selectLixo[pos] = randomizar(4);
        }while((selectLixo[pos] == selectLixo[pos2]) ||
            (selectLixo[pos] == selectLixo[pos3]));

    }


}

void colisionAction(){
    Centro.x = personagemXR;
    Centro.y = personagemYR;

    colisaoLixo(0);
    colisaoLixo(1);
    colisaoLixo(2);

}


//func�o que faz os calculos pra saber se ocorreu uma colis�o
int colision(vetor Centro,int Tamanho,vetor linePosA, vetor linePosB){

    float A, B , C, D;
    A = Centro.x - linePosA.x;
    B = Centro.y - linePosA.y ;
    C = linePosB.x - linePosA.x;
    D = linePosB.y - linePosA.y;

    float dot = A * C + B * D;
    float len = C * C + D * D;
    float para = dot / len;

    float x,y;

    if(para < 0){
        x = linePosA.x;
        y = linePosA.y;
    }else if(para > 1){
        x = linePosB.x;
        y = linePosB.y;
    }else{
        x = linePosA.x + para * C;
        y = linePosA.y + para * D;
    }
    float xdif = Centro.x - x;
    float ydif = Centro.y - y;
    float hypo = sqrt((xdif * xdif) + (ydif * ydif));
    if (hypo < Tamanho){
        return 1;
    }
    return 0;
}

void DesenhaTexto(void *font, char *string)
{
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*string++);
}
void DesenhaTextoBMP(void *font, char *string)
{
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*string++);
}
void DesenhaTextoStroke(void *font, char *string)
{
	while(*string)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,*string++);
}

void desenha_rio(){

    glColor3f(colorRio.r, colorRio.g, colorRio.b);
    GLfloat polygonVertices[] =
    {
        0, -25,
        0, 500,
        50, -20,
        50, 500,
        100, -15,
        100, 500,
        150, -12,
        150, 500,
        200, -10,
        200, 500,
        250, -14,
        250, 500,
        300, -16,
        300, 500,
        350, -18,
        350, 500,
        400, -22,
        400, 500,
        450, -26,
        450, 500,
        500, -25,
        500, 500,
        550, -24,
        550, 500,
        600, -20,
        600, 500,
        650, -18,
        650, 500,
        700, -14,
        700, 500,
        750, -12,
        750, 500,
        800, -7,
        800, 500,
        850, -4,
        850, 500,
        900, -2,
        900, 500,
        950, -5,
        950, 500,
        1000, -7,
        1000, 500,
        1050, -4,
        1050, 500,
        1150, -2,
        1150, 500,
        1200, -4,
        1200, 500,
        1250, -7,
        1250, 500,
        1300, -4,
        1300, 500,
        1350, -2,
        1350, 500,
        1400, 0,
        1400, 500,
        1450, 4,
        1450, 500,
        1500, 6,
        1500, 500,
        1550, 2,
        1550, 500,
        1600, -3,
        1600, 500,
        1650, -7,
        1650, 500,
        1700, -8,
        1700, 500,
        1750, -12,
        1750, 500,
        1800, -17,
        1800, 500,
        1850, -20,
        1850, 500,
        1900, -18,
        1900, 500,
        1950, -17,
        1950, 500,
        2000, -15,
        2000, 500,

    };
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 2, GL_FLOAT, 0, polygonVertices );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 84 );
        glDisableClientState( GL_VERTEX_ARRAY );

}
void agua_rio(){
    if(!alfaAux){
        if(alfaAgua <= 0.8){
            alfaAgua += deltaTime*0.0001;
        }else{
            alfaAux = 1;
        }
    }else{
        if(alfaAgua >= 0.2){
            alfaAgua -= deltaTime*0.0001;
        }else{
            alfaAux = 0;
        }
    }

    movagua += deltaTime*0.02;
    if(movagua > 2000)
        movagua = 0;
    glPushMatrix();

    glLineWidth(3);

    glColor4f(0.9,1,1,alfaAgua);
    GLfloat lineVertices[] =
    {
        movagua +0, 300,
        movagua +40, 315,
        movagua +80, 320,
        movagua +120, 315,
        movagua +160, 310,
        movagua +200, 305,
        movagua +240, 297,
        movagua +280, 300,
        movagua +320, 310,
        movagua +360, 313,
        movagua +400, 300,
        movagua +360, 313-5,
        movagua +320, 310-5,
        movagua +280, 300-5,
        movagua +240, 297-5,
        movagua +200, 305-5,
        movagua +160, 310-5,
        movagua +120, 315-5,
        movagua +80, 320-5,
        movagua +40, 315-5,
        movagua +0, 300,
    };
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 2, GL_FLOAT, 0, lineVertices );
        glDrawArrays( GL_LINE_LOOP, 0, 21);
        glDisableClientState( GL_VERTEX_ARRAY );

    glPopMatrix();

}


void desenha_terraRio(){
    glColor3f(colorTerra.r, colorTerra.g, colorTerra.b);
    GLfloat polygonVertices[] =
    {
        0, -25,
        0, 500,
        50, -20,
        50, 500,
        100, -15,
        100, 500,
        150, -12,
        150, 500,
        200, -10,
        200, 500,
        250, -14,
        250, 500,
        300, -16,
        300, 500,
        350, -18,
        350, 500,
        400, -22,
        400, 500,
        450, -26,
        450, 500,
        500, -25,
        500, 500,
        550, -24,
        550, 500,
        600, -20,
        600, 500,
        650, -18,
        650, 500,
        700, -14,
        700, 500,
        750, -12,
        750, 500,
        800, -7,
        800, 500,
        850, -4,
        850, 500,
        900, -2,
        900, 500,
        950, -5,
        950, 500,
        1000, -7,
        1000, 500,
        1050, -4,
        1050, 500,
        1150, -2,
        1150, 500,
        1200, -4,
        1200, 500,
        1250, -7,
        1250, 500,
        1300, -4,
        1300, 500,
        1350, -2,
        1350, 500,
        1400, 0,
        1400, 500,
        1450, 4,
        1450, 500,
        1500, 6,
        1500, 500,
        1550, 2,
        1550, 500,
        1600, -3,
        1600, 500,
        1650, -7,
        1650, 500,
        1700, -8,
        1700, 500,
        1750, -12,
        1750, 500,
        1800, -17,
        1800, 500,
        1850, -20,
        1850, 500,
        1900, -18,
        1900, 500,
        1950, -17,
        1950, 500,
        2000, -15,
        2000, 500,

    };
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 2, GL_FLOAT, 0, polygonVertices );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 84 );
        glDisableClientState( GL_VERTEX_ARRAY );

}

void corLixeira(){

    switch (tipoLixeira){
                case 0:
                    colorLixeira.r = 1.0f;
                    colorLixeira.g = 0.0f;//plastico
                    colorLixeira.b=  0.0f;
                    break;
                case 1:
                    colorLixeira.r = 1.0f;
                    colorLixeira.g = 0.7f;//metal
                    colorLixeira.b=  0.1f;
                    break;
                case 2:
                    colorLixeira.r = 0.0f;//papel
                    colorLixeira.g=  0.5f;
                    colorLixeira.b=  1.0f;
                    break;
                case 3:
                    colorLixeira.r = 0.0f;
                    colorLixeira.g = 1.0f;//vidro
                    colorLixeira.b=  0.0f;
                    break;

                default:
                    break;
            }
}
void Lixeira(float xrL, float yrL, float r, float g , float b){

    glColor3f(r,g, b);

    glBegin(GL_QUADS);
    glVertex2f( xrL + -15.0f,yrL + 20.0f);
    glVertex2f( xrL + 15.0f,yrL + 20.0f);
    glVertex2f( xrL + 15.0f,yrL + -20.0f);
    glVertex2f( xrL + -15.0f,yrL + -20.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f( xrL + -15.0f,yrL + 20.0f);
    glVertex2f( xrL + 15.0f,yrL + 20.0f);
    glVertex2f( xrL + 15.0f,yrL + 25.0f);
    glVertex2f( xrL + -15.0f,yrL + 25.0f);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f( xrL + 15.0f,yrL + 25.0f);
    glVertex2f( xrL + 9.0f,yrL + 35.0f);
    glVertex2f( xrL + -15.0f,yrL + 25.0f);
    glVertex2f( xrL + -9.0f,yrL + 35.0f);
    glVertex2f( xrL + -9.0f,yrL + 35.0f);
    glVertex2f( xrL + 9.0f,yrL + 35.0f);

    glEnd();

}


int randomizar(int randMAX){
        return (rand() % randMAX);
}

void LIXOS(){
    glPushMatrix();
    colisionAction();
    if(delay[4]>2000) {
        if(delay[0]>500) {
            glTranslatef((coordLixo[0] * 100), -750.0f,0.0f);
            switch (selectLixo[0]){
                case 0:
                    plastico_f();
                    break;
                case 1:
                    metal_f();
                    break;
                case 2:
                    papel_f();
                    break;
                case 3:
                    vidro_f();
                    break;

                default:
                    break;
            }



        }else{
            delay[0]+= deltaTime;
        }
    }else{
         delay[4]+= deltaTime;
    }



        glPopMatrix();
        glPushMatrix();

    if(delay[5]>2000) {
        if(delay[1]>4000) {
            glTranslatef((coordLixo[1] * 100), -750.0f,0.0f);
            switch (selectLixo[1]){
                case 0:
                    plastico_f();
                    break;
                case 1:
                    metal_f();
                    break;
                case 2:
                    papel_f();
                    break;
                case 3:
                    vidro_f();
                    break;

                default:
                    break;
            }



        }else{
            delay[1]+= deltaTime;
        }
    }else{
        delay[5]+= deltaTime;
    }

        glPopMatrix();
        glPushMatrix();

    if(delay[6]>2000) {
        if(delay[2]>6000) {
            glTranslatef((coordLixo[2] * 100), -750.0f,0.0f);
            switch (selectLixo[2]){
                case 0:
                    plastico_f();
                    break;
                case 1:
                    metal_f();
                    break;
                case 2:
                    papel_f();
                    break;
                case 3:
                    vidro_f();
                    break;

                default:
                    break;
            }


        }else{
            delay[2]+= deltaTime;
        }
    }else{
        delay[6]+= deltaTime;
    }

        glPopMatrix();


}

void LATA_METAL(void)
{
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);//
//  corpo
    glVertex2f( metal.xr -5.0f ,metal.yr + 15.0f);//A
    glVertex2f( metal.xr + 5.0f,metal.yr +15.0f);//B
    glVertex2f( metal.xr + 5.0f,metal.yr -20.0f);//C
    glVertex2f( metal.xr -5.0f,metal.yr -20.0f);//D
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);//  rotulo
    glVertex2f( metal.xr -5.0f ,metal.yr + 15.0f);//A
    glVertex2f( metal.xr + 5.0f,metal.yr +15.0f);//B
    glVertex2f( metal.xr + 5.0f,metal.yr + 5.0f);//C
    glVertex2f( metal.xr-5.0f,metal.yr + 5.0f);//D
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	 glColor3f(0.3f, 0.3f, 0.3f);//  topo
    glVertex2f( metal.xr + 0.0f ,metal.yr + 15.0f);//A
    glVertex2f( metal.xr -5.0f,metal.yr +15.0f);//B
    glVertex2f( metal.xr -4.0f,metal.yr + 19.0f);//C
    glVertex2f( metal.xr-3.0f,metal.yr +  20.0f);//D
    glVertex2f( metal.xr -2.0f,metal.yr +  21.0f);//D
    glVertex2f( metal.xr -1.0f,metal.yr +  22.0f);//D
    glVertex2f( metal.xr +0.0f,metal.yr +   22.0f);//D
    glVertex2f( metal.xr +2.0f,metal.yr +  21.0f);//D
    glVertex2f( metal.xr +3.0f,metal.yr +  20.0f);//D
    glVertex2f( metal.xr +4.0f,metal.yr +  19.0f);//D
    glVertex2f( metal.xr +5.0f,metal.yr +  15.0f);//D

    glEnd();

    glBegin(GL_QUADS);
	 glColor3f(0.3f, 0.3f, 0.3f);//  LIVRO
    glVertex2f( metal.xr  -2.7f ,metal.yr + 20.0f);//A
    glVertex2f( metal.xr + 3.3f,metal.yr +20.0f);//B
    glVertex2f( metal.xr + 3.3f,metal.yr + 24.0f);//C
    glVertex2f( metal.xr -2.7f,metal.yr + 24.0f);//D
	glEnd();

	 glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 0.6f);//  LIVRO
    glVertex2f( metal.xr -3.5f ,metal.yr + 23.0f);//A
    glVertex2f( metal.xr + 4.0f,metal.yr +23.0f);//B
    glVertex2f( metal.xr + 4.0f,metal.yr + 27.0f);//C
    glVertex2f( metal.xr -3.5f,metal.yr + 27.0f);//D
	glEnd();



}
void livro(void)
{
    glBegin(GL_QUADS);
   glColor3f(0.5f, 0.5f, 0.5f);//
    glVertex2f(papel.xr  -30.0f ,papel.yr  -40.0f);//A
    glVertex2f(papel.xr + 30.0f,papel.yr -40.0f);//B
    glVertex2f(papel.xr + 30.0f,papel.yr + 40.0f);//C
    glVertex2f(papel.xr -30.0f,papel.yr + 40.0f);//D


	glEnd();

	glBegin(GL_LINES );
   glColor3f(0.0f, 1.0f, 0.0f);//

    glVertex2f(papel.xr -13.0f ,papel.yr-40.0f);//A
    glVertex2f(papel.xr -13.0f,papel.yr +40.0f);//B
	glEnd();

    glRasterPos2f(-5 + papel.xr,-10 + papel.yr );
    glColor3f(0.2,0.5,0.1 );
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"A");



}


void help(){
    glColor3f(1,1,1);
    glScalef(0.5, 0.5, 0);
    glTranslatef(-1300, 0,0.0f);
    glRasterPos2f(0,0);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Pressione o botao Direito do Mouse para abrir o Menu");
    glRasterPos2f(0,-120);

    DesenhaTexto(GLUT_BITMAP_9_BY_15,"de Dificuldades e Esc para Voltar para o Menu principal.");

    glTranslatef(0, -150,0.0f);
    glRasterPos2f(0,-80);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Alterne entre as lixeiras com as setas UP e Down do teclado OU");
    glTranslatef(0, -150,0.0f);
    glRasterPos2f(0,-80);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Os botoes de 1 a 4 do teclado. Movimente a Lixeira com as setas Left e Right.");

    glTranslatef(0, -150,0.0f);
    glRasterPos2f(0,-150);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Tipos de Lixo:");

    glTranslatef(0, -150,0.0f);
    glScalef(1.5, 1.5, 0);

    plastico.xr = 95;
    plastico.yr = -80;

    garrafa();
    glScalef(0.5, 0.5, 0);

    glColor3f(1, 0, 0);
    glRasterPos2f(360,-200);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Garrafa (Plastico - Lixeira Vermelha)");

    glTranslatef(0, -200,0.0f);
    glScalef(2.5, 2.5, 0);

    papel.xr = 80;
    papel.yr = -80;

    livro();
    glColor3f(0, 0, 1);
    glScalef(0.5, 0.5, 0);
    glRasterPos2f(300,-200);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Livro (Papel - Lixeira Azul)");

    glTranslatef(0, -200,0.0f);
    glScalef(2, 2, 0);

    metal.xr = 80;
    metal.yr = -80;

    LATA_METAL();
    glScalef(0.5, 0.5, 0);
    glColor3f(1.0f, 0.7, 0.1);
    glRasterPos2f(300,-200);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Desodorante (Metal - Lixeira Amarela)");

    glTranslatef(0, -200,0.0f);
    glScalef(2, 2, 0);

    vidro.xr = 80;
    vidro.yr = -80;

    Caco_Vidro();
    glScalef(0.5, 0.5, 0);
    glColor3f(0, 1, 0);
    glRasterPos2f(300,-200);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Caco de Vidro (Vidro - Lixeira Verde)");



}

void coracao (void)
{

    glColor3f(1, 0, 0);//  topo
	glBegin(GL_TRIANGLE_FAN);

    glVertex2f( 0.0f , 15.0f);//A
    glVertex2f( -10.0f,15.0f);//B
    glVertex2f( -9.0f, 19.0f);//C
    glVertex2f(-8.0f,  20.0f);//D
    glVertex2f(-7.0f,  21.0f);//D
    glVertex2f(-6.0f,  22.0f);//D
    glVertex2f(-4.0f,  23.0f);//D
    glVertex2f(-2.0f,  24.0f);//D
    glVertex2f(2.0f,  24.0f);//D
    glVertex2f(4.0f,  23.0f);//D
    glVertex2f(6.0f,  22.0f);//D
    glVertex2f(7.0f,  21.0f);//D
    glVertex2f(8.0f,  20.0f);//D
    glVertex2f( 9.0f, 19.0f);//C
    glVertex2f( 10.0f,15.0f);//B

    glEnd();
    glTranslatef(18.0f,0.0f,0.0f);

	glBegin(GL_TRIANGLE_FAN);

    glVertex2f( 0.0f , 15.0f);//A
    glVertex2f( -10.0f,15.0f);//B
    glVertex2f( -9.0f, 19.0f);//C
    glVertex2f(-8.0f,  20.0f);//D
    glVertex2f(-7.0f,  21.0f);//D
    glVertex2f(-6.0f,  22.0f);//D
    glVertex2f(-4.0f,  23.0f);//D
    glVertex2f(-2.0f,  24.0f);//D
    glVertex2f(2.0f,  24.0f);//D
    glVertex2f(4.0f,  23.0f);//D
    glVertex2f(6.0f,  22.0f);//D
    glVertex2f(7.0f,  21.0f);//D
    glVertex2f(8.0f,  20.0f);//D
    glVertex2f( 9.0f, 19.0f);//C
    glVertex2f( 10.0f,15.0f);//B
    glEnd();

    glBegin(GL_TRIANGLES);
     glVertex2f( 10.0f , 15.0f);//A
    glVertex2f( -28.0f,15.0f);//B
    glVertex2f( -9.0f, -8.0f);//C

    glEnd();
}

void garrafa(void)
{
	glBegin(GL_QUADS);
    glColor3f(0.4f, 1, 1);//
//  corpo
    glVertex2f( plastico.xr + -10.0f ,plastico.yr + 15.0f);//A
    glVertex2f( plastico.xr + 10.0f,plastico.yr +15.0f);//B
    glVertex2f( plastico.xr + 10.0f,plastico.yr + -20.0f);//C
    glVertex2f( plastico.xr +-10.0f,plastico.yr + -20.0f);//D
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.1f, 1);//  rotulo
    glVertex2f( plastico.xr + -10.0f ,plastico.yr + 15.0f);//A
    glVertex2f( plastico.xr + 10.0f,plastico.yr +15.0f);//B
    glVertex2f( plastico.xr + 10.0f,plastico.yr + 0.0f);//C
    glVertex2f( plastico.xr +-10.0f,plastico.yr + 0.0f);//D
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.4f, 1, 1);//  topo
    glVertex2f( plastico.xr + 0.0f ,plastico.yr + 15.0f);//A
    glVertex2f( plastico.xr + -10.0f,plastico.yr +15.0f);//B
    glVertex2f( plastico.xr + -9.0f,plastico.yr + 19.0f);//C
    glVertex2f( plastico.xr +-8.0f,plastico.yr +  20.0f);//D
    glVertex2f( plastico.xr +-7.0f,plastico.yr +  21.0f);//D
    glVertex2f( plastico.xr +-6.0f,plastico.yr +  22.0f);//D
    glVertex2f( plastico.xr +-4.0f,plastico.yr +  23.0f);//D
    glVertex2f( plastico.xr +-2.0f,plastico.yr +  24.0f);//D
    glVertex2f( plastico.xr +0.0f,plastico.yr +   25.0f);//D
    glVertex2f( plastico.xr +2.0f,plastico.yr +  24.0f);//D
    glVertex2f( plastico.xr +4.0f,plastico.yr +  23.0f);//D
    glVertex2f( plastico.xr +6.0f,plastico.yr +  22.0f);//D
    glVertex2f( plastico.xr +7.0f,plastico.yr +  21.0f);//D
    glVertex2f( plastico.xr +8.0f,plastico.yr +  20.0f);//D
    glVertex2f( plastico.xr + 9.0f,plastico.yr + 19.0f);//C
    glVertex2f( plastico.xr + 10.0f,plastico.yr +15.0f);//B

    glEnd();

    glBegin(GL_QUADS);
	glColor3f(0.4f, 1, 1);//  LIVRO
    glVertex2f( plastico.xr + -2.7f ,plastico.yr + 20.0f);//A
    glVertex2f( plastico.xr + 3.3f,plastico.yr +20.0f);//B
    glVertex2f( plastico.xr + 3.3f,plastico.yr + 28.0f);//C
    glVertex2f( plastico.xr +-2.7f,plastico.yr + 28.0f);//D
	glEnd();

	 glBegin(GL_QUADS);
	glColor3f(0.1f, 0.1f, 1.0f);//  LIVRO
    glVertex2f( plastico.xr + -3.5f ,plastico.yr + 26.0f);//A
    glVertex2f( plastico.xr + 4.0f,plastico.yr +26.0f);//B
    glVertex2f( plastico.xr + 4.0f,plastico.yr + 30.0f);//C
    glVertex2f( plastico.xr +-3.5f,plastico.yr + 30.0f);//D
	glEnd();


}


void Caco_Vidro(void)
{

    glBegin(GL_QUADS);
    glColor3f(0.4f, 1, 1);//
    glVertex2f( -20.0f + vidro.xr , 0.0f + vidro.yr);//A
    glVertex2f( 00.0f+ vidro.xr,20.0f+ vidro.yr);//B
    glVertex2f( 20.0f+ vidro.xr, 0.0f+ vidro.yr);//C
    glVertex2f(0.0f+ vidro.xr, -20.0f+ vidro.yr);//D

    glEnd();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);//
    glLineWidth(3);

    glVertex2f( -6.20f + vidro.xr, -14.0f + vidro.yr);//A
    glVertex2f( 15.0f+ vidro.xr,5.0f+ vidro.yr);//B

    glVertex2f( 0.0f + vidro.xr, 10.0f+ vidro.yr);//A
    glVertex2f( -15.0f+ vidro.xr,-5.0f+ vidro.yr);//B
    glEnd();


}

void texto_pontos(){

    glColor3f(1.0,1.0,1.0);
    sprintf(texto, "Pontos: %d", pontos);
    glRasterPos2f(-100,-100);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,texto);
}

void desenha_button(float xCentro, float yCentro, float width, float height){
    glBegin(GL_QUADS);
    glVertex2f(xCentro - width/2,yCentro + height/2); //I
    glVertex2f(xCentro + width/2,yCentro + height/2); //I
    glVertex2f(xCentro + width/2,yCentro - height/2); //I
    glVertex2f(xCentro - width/2,yCentro - height/2); //I
    glEnd();

}

void dicas(){
    glPushMatrix();
    glTranslatef(0, -150,0.0f);
    glColor4f(1,1,1,0.5);
    Lixeira(-300,600,1,0,0);
    Lixeira(-100,600,1,0.7,0.1);
    Lixeira(100,600,0,0.5,1);
    Lixeira(300,600,0,1,0);

    glColor3f(1,1,1);
    glScalef(0.5, 0.5, 0);
    glTranslatef(-750, 1050,0.0f);
    glRasterPos2f(0,0);
    DesenhaTexto(GLUT_BITMAP_9_BY_15,"Plastico     Metal        Papel       Vidro");
    glPopMatrix();

}
void Desenha(void)
{

    float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    glPushMatrix();

    if(!rodandoJogo){
        paraJogo();
        rodandoJogo = 0;

        glColor3f(0.6,0,0.8);

        desenha_button(btn1_x,btn1_y,btn1_w, btn1_h);
        desenha_button(btn2_x,btn2_y,btn2_w, btn2_h);

        glColor3f(1,1,1);

        glRasterPos2f(-110,-60);
        DesenhaTexto(GLUT_BITMAP_9_BY_15,"Iniciar Jogo");

        glRasterPos2f(-60,-260);
        DesenhaTexto(GLUT_BITMAP_9_BY_15,"Ajuda");

    }else if(rodandoJogo == 1){
        texto_pontos();
    }else if(rodandoJogo == 2){
        glPushMatrix();
        help();
        glPopMatrix();

    }else{
        glPushMatrix();
        if(delay[7]<3000){
            glTranslatef(-200, -100.0f,0.0f);
            glColor3f(1,0, 0);

            DesenhaTextoStroke(GLUT_STROKE_ROMAN,"Perdeu!");

            delay[7]+= deltaTime;
        }else{
            rodandoJogo = 0;
            delay[7] = 0;
        }

        glPopMatrix();
    }

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1100.0f, 250,0.0f);
    desenha_terraRio();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1000.0f, 300,0.0f);

    desenha_rio();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3000.0f, 130,0.0f);
    agua_rio();

    glPopMatrix();
    glPushMatrix();

    glScalef(1.1,1,1);
    glTranslatef(0.0f, 330,0.0f);
    agua_rio();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1000.0f, 130,0.0f);
    agua_rio();

    glPopMatrix();
    glPushMatrix();

    glScalef(1.1,1,1);
    glTranslatef(-2000.0f, 330,0.0f);
    agua_rio();

    glPopMatrix();
    glPushMatrix();


    glScalef(1, 1, 0);
    glTranslatef(-400, 470.0f,0.0f);

    DesenhaTextoStroke(GLUT_STROKE_ROMAN,"Salve o Rio!");


     glPopMatrix();
    glPushMatrix();

     glTranslatef(200, 630.0f,0.0f);
    glScalef(2,2,0);
     for(int i=vidaRio; i>0; i--){
         glTranslatef(30, 0,0.0f);
         coracao();
     }

    glPopMatrix();
    glPushMatrix();


     if(rodandoJogo == 1){
        LIXOS();
    }

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -750.0f,0.0f);
    glColor3f(colorRio.r,colorRio.g,colorRio.b);
    glBegin(GL_LINE_STRIP);//linha que une as duas linhas principais no come�o
    glVertex2f(riop1.x, riop1.y);
    glVertex2f(riop2.x, riop2.y);
    glEnd();

    corLixeira();

    Lixeira(personagemXR,personagemYR,colorLixeira.r,colorLixeira.g,colorLixeira.b);
    if((dificuldade == 0) && rodandoJogo == 1)
        dicas();

    glFlush();

    glPopMatrix();
    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

	glFlush();
    glutPostRedisplay();

}

	void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;

	if(h == 0) h = 1;
	largura = w;
	altura = h;
	glViewport(0, 0, 1000, 700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-1000, 1000, -700, 700);
}

void selectLIXOS(){
    selectLixo[0] = randomizar(4);
    do{
        selectLixo[1] = randomizar(4);
    }while(selectLixo[1] == selectLixo[0]);

    do{
        selectLixo[2] = randomizar(4);
    }while((selectLixo[2] == selectLixo[0]) || ((selectLixo[2] == selectLixo[1])));

}

void selectCOORD(){
    coordLixo[0] = randomizar(8);

    do{
        coordLixo[1] = (randomizar(8) * pow(-1,randomizar(2)));
    }while(coordLixo[1] == coordLixo[0]);

    do{
        coordLixo[2] = (randomizar(8) * pow(-1,randomizar(2)));
    }while((coordLixo[2] == coordLixo[0]) || ((coordLixo[2] == coordLixo[1])));
}

void Inicializa (void)
{
    alfaAgua = 0.2;
    colorRio.r = 0.4f;
	colorRio.g = 0.8f;
    colorRio.b=  0.8f;

    colorLixeira.r = 1.0f;
	colorLixeira.g = 0.0f;
    colorLixeira.b=  0.0f;

    colorTerra.r = 1.0f;
    colorTerra.g = 0.7f;
    colorTerra.b = 0.1f;

    colorBack.r = 0.4f;
	colorBack.g = 0.7f;
    colorBack.b=  0.1f;



    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
	glClearColor(colorBack.r, colorBack.g, colorBack.b,1.0f);

    plastico.speed = 0.01;
    metal.speed = 0.01;
    vidro.speed = 0.01;
    papel.speed = 0.01;

    delay[0] =  0.0f;
    delay[1] =  0.0f;
    delay[2] =  0.0f;
    delay[3] =  0.0f;
    delay[4] =  0.0f;
    delay[5] =  0.0f;
    delay[6] =  0.0f;

    riop1.x=-1000;
    riop1.y=1150;
    riop2.x=1000;
    riop2.y=1150;

    selectLIXOS();
    selectCOORD();


    for(int i=0; i<=3 ; i++){
        printf("%d \n", selectLixo[i]);
    }
}

void MenuDificuldade(int op)
{
   switch(op) {
            case 0:
                    dificuldade = 0;
                    paraJogo();
                    rodandoJogo = 0;
                     break;
            case 1:
                     dificuldade = 1;
                     paraJogo();
                     rodandoJogo = 0;
                     break;
            case 2:
                     dificuldade = 2;
                     paraJogo();
                     rodandoJogo = 0;
                     break;

    }

    glutPostRedisplay();
}

// Gerenciamento do menu principal
void MenuPrincipal(int op)
{
}

// Criacao do Menu
void CriaMenu()
{
    int menu,submenu1;

    submenu1 = glutCreateMenu(MenuDificuldade);
	glutAddMenuEntry("Facil(Velocidade Baixa + dicas)",0);
	glutAddMenuEntry("Medio(Velocidade Baixa)",1);
	glutAddMenuEntry("Dificil(Velocidade Alta)",2);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Dificuldade",submenu1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Programa Principal
int main(void)
{
    srand((unsigned) time(&t));

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//modo de exibi��o com janela single buffered e padrao de cores rgba
	glutInitWindowPosition(10,10);//posi��o inicial da tela
	glutInitWindowSize(1000,700);//tamanho da tela
	glutCreateWindow("Salve o Rio");
	glutDisplayFunc(Desenha);//define a fun�ao Desenha como respons�vel por redesenhar a janela OpenGL sempre que necess�rio
	Inicializa();// conven��o do livro pra colocar oq deve ser executado antes de desenhar
	glutReshapeFunc(AlteraTamanhoJanela);//mudar tamannho. N�o usada efetivamente neste codigo
    glutMouseFunc(GerenciaMouse);//
    glutSpecialFunc(TeclasEspeciais);//
    glutKeyboardFunc(Teclado);
    glutMainLoop();//Loop glut

	return 0;
}
