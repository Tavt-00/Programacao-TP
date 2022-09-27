/* 
 * File:   pessoas.c
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 19 de Maio de 2020, 18:21
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoas.h"   //necessário para colocar as pessoas nas estruturas dos locais
#include "utils.h"      //necessário para pedir um numero
#include "espaco.h"
#include"modelPropaga.h"

//criar LISTAS LIGADAS constituidas pelas pessoas

pPessoa cria_listas_dadosPessoas(pPessoa lista, pPessoa tabListas[])
{    
    char fichUtilizador[20];
    int i;
    FILE *f;

    pPessoa novo; //ponteiro para o novo nó
    pPessoa auxiliar;
    
     printf("Qual o ficheiro referente ao grupo de pessoas escolhido? ");
     scanf("%s", fichUtilizador);

    f = fopen(fichUtilizador, "r"); //para ler e de texto

    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro %s\n", fichUtilizador);
        return 0;
    }

    while(!feof(f)) 
    {
        //arranjar espaço em memoria para o novo nó para um tamanho da estrutura "pessoa" 
        novo = malloc(sizeof (pessoa));
        
        if (novo == NULL) 
        {
            printf("Erro na alocacao de memoria\n");
            fclose(f); //não alocou, fechar logo o ficheiro
            return 0;
        }

        if (fscanf(f, "%s %d %c", novo->identificador, &novo->idade, &novo->estado) == 3) //ler linha a linha, a string(por isso não leva &) do indentificador, um inteiro da idade e um char do estado, portanto como fscanf retorna o numero de variáveis lidos, só avançamos se tiver lido esses 3 
        {
            if (novo->idade < 0)
            {
                printf("Erro: idades erradas, não podem ser negativas\n");
                return 0;
            }

        
             if (novo->estado == 'D') //se o estado da pessoa for 'D', quer dizer que é doente, então temos de guardar há quantos dias está
             {
                fscanf(f, "%d", &novo->diasDoente);
             }
            
           /*novo->prox = NULL;      

            if (lista == NULL)
            {                                       // Lista vazia
                lista = novo;
            }
        
            else 
            {
                auxiliar = lista;

                while (auxiliar->prox != NULL)
                {
                    auxiliar = auxiliar->prox;
                }
                auxiliar->prox = novo;
            }*/
            
            novo->prox = lista;
            lista = novo;

          }
    } //enquanto não chegarmos ao fim do ficheiro

    return lista; //retornar a li sta já preenchida
    fclose (f);
}



//MOSTRAR lista ligada

void mostra_pessoas(pPessoa lista)
{
    while (lista != NULL)
    {
        printf("\nNome: %s\n Idade: %d\n Estado: %c\n Esta colocada no local com ID %d \n", lista->identificador, lista->idade,lista->estado, lista->localRandom->id);

        if (lista->estado == 'D')
        {
            printf(" Esta doente ha %d dias\n", lista->diasDoente);
        }
        lista = lista->prox; 
    }
}


//Dar LOCAIS  a PESSOAS

//temos de colocar as pessoas, aleatoriamente, em locais (cada uma num), desde que satisfaça a capacidade de cada local
//como é dito no enunciado, se existerem pessoas a mais para a capacidade total do espaço (conjunto de locais) escolhidos, estas pessoas não participam na simulação
//assim, é necessário saber a capacidade de cada local, para saber a capacidade máxima total do espaço
//com isto, temos de guardar todas as capacidades e quando colocarmos uma pessoa num local, garantir que a sua capacidade não está a ser ultrapassada e consequentemente a do espaço

void selecionaLocal_paraPessoa(pPessoa lista, local *tab, int total) {

    int *totalCapacidades; //array onde vamos guardar cada capacidade de cada local que vamos ler ao array dinâmico de locais
    int posiRandom, i,j, flag = 0;

    totalCapacidades = malloc(sizeof (total));


    if (totalCapacidades != NULL) 
    {


        for (i = 0; i < total; i++) //guardar cada capacidade de cada local do array de locais(tab), neste array de capacidades
        {
            totalCapacidades[i] = tab[i].capacidade;
        }
        
        while (lista != NULL)       //percorrer a lista de pessoas
        {
            for (j = 0; j < total; j++)
            {
                if (totalCapacidades[j] != 0)               //poderá não haver qaundo voltarmos ao incio deste while pois antes de voltarmos, decrementamos uma vaga no array de capacidades
                {
                    flag = 1;       //confirmamos que há esaço para colocar mais uma nova pessoa
                    break;            //encontramos uma vaga, podemos parar de procurar
                }
            }

            if (!flag)      // se não houver espaço paramos logo
            {  
                printf("Nao ha espaço para colocar mais pessoas neste espaco\n");
                break;
            }
            do
            {
                posiRandom = intUniformRnd(0, total - 1); //receber numero aleatório que como metemos de 0 a total do array dos locais -1, acabamos por receber o indice (a posição) de uma estrutura local                    

            }while (totalCapacidades[posiRandom] == 0); //pedir sempre um numero aleatório (posiRandom) enquanto nessa mesma posiçao no array das capacidade for =0, porque quer dizer que essa está indisponivel 

            lista->localRandom = &tab[posiRandom];       //inserir a pessoa no local que a posiRandom conseguiu encontrar livre, estamos a dizer que esta pessoa vai receber uma estrutura local do array de locais na posição conseguida 
            totalCapacidades[posiRandom] -= 1;              //tirar uma vaga pois foi ocupada com uma pessoa
            lista = lista->prox;
        }
    }
    else
    {
        printf("Erro na alocacao de memoria");
        return;
     }
}   

