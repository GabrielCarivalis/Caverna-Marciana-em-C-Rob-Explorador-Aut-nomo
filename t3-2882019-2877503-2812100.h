/*----------------------------------------------------------------------------*/
/* Autores: Gabriel Carivalis, Dagno Camargo, Carolina Bruno                  */ 
/*============================================================================*/
/*  Ra:     2882019,        2877503,           2812100                        */
/*============================================================================*/
#ifndef __PROTOTIPO_H
#define __PROTOTIPO_H

#include "game_state.h"

/*============================================================================*/

//constantes usadas para guardar o local na memoria
#define DESCONHECIDO -5
#define SEGURO       -4
#define VISITADO     -3
#define LAVA        -2

/*============================================================================*/

typedef struct
{
    int** matriz_memoria; //matriz que guarda cada posicao como uma das constantes
    int*  vetor_movimento; //vetor que guarda as ultimas direcoes do robo
    int   altura; //altura do tabuleiro
    int   largura; //largura do tabuleiro
    int   topo; //empilha a quantidade de posicoes do vetor_movimento
    int   retornando; //flag que sinaliza se esta no caminho de volta
    int** n_lava_memoria; //matriz que guarda o numero de n_lava conhecidos
} Decisor;

/*============================================================================*/

Decisor* criaDecisor (int altura, int largura); //funcao que cria o decisor do robo
void destroiDecisor (Decisor* d_robo); //funcao que destroi o decisor do robo
void calculaDistancia (Decisor* d_robo); //funcao que calcula a distancia do ponto (0,0) ate posicao atual
int reconstruirCaminho (Decisor* d_robo, Coordenada pos); //reconstroi o caminho que tem a menor distancia para voltar
int explorar (Decisor* d_robo, Coordenada pos, int n_lava); //funcao usada para explorar o tabuleiro na ida
void contaVizinhos (Decisor* d_robo, int i, int j, int* desconhecido, int* bomba); //conta se os vizinhos sao lava ou desconhecido
void deduzirLava (Decisor* d_robo); //usada para deduzir onde eh lava e onde eh seguro
int proximoMovimento (Decisor* d_robo, Coordenada pos, int agua, int n_lava); //retorna o proximo movimento do robo

/*============================================================================*/
#endif
