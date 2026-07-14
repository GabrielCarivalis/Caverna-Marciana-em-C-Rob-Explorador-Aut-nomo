/*                 Trabalho 3 de Fundamentos da Programacao                  */
/*----------------------------------------------------------------------------*/
/* Autores: Gabriel Carivalis, Dagno Camargo, Carolina Bruno                  */ 
/*============================================================================*/
/*  Ra:     2882019,        2877503,           2812100                        */
/*============================================================================*/
#include "t3-2882019-2877503-2812100.h"
#include <stdio.h>
#include <stdlib.h>
/*============================================================================*/

//cria o decisor do robo
Decisor *criaDecisor(int altura, int largura)
{
    int i, j;

    // aloca espaço de memórias para as matrizes e vetores utilizados, alem do d_robo
    Decisor *d_robo = (Decisor *)malloc(sizeof(Decisor));

    //matriz_memoria guarda lugares desconhecidos, seguros, visitados e locais com lava
    d_robo->matriz_memoria = (int **)malloc(sizeof(int *) * altura);
    //matriz guarda o valor de n_lava em todas posicoes
    d_robo->n_lava_memoria = (int **) malloc (sizeof(int*) * altura);
    //guarda os ultimos movimentos feitos pelo robo
    d_robo->vetor_movimento = (int *)malloc(sizeof(int) * altura * largura);

    //altura e largura da matriz
    d_robo->altura = altura;
    d_robo->largura = largura;

    //alocacao das duas matrizes com todas posicoes desconhecidos
    for (i = 0; i < altura; i++)
    {
        d_robo->matriz_memoria[i] = (int *)malloc(sizeof(int) * largura);
        d_robo->n_lava_memoria[i] = (int*) malloc(sizeof(int)*largura);

        for (j = 0; j < largura; j++)
        {
            d_robo->matriz_memoria[i][j] = DESCONHECIDO;
            d_robo->n_lava_memoria[i][j] = DESCONHECIDO;
        }
    }

    d_robo->vetor_movimento[0] = 0;

    //guarda quantos numeros de movimentos ele fez
    d_robo->topo = 0;

    //flag que sinaliza se o robo esta retornando (depois de encontrar agua) ou nao
    d_robo->retornando = 0;

    //as 4 primeiras casas são sempre seguras
    d_robo->matriz_memoria[0][0] = SEGURO;
    d_robo->matriz_memoria[0][1] = SEGURO;
    d_robo->matriz_memoria[1][0] = SEGURO;
    d_robo->matriz_memoria[1][1] = SEGURO;

    return d_robo;
}

//desaloca as memorias que foram alocadas no decisor
void destroiDecisor(Decisor *d_robo)
{
    for (int i = 0; i < d_robo->altura; i++)
    {
        free(d_robo->matriz_memoria[i]);
        free(d_robo->n_lava_memoria[i]);
    }

    free(d_robo->matriz_memoria);
    free(d_robo->n_lava_memoria);
    free(d_robo->vetor_movimento);
    free(d_robo);
}

