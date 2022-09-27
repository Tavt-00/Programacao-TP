/* 
 * File:   main.c
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 16 de Maio de 2020, 17:02
 */

#include <stdio.h>
#include <stdlib.h>
#include "espaco.h"
#include "pessoas.h"
#include "utils.h"
#include "modelPropaga.h"

int main(int argc, char** argv) {
    
    int total = 0;
    local *espaco = NULL; //vetor que vai ser dinamico, onde vamos colocar as varias estruturas (locais) que vao ser lidas do ficheiro (espaço) que o utilizador indicar
    pPessoa lista = NULL;       //lista ligada do tipo pPessoa
    char opc;      
    int diasRecuados = 0, contaIter = 0;
    pPessoa tabListas[3] = {NULL};        //array de ponteiros
    
    
    
    //Começar por iniciar a fase de preparação, pedindo ficheiros, lendo-os e criando arrays e listas
    printf("\n");
    printf ("\n             ***Iniciar fase de preparacao da simulacao do virus***\n");
    
    espaco = cria_vetor_dadosLocais(espaco, &total);                   //"espaco" é um vetor e vai receber o vetor dinamico, que é devolvido pela função, com a informacao do ficheiro escolhido pelo utilizador, com as estruturas
    valida_espaco(espaco, total); 
    lista = cria_listas_dadosPessoas (lista, tabListas);
    prepara_arrayListas (lista, tabListas);   
    initRandom();
    selecionaLocal_paraPessoa  (lista, espaco, total);
    
    do
    { 
        opc = menu();
        switch(opc)
        {
        case '1':
           array_listasNovas (lista, tabListas);           //guardar o dia anterior no array de listas antes de anvançarmos de novo
           avancar_iter (espaco, lista, total);
           contaIter++;
           printf ("\n**--Dia atual da simulacao: %d--**\n",contaIter);    //mostrar dias que avançamos, mas se recuamos, também é necessário tirar esses dias
           break;
        case '2':
           lista = recuar_iteracoes (lista, tabListas, &diasRecuados);
           contaIter = contaIter - diasRecuados;                                                                    //mostrar dias que avançamos, mas se recuamos, também é necessário tirar esses dias
           printf ("\n**--Dia atual da simulacao: %d--**\n",contaIter);                                //voltar a mostrar os dias certos
           break;
        case '3':
           apres_estatistica(espaco, lista, total);
           break;
        case '4':
           adic_doente (lista, espaco, tabListas, total);
           break;
        case '5':
           transf_NPessoas(lista, espaco, total);
           break;
        case '6':
           mostra_locais (espaco, total);
           break;
        case '7':
           mostra_pessoas(lista); 
           break;
         case '8':
           termina_simulacao (espaco, lista, contaIter, total);
           return 0;           //parar programa se escolhido
        }
    }while(opc!='9');

    
    free (lista);
    free(espaco);
   
    return (EXIT_SUCCESS);
}

