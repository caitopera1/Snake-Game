#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>

#define TAMANHO_X 30 //Minimo de 20
#define TAMANHO_Y 20 //Minimo de 15
int tamanho_maximo_cobra = (TAMANHO_X - 2) * (TAMANHO_Y - 2); // Tamanho Maximo que a cobra pode ocupar em pixels(pontos)

//--------------------- Estrutura de Ponto ---------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    int x;
    int y;

} ponto; // Comeca de (0,0)

//--------------------- Estrutura do Mundo do jogo ---------------------------------------------------------------------------------------------------------v

typedef struct
{
    char player[50];
    int score;
    int max_frutos;
    int run;
    int map[TAMANHO_Y][TAMANHO_X];
    int num_fruto;

} world;

//--------------------- Estrutura da Cobra ---------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct 
{
    ponto cabeca;
    ponto corpo[(TAMANHO_X - 2) * (TAMANHO_Y - 2)]; // Tamanho maximo da cobra;
    int direcao; //Direacao da cobra        1: Para Cima    -1: Para Baixo    2: Para Direita    -2: Para Esquerda
    int tamanho_cauda; //Tamanho que o programa ira considerar do vetor da cauda
    ponto pos_antiga_cabeca; // Auxilia na hora de guiar o proximo ponto que a cauda ira assumir 

} snake;

//--------------------- Protótipo das funcoes ---------------------------------------------------------------------------------------------------------------------------------------------------

void draw(world jogo);
world verificaColisao (snake cobra, world jogo);
world geraMapaInicial (world jogo);
world geraFrutoNoMapa(world jogo);
world atualizaCabecaMapa(snake cobra, world jogo);
world limpaCaudaMapa (snake cobra, world jogo);
world atualizaCorpoCobraNoMapa (snake cobra, world jogo);
snake correCobra(snake cobra);
snake atualizaCaudaCobra(snake cobra);
snake direcionaCobra(snake cobra, char tecla);

//--------------------- Funcao Principal Snake Game ---------------------------------------------------------------------------------------------------------------------------------------------------]

int main ()
{

    //Variáveis Locais
    int i, j, k;
    int temp;
    int tecla; //Tecla pressionada
    int aux_bugs = 0; //Auxiliar de Conserto de Bugs 
    srand(time(NULL));
    snake cobra;
    world jogo;
    world jogo_inicial;
    
    //Define o world inicial
    jogo.max_frutos = 3;
    jogo.score = 2;
    jogo.num_fruto = 0;
    jogo.run = 1;
    cobra.cabeca.x = 10;
    cobra.cabeca.y = 10; 
    cobra.direcao = 1;
    cobra.tamanho_cauda = 2;
    cobra.corpo[0].x = cobra.cabeca.x - 1;
    cobra.corpo[0].y = cobra.cabeca.y;
    
    //Pega Nome Player
    printf("Digite Seu Nome : ");
    scanf("%s", &jogo.player);

    jogo = geraMapaInicial(jogo);
    
    draw(jogo);
    

    //Lógica do Jogo
    while (jogo.run != 0)
    {    
        //pos_antiga_cabeca = pos nova que vai entrar no vetor da cauda
        cobra.pos_antiga_cabeca.x = cobra.cabeca.x;
        cobra.pos_antiga_cabeca.y = cobra.cabeca.y;
        


        //Corre Cobra
        cobra = correCobra(cobra);
        
        //Verifica keyBoard Hit
        if (kbhit()){
            tecla = getch();
            cobra = direcionaCobra(cobra, toupper(tecla));
        }

        //Gera Fruto no mapa
        jogo = geraFrutoNoMapa(jogo);

        //Verifica Colisoes
        jogo = verificaColisao(cobra, jogo);
        cobra.tamanho_cauda = jogo.score;

        //Atualiza Mapa com Cabeca da cobra
        jogo = atualizaCabecaMapa(cobra, jogo);
         
        //Atualiza vetor da cauda da cobra com posicoes novas
        cobra = atualizaCaudaCobra(cobra);

        //Retira posicoes marcadas como 2 e transformam para 0
        jogo = limpaCaudaMapa(cobra, jogo);

        //Atualiza Mapa com Corpo da Cobra
        jogo = atualizaCorpoCobraNoMapa(cobra, jogo);

        //Mostra Mapa
        system("cls");
        draw(jogo);
        Sleep(30);
    
        //Bug
        aux_bugs++;
        k = 0;
        for (i = 0; i < TAMANHO_Y; i++)
            for (j = 0; j < TAMANHO_X; j++)
                if (jogo.map[i][j] == 3) 
                    k++;

        if (k != jogo.num_fruto)
            jogo.num_fruto = k;   
        k=0;    
        
        if (jogo.run == 0){
            
        }
    }

    //Fim de Jogo
    system("cls");

    printf("\n"
 " _____          __  __ ______    ______      ________ _____\n"  
 "/ ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \n" 
 "| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__)\n" 
 "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  /\n" 
 "| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \n" 
 " \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\n\n\n");

    printf("JOGADOR : %s\n", jogo.player);
    printf("SCORE : %d\n\n\n", jogo.score);
                                                            
            
    system("pause");

    return 0;
}



//--------------------- Mostra Mapa ---------------------------------------------------------------------------------------------------------

void draw(world jogo)
{
    int i, j;
    
    for (i = 0; i < TAMANHO_Y; i++){
        
        for (j = 0; j < TAMANHO_X; j++)
            switch(jogo.map[i][j]){
                case(0):
                    printf("  ");
                    break;

                case(1):
                    printf("# ");
                    break;
                
                case(2):
                    printf("O ");
                    break;
            
                case(5):
                    printf("! ");
                    break;
                
                case(3):
                    printf("* ");
                    break;

                default:
                    break;
            }
        
        printf("\n"); 
    }
    printf("PLAYER : %s\n", jogo.player);
    printf("SCORE  : %d\n", jogo.score - 2);
}