//calcula as distâncias a partir do ponto (0,0) usando BFS (Algoritmo de ondas)
void calculaDistancia(Decisor *d_robo)
{
    int inicio = 0, fim = 0;

    // Fila que armazena as coordenadas que ainda precisam ser processadas pelo algoritmo.
    Coordenada *fila = (Coordenada *)malloc(sizeof(Coordenada) * d_robo->altura * d_robo->largura);

    // Insere a entrada na fila e atribui distância zero nela, pois está é a entrada da caverna
    fila[fim].x = 0;
    fila[fim].y = 0;
    fim++;

    d_robo->matriz_memoria[0][0] = 0;

    // Continua enquanto existirem posições aguardando processamento
    while (inicio < fim)
    {
        // Retira a próxima posição da fila.
        Coordenada atual = fila[inicio];
        inicio++;

        // Verifica a casa acima.
        if (atual.y > 0 && (d_robo->matriz_memoria[atual.y - 1][atual.x] == SEGURO || d_robo->matriz_memoria[atual.y - 1][atual.x] == VISITADO))
        {
            d_robo->matriz_memoria[atual.y - 1][atual.x] = d_robo->matriz_memoria[atual.y][atual.x] + 1;

            fila[fim].x = atual.x;
            fila[fim].y = atual.y - 1;
            fim++;
        }

        // Verifica a casa abaixo.
        if (atual.y + 1 < d_robo->altura && (d_robo->matriz_memoria[atual.y + 1][atual.x] == SEGURO || d_robo->matriz_memoria[atual.y + 1][atual.x] == VISITADO))
        {
            d_robo->matriz_memoria[atual.y + 1][atual.x] = d_robo->matriz_memoria[atual.y][atual.x] + 1;

            fila[fim].x = atual.x;
            fila[fim].y = atual.y + 1;
            fim++;
        }

        // Verifica a casa à esquerda.
        if (atual.x > 0 && (d_robo->matriz_memoria[atual.y][atual.x - 1] == SEGURO || d_robo->matriz_memoria[atual.y][atual.x - 1] == VISITADO))
        {
            d_robo->matriz_memoria[atual.y][atual.x - 1] = d_robo->matriz_memoria[atual.y][atual.x] + 1;

            fila[fim].x = atual.x - 1;
            fila[fim].y = atual.y;
            fim++;
        }

        /* Verifica a casa à direita. */
        if (atual.x + 1 < d_robo->largura && (d_robo->matriz_memoria[atual.y][atual.x + 1] == SEGURO || d_robo->matriz_memoria[atual.y][atual.x + 1] == VISITADO))
        {
            d_robo->matriz_memoria[atual.y][atual.x + 1] = d_robo->matriz_memoria[atual.y][atual.x] + 1;

            fila[fim].x = atual.x + 1;
            fila[fim].y = atual.y;
            fim++;
        }
    }

    // Libera a fila temporária utilizada
    free(fila);
}

// Reconstrói o caminho da posição atual até (0,0) e armazena no vetor_movimento
int reconstruirCaminho(Decisor *d_robo, Coordenada pos)
{
    Coordenada atual = pos;
    int distancia_atual, direcao;

    // Distância da posição atual até a entrada calculada pela BFS
    distancia_atual = d_robo->matriz_memoria[atual.y][atual.x];

    //A posição 0 guarda quantos movimentos ainda serão executados no retorno
    d_robo->vetor_movimento[0] = distancia_atual;

    while (distancia_atual > 0)
    {
        direcao = 0;


        /* Procura entre os vizinhos, uma casa com distância uma unidade menor
        ao encontrar tal distãncia salva a direção e atualiza a posição atual.*/
        if (atual.y > 0 &&
            d_robo->matriz_memoria[atual.y - 1][atual.x] == distancia_atual - 1)
        {
            direcao = DIR_MOVIMENTO_CIMA;
            atual.y--;
        }
        else if (atual.y + 1 < d_robo->altura &&
                 d_robo->matriz_memoria[atual.y + 1][atual.x] == distancia_atual - 1)
        {
            direcao = DIR_MOVIMENTO_BAIXO;
            atual.y++;
        }
        else if (atual.x > 0 &&
                 d_robo->matriz_memoria[atual.y][atual.x - 1] == distancia_atual - 1)
        {
            direcao = DIR_MOVIMENTO_ESQUERDA;
            atual.x--;
        }
        else if (atual.x + 1 < d_robo->largura &&
                 d_robo->matriz_memoria[atual.y][atual.x + 1] == distancia_atual - 1)
        {
            direcao = DIR_MOVIMENTO_DIREITA;
            atual.x++;
        }

        //Salva a direção no índice correspondente à distância atual
        d_robo->vetor_movimento[distancia_atual] = direcao;
        //Passa para a próxima distância do caminho mais próxima da entrada.
        distancia_atual--;
    }
    //Finaliza a função indicando que o robô possui o menor caminho reconstruiído e armazenado
    return 1;
}