//Adicionar DOENTE no inico da lista ligada
//utilizador diz o ID do local, o identificador, a idade e os dias de infeção


pPessoa adic_doente (pPessoa p, local *tab, pPessoa tabListas[], int total)
{
    pPessoa novo, aux, auxConta, novoArListas = NULL;
    int id, i, flagEspaco=0, flagIdade =0, indice, contaPessoasDest = 0, capacidade = 0, vagas, flagExiste=0 ;
            
     novo = malloc(sizeof (pessoa));
     
      if (novo == NULL)
      {
        printf("Erro na alocação de memória\n");
        return p;
      }
      
      else 
      {
  
            //perguntar a sala onde colocar o doente e confirmar que existe e capacidade
            while (flagEspaco == 0 || flagExiste == 0)
            {      
                contaPessoasDest = 0;
                
                printf ("\nIndique o ID do local para onde vai esta pessoa: ");     
                scanf ("%d", &id);
                
                for (i=0; i<total; i++)         //econtrar onde fica a sala no array e ver a sua capacidade
                {
                    if (tab[i].id == id)
                    {
                        capacidade = tab[i].capacidade;
                        indice = i;
                        flagExiste = 1;
                        break;
                    }
                }
                
                if (flagExiste == 0)
                {
                    printf ("\nEste ID nao existe, volte a colocar um de novo");
                }
                
                else
                    {
                    auxConta = p;

                    while (auxConta != NULL)            //contar quantas estão na sala destino
                    {

                         if (id == auxConta->localRandom->id)      
                         {
                               contaPessoasDest++;
                         }
                     auxConta = auxConta->prox;
                    }       

                    vagas = capacidade - contaPessoasDest;

                    if (vagas >= 1)         //verificar que há capacidade para por este doente
                    {
                        flagEspaco = 1;
                    }

                    else
                    {
                        printf ("\nErro, nao existe espaco suficente para colocar esta pessoas\n");   
                    }
              }
            }          
         
            
            //colocar nó e caracterizar a pessoa
            
          aux = p;   
          while (aux->prox != NULL) 
          {
            aux = aux->prox;
          }
          aux->prox = novo;
            
            novo->estado = 'D';         //sabemos que é doente, colocar o estado para doente
           
            printf("Indique o identificador da pessoa: ");
            scanf("%s", novo->identificador);
            
            while (flagIdade == 0)
            {
                printf("Indique a  idade da pessoa: ");
            scanf("%d", &novo->idade);
            
                if (novo->idade <= 0)
                {
                    printf("Erro: idades erradas, não podem ser negativas\n");
                    flagIdade =0;
                }
                else
                    flagIdade=1;
            
            }
            printf("Indique ha quantos dias a pessoa esta infetada: ");
            scanf("%d", &novo->diasDoente);
            
            novo->localRandom = &tab[indice];
            
            novo->prox = NULL;
            
            //meter este doente novo também no array de listas para o recua 
            
            for (int j = 0; j<3; j++)
            {
                novoArListas  =malloc(sizeof (pessoa));
                
                if (novoArListas == NULL)
                {
                    printf ("\nErro a alocar o novo doente no array de listas\n");
                    return p;
                }
                
                novoArListas->prox = p;
                p = novoArListas;               //colocar no inicio
            }
            
            
      } 
     return p;
}



/// NOVA FUNCIONALIDADE

//criar array de ponteiros para listas do tipo pessoa
//onde iremos colocar cada população de cada 3 dias
void prepara_arrayListas (pPessoa lista, pPessoa tabListas[])         //chamar logo depois de meter as pessoas nas listas
{
    pPessoa novo = NULL, aux;
    int i;
    
    aux = lista;
    while (aux != NULL)
    {
        for (i=0; i<3; i++)     //alocar até 3 listas 
        {
            novo = malloc (sizeof (pessoa));                                                        
            
            if (novo == NULL)
            {
                printf ("\nErro a alocar memória\n");
                return;
            }
            
            novo->prox = tabListas[i];              //alocar 
            tabListas[i] = novo;
            novo = NULL;                                    //criámos o espaço necessário e precisamos de garantir que quando formos colocar os nós das pessoas noutras funções, esteja a NULL, esteja uma lista vazia 
        }
        
        aux = aux->prox;        
    }
}


