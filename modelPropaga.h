
/* 
 * File:   modelPropaga.h
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 24 de Maio de 2020, 20:35
 */

#ifndef MODELPROPAGA_H
#define MODELPROPAGA_H

//CABEÇALHOS

// associadas ao avançar uma iteração
void taxa_dissemina (local *tab, pPessoa lista, int total);         //taxa de disseminação
void proba_recuperar (pPessoa p);                   // probabilidade de recuperação
void duracaoMax_Infec (pPessoa lista);          //duração máxima de infeção
void taxa_imunidade(pPessoa lista);               //taxa de imunidade
void avancar_iter (local *tab, pPessoa lista, int total);              //geral para avançar iteração


//associadas ao apresentar estatística
void distri_pessoasSalas (local *tab, pPessoa lista, int total);            //distribuição das pessoas por salas
void outras_taxas(pPessoa lista, double *taxaI, double *taxaS, double *taxaD, double *taxaR);   //taxas sem ser da imunidade
void idades_pessoas (pPessoa lista);                                                             //idades das pessoas
void apres_estatistica (local *tab, pPessoa lista, int total);                 //gerla para apresentar estatistica 
void mostra_taxas (pPessoa lista);

//------
void transf_NPessoas(pPessoa lista, local *tab, int total);         //transferir N pessoas
char menu();

//fim
void taxas_relatoFinal(pPessoa lista, double *taxaI, double *taxaS, double *taxaD, double *taxaR, int *contaIter); //taxas para o report.txt
void idades_relatoFinal (pPessoa lista);                //idades para o report.txt
void pess_espac_reltoFinal (local *tab, pPessoa lista, int total);          //escrever espaço e pessoas no relatório final
void criarFicheiro_ultimaPop (pPessoa lista);           //ficheiro novo apenas com dados da ultima população
void termina_simulacao (local *tab, pPessoa lista, int contaIter, int total);       //terminar


#endif /* MODELPROPAGA_H */