//--------------------- Gera Mapa Inicial ------------------------------------------------------------------------------------------------------------------------------

world geraMapaInicial (world jogo)
{
    int i, j;
    
    for (i = 0; i < TAMANHO_Y; i++)
        for (j = 0; j < TAMANHO_X; j++){
           
            if (i == 0 || j == 0 || i == TAMANHO_Y - 1 || j == TAMANHO_X - 1)
                jogo.map[i][j] = 5;
            else    
                jogo.map[i][j] = 0;
        }
    
    return (jogo);
}



//--------------------- Atualiza Mapa com Corpo da Cobra ------------------------------------------------------------------------------------------------------------------------------

world atualizaCorpoCobraNoMapa (snake cobra, world jogo)
{
    int i, j, k;
    
    for (i = 0; i < TAMANHO_Y; i++)
            for (j = 0; j < TAMANHO_X; j++)
                for (k = 0; k < cobra.tamanho_cauda; k++)
                    if (j == cobra.corpo[k].x && i == cobra.corpo[k].y)
                        jogo.map[i][j] = 2;

    return (jogo);
}


//--------------------- Limpa posicoes Antigas da cauda que ja passaram ------------------------------------------------------------------------------------------------------------------------------

world limpaCaudaMapa (snake cobra, world jogo)
{
    int i, j, k;
    
    for (i = 0; i < TAMANHO_Y; i++)
            for (j = 0; j < TAMANHO_X; j++)
                for (k = 0; k < tamanho_maximo_cobra; k++)
                    if (k >= cobra.tamanho_cauda && jogo.map[i][j] == 2)
                        jogo.map[i][j] = 0;

    return (jogo);
}



//--------------------- Atualiza Mapa com Cabeca da Cobra ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

world atualizaCabecaMapa(snake cobra, world jogo)
{
    int i, j;
    
    for (i = 0; i < TAMANHO_Y; i++)
            for (j = 0; j < TAMANHO_X; j++){
                if (j == cobra.cabeca.x && i == cobra.cabeca.y)
                    jogo.map[i][j] = 1;
            }

    return (jogo);
}


//--------------------- Atualiza Cobra com movimento da cobra ---------------------------------------------------------------------------------------------------------------------------------------------------

snake correCobra(snake cobra)
{
    if (cobra.direcao == 1)
	        cobra.cabeca.y--;

        else if (cobra.direcao == -1)
            cobra.cabeca.y++;

        else if (cobra.direcao == 2)
            cobra.cabeca.x++;

        else if (cobra.direcao == -2)
            cobra.cabeca.x--;

    return (cobra);
}



//--------------------- Direciona a Cobra de acordo com a tecla ---------------------------------------------------------------------------------------------------------------------------------------------------

snake direcionaCobra(snake cobra, char tecla)
{
    if (tecla == 'A'){
                if (cobra.direcao == 2)
                    cobra.direcao = cobra.direcao;
                else
                    cobra.direcao = -2;
            }

            if (tecla == 'D'){
                if (cobra.direcao == -2)
                    cobra.direcao = cobra.direcao;
                else
                    cobra.direcao = 2;
            }

            if (tecla == 'W'){
                if (cobra.direcao == -1)
                    cobra.direcao = cobra.direcao;
                else
                    cobra.direcao = 1;
            }

            if (tecla == 'S'){
                if (cobra.direcao == 1)
                    cobra.direcao = cobra.direcao;
                else
                    cobra.direcao = -1;
            }
    
    return (cobra);
}


//--------------------- Atualiza vetor da cauda da cobra com pos novas ------------------------------------------------------------------------------------------------------------------------------

snake atualizaCaudaCobra(snake cobra)
{
    int temp;

    temp = cobra.tamanho_cauda;
    
    while (temp >= 0){
        if (temp == 0) {
            cobra.corpo[temp].x = cobra.pos_antiga_cabeca.x;
            cobra.corpo[temp].y = cobra.pos_antiga_cabeca.y;
            break;
        }

        else{
            cobra.corpo[temp].x = cobra.corpo[temp - 1].x;
            cobra.corpo[temp].y = cobra.corpo[temp - 1].y;
            temp--;
        }
    }

    return (cobra);
}

//--------------------- Verifica Se Houve Colisao ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

world verificaColisao (snake cobra, world jogo)
{
    int i, j;

    for(i = 0; i < TAMANHO_Y; i++)
        for(j = 0; j < TAMANHO_X; j++)
            if (cobra.cabeca.x == j && cobra.cabeca.y == i){
                if(jogo.map[i][j] == 5){       //Colisao com a Borda
                    jogo.run = 0;
                    break;     
                }

                else if(jogo.map[i][j] == 3){  //Colisao com fruto
                    jogo.score++;
                    jogo.num_fruto--;
                    break;  
                }

                else if(jogo.map[i][j] == 2)
                    jogo.run = 0;
                    break;
            }

    return (jogo);
}



//--------------------- Gera Fruto No Mapa ---------------------------------------------------------------------------------------------------------------------------------------------------

world geraFrutoNoMapa(world jogo)
{
    int pontoX_fruta, pontoY_fruta;

    pontoX_fruta = (rand() % (TAMANHO_X - 5)) + 2;
    pontoY_fruta = (rand() % (TAMANHO_Y - 5)) + 2; // Num de 1 a 17 é o mesmo que um numero de 0 a 16 + 1;

    if (jogo.num_fruto < jogo.max_frutos){
            jogo.map[pontoY_fruta][pontoX_fruta] = 3;
            jogo.num_fruto++;
        }

    return (jogo);
}