/*conta em todas posicoes se ha lava ou desconhecidos, eh usado para o calculo
que verifica se ha lavas novas em lugares nao conhecidos*/
void contaVizinhos(Decisor* d_robo, int i, int j, int* qtd_desconhecido, int* qtd_lava)
{
    //contador para a quantidade de desconhecidos e lavas
    *qtd_desconhecido = 0;
    *qtd_lava = 0;

    /*verifica as casas vizinhas para contabilizar se eh
    desconhecido ou lava*/
    if (i > 0)
    {
        if (d_robo->matriz_memoria[i-1][j] == DESCONHECIDO)
        (*qtd_desconhecido)++;

        else if (d_robo->matriz_memoria[i-1][j] == LAVA)
        (*qtd_lava)++;
    }

    if (i + 1 < d_robo->altura)
    {
        if (d_robo->matriz_memoria[i+1][j] == DESCONHECIDO)
        (*qtd_desconhecido)++;

        else if (d_robo->matriz_memoria[i+1][j] == LAVA)
        (*qtd_lava)++;
    }

    if (j > 0)
    {
        if (d_robo->matriz_memoria[i][j-1] == DESCONHECIDO)
        (*qtd_desconhecido)++;

        else if (d_robo->matriz_memoria[i][j-1] == LAVA)
        (*qtd_lava)++;
    }

    if (j + 1 < d_robo->largura)
    {
        if (d_robo->matriz_memoria[i][j+1] == DESCONHECIDO)
        (*qtd_desconhecido)++;

        else if (d_robo->matriz_memoria[i][j+1] == LAVA)
        (*qtd_lava)++;
    }
}

//faz a conta para deduzir a quantidade de lavas ao redor
void deduzirLava(Decisor* d_robo)
{
    //flag que identifica se houve mudanca na matriz
    int mudou = 1;
    int qtd_desconhecido, qtd_lava;

    while(mudou)
    {
        //flag inicializa em 0 para verificar se houve mudanca
        mudou = 0;

        //percorre a matriz
        for (int i = 0; i < d_robo->altura; i++)
        {
            for (int j = 0; j < d_robo->largura; j++)
            {
                //caso nao seja conhecido aquela posicao, vai para a proxima iteracao
                if (d_robo->n_lava_memoria[i][j] == DESCONHECIDO)
                    continue;

                //chama a funcao que conta a quantidade de lavas e desconhecidos nos vizinhos
                contaVizinhos(d_robo, i, j, &qtd_desconhecido, &qtd_lava);

                //verifica se as posicoes ao redor sao seguras
                if (qtd_lava == d_robo->n_lava_memoria[i][j] && qtd_desconhecido > 0)
                {
                    if (i > 0 && d_robo->matriz_memoria[i-1][j] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i-1][j] = SEGURO;
                        mudou = 1;
                    }

                    if (i+1 < d_robo->altura && d_robo->matriz_memoria[i+1][j] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i+1][j] = SEGURO;
                        mudou = 1;
                    }

                    if (j > 0 && d_robo->matriz_memoria[i][j-1] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i][j-1] = SEGURO;
                        mudou = 1;
                    }

                    if (j+1 < d_robo->largura && d_robo->matriz_memoria[i][j+1] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i][j+1] = SEGURO;
                        mudou = 1;
                    }
                }

                //verifica se as posicoes ao redor sao lava
                else if (qtd_lava + qtd_desconhecido == d_robo->n_lava_memoria[i][j] && qtd_desconhecido > 0)
                {
                    if (i > 0 && d_robo->matriz_memoria[i-1][j] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i-1][j] = LAVA;
                        mudou = 1;
                    }

                    if (i+1 < d_robo->altura && d_robo->matriz_memoria[i+1][j] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i+1][j] = LAVA;
                        mudou = 1;
                    }

                    if (j > 0 && d_robo->matriz_memoria[i][j-1] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i][j-1] = LAVA;
                        mudou = 1;
                    }

                    if (j+1 < d_robo->largura && d_robo->matriz_memoria[i][j+1] == DESCONHECIDO)
                    {
                        d_robo->matriz_memoria[i][j+1] = LAVA;
                        mudou = 1;
                    }
                }
            }
        }
    }
}

