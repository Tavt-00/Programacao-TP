/*
 *File: espaço.c
 * Author: Rafael Ribeiro - 2019131989
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "espaco.h"


//CRIAR ARRAY DINAMICO de estruturas do tipo LOCAL onde vamos por a informação do ficheiro que o utilizador indica

local *cria_vetor_dadosLocais(local *tab, int *total) //devolve um array com estruturas do tipo local 
{
    char fichUtilizador [20]; //guardar aqui o ficheiro que o utilizador escolhe
    int tam, i;
    
    local *aux = NULL; //array auxliar para alocar memoria
    FILE *f;

 
    
      printf("\nQual o ficheiro do espaco onde vai realizar a simulacao? ");
      scanf("%s", fichUtilizador);
    
      f = fopen(fichUtilizador, "rb"); //abrir ficheiro indicado

      if (f == NULL) 
      {
          fprintf(stderr, "Erro ao abrir o ficheiro: %s, coloque de novo!\n", fichUtilizador);  
          return 0; 
      }
      
    fseek(f, 0, SEEK_END); //ir para o fim do ficheiro para conseguirmos calcular o tamanho
    tam = ftell(f); //indica o numero de bytes do ficheiro
    *total = tam / sizeof (local);

    aux = malloc(sizeof (local) * (*total)); //alocar espaço num vetor de maneira a ter espaço para o sizeof da estrutura, as vezes que forem precisas dependendo do total (local * total) 

    fseek(f, 0, SEEK_SET); //voltar para cima para começarmos a ler o ficheiro e passar para o array criado

    if (aux == NULL) 
    {
        return tab;
    }
    tab = aux; // se correr bem, entao vamos apontar tab para onde aponta aux, visto que aux é apenas auxiliar

    for (i = 0; i<*total; i++)
    {
        fread(&tab[i], sizeof (local), 1, f);        //lemos do ficheiro, 1 elemento de cada vez e colocamos cada um no array, conforme avançamos o indice
    }

    fclose(f);

    return tab;
}



//VALIDAR a organização do espaço: cada local deve ter ID UNICOS e POSITIVOS e as LIGAÇÕES devem estar corretas

void valida_espaco(local *tab, int total) {
    int i, j, k, x, h, t;
    int idLocal_Encontrar;

    for (i = 0; i < total; i++)
    {

        //verificar se cada ID é positivo
        if (tab[i].id < 0) 
        {
            printf("\nErro na validacao dos locais, há IDs não positivos \n");
            return;
        }

        //verificar se IDs são unicos
        for (j = i + 1; j < total; j++) // ao fazermos j=i+1, estamos com o i sempre num certo indice e o j estara no seguinte, assim podemos comparar sempre o id do anterior com o proximo
        {
            if (tab[i].id == tab[j].id) //se o ID anterior é igual ao seguinte, há IDs repetidos 
            {
                printf("\nErro na validacao dos locais, ha IDs repetidos \n");
                return;
            }
        }
    }

    //verificar ligações corretas 
    for (t = 0; t < total; t++)
    {
        int idOriginal = tab[t].id;
        
        for (k = 0; k < 3 && tab[t].liga[k] > 0 ; k++) //em cada estrura, ver cada ID de ligação no vetor "liga" que cada estrutura tem
        {
                idLocal_Encontrar = tab[t].liga[k]; // para depois  procurar então a sala que diz ter uma ligaçao, para ver se ela também tem uma ligação à primeira        
            for (x = 0; x < total; x++) //começar a ver de novo o array de estruturas para encontrar a sala que supostamente também tem uma ligação à primeira
            {

            int flag_ligaCorreta = 0;
            
            //printf("Tabx %d\n", tab[x].id);
            //printf("Local a encontrar %d\n", idLocal_Encontrar);        
            if (tab[x].id == idLocal_Encontrar && tab[x].id != idOriginal) //se encontrar o local e se for diferente do primeiro local
            {
            
                for (h = 0; h < 3 ; h++) //percorrer o array de ligações dele para ver se também tem uma ligação ao outro
                    {
                        if (tab[x].liga[h] == tab[t].id) //se encontrou o id da outra, ligação certa
                        {
                            flag_ligaCorreta = 1;
                        }
                    }
 
                if (flag_ligaCorreta == 1)
                    break; //já confirmou que os locais estão bem ligados, não vale a pena continuar com as outros
                
                else 
                {
                    printf("\nErro na validacao dos locais, ha ligacoes mal feitas \n");
                    return;
                }
            }
          }
        }
    }
}


// Função para fazer PRINTS de todas as estruturas (LOCAIS) que estão no array dinamico

void mostra_locais(local *tab, int total) 
{
    int i, j;

    printf("\nConstituicao de cada local do espaço\n");

    for (i = 0; i < total; i++)
    {
        printf("\n----ID do local: %d\n", tab[i].id);
        printf("\nCapacidade total do local: %d\n", tab[i].capacidade);

        for (j = 0; j < 3; j++) //em cada estrutura, apresentar o array de ligações
        {
            printf("\nVai ligar a: %d\n", tab[i].liga[j]);
        }
    }
}










