
/* 
 * File:   pessoas.h
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 19 de Maio de 2020, 18:22
 */

#ifndef PESSOAS_H
#define PESSOAS_H
#include "espaco.h"

// estrutura que identifica uma pessoa
typedef struct Pessoas pessoa, *pPessoa;   //pPessoa- ponteiro para lista ligada
struct Pessoas
{
    char identificador[30];  
    int idade;
    char estado;                                                                        // S - saudável  D - doente  I - Imune
    int diasDoente;                                                                     //no caso de estado = "D" doente, guardar há quantos dias foi infetado
    plocal    localRandom;                                                      //usar o ponteiro criado na estrutura "local" para agora atribuir um local aleatorio a cada pessoa
    pPessoa prox;                                                                   //ponteiro para apontar para o proximo elemento da lista ligada  
};    


//CABEÇALHOS
pPessoa cria_listas_dadosPessoas(pPessoa lista, pPessoa tabListas[]);
void mostra_pessoas(pPessoa lista);
void selecionaLocal_paraPessoa (pPessoa lista, local *tab,  int total);
pPessoa adic_doente (pPessoa p, local *tab, pPessoa tabListas[], int total);

//nova funcionalidade
void prepara_arrayListas (pPessoa lista, pPessoa tabListas[]);
pPessoa recuar_iteracoes (pPessoa lista, pPessoa tabListas[], int *diasRecuados);
void array_listasNovas (pPessoa lista, pPessoa tab[]);


#endif /* PESSOAS_H */