//recuar os dias e preparar a lista que vai ser a atual do dia que o utilizador desejar
pPessoa recuar_iteracoes (pPessoa lista, pPessoa tabListas[], int *diasRecuados)
{
    pPessoa aux = NULL;
    pPessoa  auxA, auxB;
    int dias, flagDias = 0;
    
    do
    {
        printf("\nQuantos dias deseja recuar, ate 3? ");
        scanf ("%d", &dias);
        
        if (dias <= 3)
        {
            flagDias = 1;
        }
        else
        {
          printf ("\nSo e possivel recuar 1, 2 ou 3 dias! Coloque de novo.\n");
        }
    }while (flagDias == 0);
    
    //se só formos recuar um dia e visto que o nosso array vai do 0 ao 2, por ordem do mais antigo, respetivamente, a populção que queremos ir buscar é a do tab[0]
    if (dias == 1)
    {   
        
             //fazer com o tab[2] seja o nosso atual agora
        
        auxA = tabListas[2];
        auxB = lista;
        
        while (auxA != NULL)
        {

            strcpy(auxB->identificador, auxA->identificador);
            auxB->estado = auxA->estado;
            if (auxB->estado == 'D')
            {
                  auxB->diasDoente= auxA->diasDoente;
            }
            auxB->idade = auxA->idade;
            auxB->localRandom = auxA->localRandom;


            auxA = auxA->prox;
            auxB = auxB->prox;

       }
        
        //retirar tab[2] do array de ponteiros e colocar o 1 no local do 2, sendo o dia anterior mais recente
        
        aux = tabListas[2];                                                          // Iremos descer no array para tornar o que estava no índice 1 como o dia anterior mais recente, andando para baixo. Para o ponteiro do índice 0 não se perder, colocamos lá a lista que estava no 2
        tabListas[1] = tabListas[0];                                            //andar para baixo
        tabListas[2] = tabListas[1];    
        tabListas[0] = aux;
    }
    
    
    if (dias == 2)      //recuar dois dias, já deve ser a tab[1] que deve ser a atual
    {

           //dia atual vai ser o 1, vamos então tirar meter o 0 como dia anterior mais recente dos dias anteriores
        
        auxA = tabListas[1];
        auxB = lista;
        
        while (auxA != NULL)
        {

            strcpy(auxB->identificador, auxA->identificador);
            auxB->estado = auxA->estado;
            if (auxB->estado == 'D')
            {
                  auxB->diasDoente= auxA->diasDoente;
            }
            auxB->idade = auxA->idade;
            auxB->localRandom = auxA->localRandom;


            auxA = auxA->prox;
            auxB = auxB->prox;

        }
        
        
        aux = tabListas[2];         //guardar o ponteiro 2 no aux para depois garantir que o mais recente é o 2 e expulsar o que estava no 0
        
        tabListas[2] = tabListas[0];       //o que está no zero passa a ser o 2, pois passa a ser o dia anterior mais recente, visto que expulsamos o 0 e 1 
        tabListas[0] = aux;                  
        
    }
    
    if (dias == 3)
    {
         //se recuarmos 3, apenas necessitamos de colocar a população do indice 0 como a atual, o array ficará vazio pois fomos vuscar o mais antigo
        auxA = tabListas[0];
        auxB = lista;
        
        while (auxA != NULL)
        {

            strcpy(auxB->identificador, auxA->identificador);
            auxB->estado = auxA->estado;
            if (auxB->estado == 'D')
            {
                  auxB->diasDoente= auxA->diasDoente;
            }
            auxB->idade = auxA->idade;
            auxB->localRandom = auxA->localRandom;


            auxA = auxA->prox;
            auxB = auxB->prox;

        }
    }
    
    (*diasRecuados) = dias;
    return lista;
}



//em cada iteração, meter a nova lista desse dia no array de listas. a nova terá de ir para o tab[2]
void array_listasNovas (pPessoa lista, pPessoa tabListas[])       
{
    pPessoa novo = NULL, auxA, auxB;
           
        novo = tabListas[0];       //já não precisamos da informação do indice 0, mas precisamos da sua lista para o novo ter uma para conseguir receber a informação a seguir
        auxA = lista;
        auxB = novo;
        
        //copiar a lista des cada dia para o array de ponteiros
        
        while (auxA != NULL)
        {
            strcpy(auxB->identificador, auxA->identificador);
            auxB->estado = auxA->estado;
            if (auxB->estado == 'D')
            {
                  auxB->diasDoente= auxA->diasDoente;
            }
            auxB->idade = auxA->idade;
            auxB->localRandom = auxA->localRandom;

            auxA = auxA->prox;
            auxB = auxB->prox;
        }
    
    tabListas[1] = tabListas[2];            //andar com todos para cima para reservar espaço no tab[2] que irá ser o mais recente
    tabListas[0] = tabListas[1];
    tabListas[2] = novo;  
}