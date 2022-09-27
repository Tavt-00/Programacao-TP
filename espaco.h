/* 
 * File:   espaco.h
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 16 de Maio de 2020, 17:47
 */

#ifndef ESPACO_H
#define ESPACO_H

//estrutua do local 
typedef struct sala local, *plocal;       //*plocal irá servir para ligar as estruturas das pessoas a esta, ligar pessoas a locais
struct sala{
    int id;                                               // id númerico do local
    int capacidade;                                // capacidade maxima
    int liga[3];                                   // id das ligações (-1 nos casos nao usados)
};

//  CABEÇALHOS
local *cria_vetor_dadosLocais(local *tab, int *total);
void valida_espaco(local *tab, int total);
void mostra_locais(local *tab, int total);


#endif /* ESPACO_H */