int explorar(Decisor *d_robo, Coordenada pos, int n_lava)
{
    //marca como visitado a posicao atual
    d_robo->matriz_memoria[pos.y][pos.x] = VISITADO;

    //guarda na matriz n_lava_memoria a quantidade de lavas ao redor
    d_robo->n_lava_memoria[pos.y][pos.x] = n_lava;
    //chama a funcao para deduzir a quantidade de lavas ou seguros ao redor
    deduzirLava (d_robo);

    //variavel que indica a direcao que ele vai andar
    int direcao;

    /*verifica se as casas ao redor sao seguras e vai em direcao a essa casa,
    na ordem: esquerda, cima, baixo, direita. Essa ordem foi testada de todas as formas,
    atingindo maior eficiencia percentual nessa ordem.*/
    if (pos.x-1 >= 0 && d_robo->matriz_memoria[pos.y][pos.x-1] == SEGURO)
    {
        d_robo->vetor_movimento[d_robo->topo] = DIR_MOVIMENTO_ESQUERDA;
        d_robo->topo++;
        return DIR_MOVIMENTO_ESQUERDA;
    }

    else if (pos.y-1 >= 0 && d_robo->matriz_memoria[pos.y-1][pos.x] == SEGURO)
    {
        d_robo->vetor_movimento[d_robo->topo] = DIR_MOVIMENTO_CIMA;
        d_robo->topo++;
        return DIR_MOVIMENTO_CIMA;
    }

    else if (pos.y+1 < d_robo->altura && d_robo->matriz_memoria[pos.y+1][pos.x] == SEGURO)
    {
        d_robo->vetor_movimento[d_robo->topo] = DIR_MOVIMENTO_BAIXO;
        d_robo->topo++;
        return DIR_MOVIMENTO_BAIXO;
    }

    else if (pos.x+1 < d_robo->largura && d_robo->matriz_memoria[pos.y][pos.x+1] == SEGURO)
    {
        d_robo->vetor_movimento[d_robo->topo] = DIR_MOVIMENTO_DIREITA;
        d_robo->topo++;
        return DIR_MOVIMENTO_DIREITA;
    }

    //caso nao haja seguros nos vizinhos
    else
    {
        /*se o vetor movimento ainda existir, ou seja, o topo > 0,
        entao ele começa a retornar procurando por lugares seguros*/
        if (d_robo->topo > 0)
        {
            direcao = d_robo->vetor_movimento[d_robo->topo - 1];
            d_robo->topo--;

            if (direcao == DIR_MOVIMENTO_CIMA)
                return DIR_MOVIMENTO_BAIXO;

            if (direcao == DIR_MOVIMENTO_BAIXO)
                return DIR_MOVIMENTO_CIMA;

            if (direcao == DIR_MOVIMENTO_ESQUERDA)
                return DIR_MOVIMENTO_DIREITA;

            if (direcao == DIR_MOVIMENTO_DIREITA)
                return DIR_MOVIMENTO_ESQUERDA;
        }

        /*essa parte do codigo ocorre quando ja nao tem mais pra onde ir, entao ele chuta
        uma direcao possivel */
        if (pos.y + 1 < d_robo->altura && d_robo->matriz_memoria[pos.y+1][pos.x] != LAVA)
            return DIR_MOVIMENTO_BAIXO;

        //senão tenta outra direção possivel
        else if (pos.x + 1 < d_robo->largura && d_robo->matriz_memoria[pos.y][pos.x+1] != LAVA)
            return DIR_MOVIMENTO_DIREITA;

        //uso como ultimo recurso, realmente uma questao de sorte :(  //
        return DIR_MOVIMENTO_DIREITA;
    }
}

//retorna o proximo movimento do robo
int proximoMovimento(Decisor *d_robo, Coordenada pos, int agua, int n_lava)
{
    int direcao;

    /*se o robo estiver retornando, ele faz o caminho de volta com base no
    algoritmo de ondas*/
    if (d_robo->retornando)
    {
        direcao = d_robo->vetor_movimento[d_robo->vetor_movimento[0]];
        d_robo->vetor_movimento[0]--;
        return direcao;
    }

    //executa enquanto ele nao achar a agua, ou seja, o caminho de ida
    else if (!agua)
    {
        direcao = explorar(d_robo, pos, n_lava);
        return direcao;
    }

    //faz o calculo do caminho de volta, no momento em que acha a agua
    else
    {
        d_robo->matriz_memoria[pos.y][pos.x] = VISITADO;

        /*chama a funcao que calcula a distancia para ele voltar
        ao ponto (0,0) de inicio*/
        calculaDistancia(d_robo);

        //caso de algum bug, mas apenas de verificacao
        if (!reconstruirCaminho(d_robo, pos))
            return 0;

        //sinaliza na flag que o robo esta retornando
        d_robo->retornando = 1;

        //inicia o caminho de volta
        direcao = d_robo->vetor_movimento[d_robo->vetor_movimento[0]];
        d_robo->vetor_movimento[0]--;

        return direcao;
    }
}
