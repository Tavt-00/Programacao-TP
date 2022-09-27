/* 
 * File:   modelPropaga.c
 * Author: Rafael Ribeiro - 2019131989
 *
 * Created on 23 de Maio de 2020, 17:54
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "espaco.h"
#include "pessoas.h"   
#include "utils.h"     
#include "modelPropaga.h"

//calcular TAXA DE DISSEMINÇÃO, acontece por interação, por dia
//se sabemos que uma pessoa doente infeta 0.05 do local onde está, temos de saber quantos doentes temos numa sala para saber quanto é que ficam o total de infetados pois cada um vai infetar 0.05

void taxa_dissemina (local *tab, pPessoa lista, int total)
{
    pPessoa aux, aux2;
    int contaDoentes = 0, contaPessoas = 0;
    int i, resulDoentesPdia = 0;
    
    for (i=0; i<total; i++)    //começar num local, calcular a propagação e só depois saltar para novo local
    {
          
        resulDoentesPdia=0;
        contaPessoas=0;
        contaDoentes =0;
        aux = lista;
        while (aux != NULL)
        {
            if (tab[i].id == aux->localRandom->id)        //começar no primeiro local e percorrer todas as pessoas que estejam nesse local (que tenham o mesmo id), sempre que tiveram somamos mais uma pessoa a esse local e vemos se ela é doente para saber o total de doentes na sala para sabermos como fica a infeção no final do dia
            {
                contaPessoas++;  //incrementar pessoas
                
                if (aux->estado == 'D')
                {
                    contaDoentes++;     //incrementar doentes para saber o total deles
                }
            }
            aux = aux->prox;
        }
           
        resulDoentesPdia = ((int) ((float) contaPessoas * 0.5)) * contaDoentes; 
                   
        aux2 = lista;
       //dos agora doentes, ver aqueles que ainda têm o estado = S para o passar a D
        while (aux2 != NULL && resulDoentesPdia > 0)       //parar so quando a lista terminar e começar no inicio de todos os doentes e mudar-lhes o estado. quando já não houver doentes, parar
        {
            if (tab[i].id == aux2->localRandom->id)       //começar a ver novamente na mesma sala
            {
              if(probEvento(0.5))
              {
                    if (aux2->estado == 'S')
                    {
                        aux2->estado = 'D';
                        aux2->diasDoente = 0;             
                        resulDoentesPdia--;
                    }
              }
            }
            aux2 = aux2->prox;
        }

    }
    
}


//PROBABILIDADE DE RECUPERAÇÃO de doente, acontece por iteração, por dia

void proba_recuperar (pPessoa p)
{
    float resulProba;  

        if (p->estado == 'D')
        {
            resulProba = 1 /  (float) p->idade;         //fórmula: 1/idade da pessoa
        
            if (probEvento(resulProba))       //chamar função dada que diz se a probabilidade de ser 1/idade existe ou não
            {
                p->estado = 'R';        // se a probabilidade de se recuperar conferir, mudar estado para R
            }           
        }
        p = p->prox;
}



//calcular DURAÇÃO MÁXIMA de INFEÇÃO

//5+1 dia por cada dezena de anos

void duracaoMax_Infec (pPessoa lista)
{
    float resulDurac;
    int idade = 0, nDiasDoente = 0;
   
        if (lista->estado == 'D')
        {
            idade = lista->idade;
            nDiasDoente = lista->diasDoente;
            
            resulDurac = ( (float) idade / 10) + 5;
        
            if ((int) resulDurac <= nDiasDoente)    //se está com mais dias de doente do que os dias que precisa para recuparar, então quer dizer que já recuperou 
            {
                lista->estado = 'R';
            }
            
        }      
    
}
    

//TAXA de IMUNIDADE

void taxa_imunidade(pPessoa lista)
{
    //probabilidade 20% de ficar imune
            if (lista->estado == 'R')
            {
                if (probEvento(0.2))
                {
                    lista->estado = 'I';            // I de Imune
                    printf ("\nO doente %s ficou imune\n", lista->identificador);
                }
            }

            lista = lista->prox;        //ERRO, não devia aqui estar
    
}

//Avançar iteração

void avancar_iter (local *tab, pPessoa lista, int total)
{
    
     taxa_dissemina (tab, lista, total);        //colocar primeiro doentes pela taxa de infeção
    
     while (lista != NULL)
     {
         if (lista->estado == 'D')
         {
             lista->diasDoente++;
         }
         
        proba_recuperar (lista);           //chamar funções que fazem parte de um dia de propagação
        duracaoMax_Infec (lista);
        taxa_imunidade (lista);
        lista = lista->prox;
     }
}

//------------------------------------------------------------


//calcular DISTRIBUIÇAI de PESSOAS por sala

void distri_pessoasSalas (local *tab, pPessoa lista, int total)
{
    int i, contaPessoas=0;
    pPessoa aux;
    
    printf ("\n**--Distribuicao das pessoa por sala--**\n");

    for (i=0; i<total; i++)
    {
        aux = lista;        //colocar o aux sempre no inicio
        contaPessoas =0;        //colocar de novo o contador a 0 para conseguir fazer a contagem da proxima sala corretamente
        while (aux != NULL)         //percorrer toda a lista para ver se há alguma pessoa que tem o id do primeiro local e fazer o mesmo para os seguintes locais
        {
            if (tab[i].id == aux->localRandom->id)      //se essa pessoa estiver na sala, então incrementar o nº de pessoas dessa sala e apresentar antes do fim do for para podermos apresentar para todas
            {
                contaPessoas ++;
            }
            aux = aux->prox;        //avançar na lista para ver todas as pessoas
        }
        printf ("\n--Local com o ID %d--\n", tab[i].id);
        printf ("\nEsta sala tem %d pessoas ", contaPessoas);
        printf ("\n");
        
    }
   
}

//taxas
void outras_taxas(pPessoa lista, double *taxaI, double *taxaS, double *taxaD, double *taxaR)
{
    int contaTpessoas = 0, contaI = 0, contaD = 0, contaS = 0, contaR = 0;
    pPessoa aux;
    
    *taxaI = *taxaS = *taxaD = *taxaR = 0;
            
    aux = lista;
    while (aux != NULL)
    {
        contaTpessoas++;
        if(aux->estado == 'D')
        {
            contaD++;
        }
        else if (aux->estado == 'I')
        {
            contaI++;
        }
        else if (aux->estado == 'S')
        {
            contaS++;
        }
        else if (aux->estado == 'R')
        {
            contaR++;
        }
       
        aux = aux->prox;
    }
    
    *taxaD = contaD / (double) contaTpessoas;
    *taxaI = contaI / (double) contaTpessoas;
    *taxaS = contaS / (double) contaTpessoas;
    *taxaR = contaR / (double) contaTpessoas;
   
     printf ("\n**--Taxas--**\n");
     printf ("\n-Num total de %d pessoas-\n", contaTpessoas);
     if (*taxaI > 0 )
     {
        printf ("\nA taxa de pessoas imunes: %.2f", *taxaI);
        printf ("\nHa %d imunes", contaI);
     }
    
     if (*taxaD > 0 )
     {
        printf ("\nA taxa de pessoas doentes: %.2f", *taxaD);
        printf ("\nHa %d doentes", contaD);
     }
     
     if (*taxaS > 0)
     {
        printf ("\nA taxa de pessoas saudaveis: %.2f", *taxaS);
        printf ("\nHa %d saudaveis", contaS);
     }
     
     if (*taxaR > 0)
     {
        printf ("\nA taxa de pessoas recuperados: %.2f", *taxaR);
        printf ("\nHa %d recuperados", contaR);
     }
     printf ("\n");
}

void idades_pessoas (pPessoa lista)
{
    int idadeVelhoD = 0, idadeNovoD = 0, idadeVelhoI = 0, idadeNovoI = 0, idadeNovoS = 0, idadeVelhoS = 0, tempoDoente = 0, idadeMaisDoente = 0;
    int somaDoentes = 0, somaRecu = 0,somaImune = 0, somaSau = 0, numSau = 0, numDoentes = 0, numRecu = 0, numImune = 0, contaPessoas = 0;
    float mediaD = 0, mediaR = 0, mediaI = 0, mediaS = 0;
    pPessoa aux; 
    
  
    while (lista!=NULL)
    {
        if (lista->estado == 'D')
        {
            idadeNovoD = lista->idade;
            somaDoentes = somaDoentes + lista->idade;
            numDoentes ++;
            
            if (tempoDoente < lista->diasDoente)
            {
                idadeMaisDoente = lista->idade;
                tempoDoente = lista->diasDoente;
            }
            
            if (idadeVelhoD < lista->idade)
            {
                idadeVelhoD = lista->idade;
            }
            
            if (idadeNovoD > lista->idade)
            {
                idadeNovoD = lista->idade;
            }
        }
        
        if (lista->estado == 'S')
        {
            idadeNovoS = lista->idade;
            somaSau = somaSau + lista->idade;
            numSau++;
            
            if (idadeVelhoS < lista->idade)
            {
                idadeVelhoS = lista->idade;
            }
            
            if (idadeNovoS > lista->idade)
            {
                idadeNovoS = lista->idade;
            }
        }
        
        if (lista->estado == 'R')
        {
            somaRecu = somaRecu + lista->idade;
            numRecu++;
        }
        
        if (lista->estado == 'I')
        {
            idadeNovoI = lista->idade;
            somaImune = somaImune + lista->idade;
            numImune++;
            
            if (idadeVelhoI < lista->idade)
            {
                idadeVelhoI = lista->idade;
            }
            
            if (idadeNovoI > lista->idade)
            {
                idadeNovoI = lista->idade;
            }
        }
    
        lista = lista->prox;
    }
   
    
    mediaD = somaDoentes / (float) numDoentes;
    mediaR = somaRecu / (float) numRecu;
    mediaI = somaImune / (float) numImune;
    mediaS = somaSau / (float) numSau;
    
    
     printf ("\n**--Estatistica das idades da populacao--**\n");
     if (idadeVelhoD>0)
     {
        printf ("\nA idade da pessoa mais velha doente e de: %d", idadeVelhoD);
     }
     else 
         printf ("\nNao existe pessoa mais velha doente,");
     
     if (idadeNovoD > 0)
     {
        printf ("\nA idade da pessoa mais nova doente e de: %d\n", idadeNovoD);
     }
     else
         printf ("\nNao existe pessoa mais nova doente.\n");
     
       if (idadeVelhoS>0)
     {
        printf ("\nA idade da pessoa mais velha saudavel e de: %d", idadeVelhoS);
     }
     else 
         printf ("\nNao existe pessoa mais velha saudavel.");
     
     if (idadeNovoS > 0)
     {
        printf ("\nA idade da pessoa mais nova saudavel e de: %d\n", idadeNovoS);
     }
     else
         printf ("\nNao existe pessoa mais nova saudavel.\n");
     
     if (idadeVelhoI > 0)
     {
        printf ("\nA idade da pessoa mais velha imune e de: %d", idadeVelhoI);
     }
     else
         printf ("\nNao existe pessoas mais velha imune.");
     if (idadeNovoI > 0)
     {
        printf ("\nA idade da pessoa mais nova imune e de: %d\n", idadeNovoI);
     }
     else
         printf ("\nNao existe pessoa mais nova imune.\n");
     
     if (mediaD > 0)
     {
        printf ("\nA media das idades das pessoas doentes e de: %.2f", mediaD);
     }
     
     if (mediaS > 0)
     {
        printf ("\nA media das idades das pessoas saudaveis e de: %.2f", mediaS);
     }
     
     if (mediaI > 0)
     {
         printf ("\nA media das idades das pessoas imunes e de: %.2f", mediaI);
     }
     
     if (mediaR > 0)
     {
        printf ("\nA media das idades das pessoas recuperadas e de: %.2f \n", mediaR);
     }
     
     if (idadeMaisDoente > 0 && tempoDoente > 0)
     {
        printf ("\nA pessoa que esta ha mais tempo doente tem %d anos e esta doente ha %d dias\n", idadeMaisDoente, tempoDoente);
     }
}

//Mostrar estatisca

void apres_estatistica (local *tab, pPessoa lista, int total)
{
   double taxaS, taxaD, taxaI, taxaR;
    
    printf ("\n            ***Apresentar estatistica***                    \n");
    printf ("----------------------------------------------------------------\n");
    
    //taxas
     outras_taxas (lista, &taxaI, &taxaS, &taxaD, &taxaR);
    
    idades_pessoas (lista); 
    
      //distribuição das pessoas por salas
    distri_pessoasSalas (tab, lista, total);
}

//------------------------------------------------- -----------



//transferir N pessoas de um local para outro
//ID origem, ID destino e N pessoas indicado pelo utilizador
//verificar que esses IDS têm ligação
//se sim, escolher as N pessoas aleatóriamente do local origem 


void transf_NPessoas(pPessoa lista, local *tab, int total)
{
    int idOrigem, idDestino, contaPessoasOrig = 0, contaPessoasDest = 0, numTransf = 0, capaMaxDest = 0, contaPessMover=0, vagas = 0;
    int flagIDpos=0, flagIDori = 0, flagIDdest = 0, flagLiga = 0, flagLocalPessoas = 0, flagMover = 0, flagSucesso = 0, flagMover2 = 0 ;
    int i, j, t, x, h;
    pPessoa aux, aux2, aux3;
    
    
    
    //validar IDs indicados e se sala origem tem pessoas para ser transferidas 
    do
    {
        
        printf ("\nIndique o ID do local origem: ");
        scanf ("%d", &idOrigem);
        
        //vereficar que IDs origem existe

            //id origem
        for (i=0; i<total && tab[i].id != idOrigem ; i++);      //percorrer apenas para ver onde fica o i

        if (i == total)     //se o i chegar ao total, quer dizer que saio do ciclo sem encontrar o id origem
        {
            printf ("\nErro, o ID do local de origem nao existe!\n");
            return;
         }

         else        //quer dizer que encontrou o local origem e podemos fazer o mesmo para o destino
         {
            flagIDori = 1;      //id origem encontrado
         }

            //id destino
        printf ("\nIndique o ID do local destino: ");
        scanf ("%d", &idDestino);

        
        //verficar ids positvos 
       
        if (idDestino < 0 || idOrigem <0)     
         {
              printf("\nErro na validacao dos locais, IDs nao sao todos postivos! \n");
              return;
         }
        else
        {
             flagIDpos = 1;         //ids são postivos, avançamos
        }
             
            
            //verificar que ID destino existe caso afirmativo, continuar
                for (j=0; j<total && tab[j].id != idDestino; j++); 

                if (j == total)
                {
                    printf ("\nErro, o ID do local de destino nao existe!\n");
                    return;
                }
                else 
                {
                    flagIDdest = 1;         //id destino encontrado
                    
                    //verificar que têm ligação direta
                    
                    for (t = 0; t<3; t++)       //percorrer o array de ligações do ID destino e ver se tem lá o ID origem
                    {
                        if (tab[j].liga[t] == tab[i].id)
                        {    
                            flagLiga = 1;
                            break;      //break, podemos parar de procurar
                        }
                    }
                    if (flagLiga == 0)
                    {
                        printf ("\nErro, nao ha ligacao direta entre os locais indicados\n");
                        return;
                    }
                        
                }
      
        
        
        //verificar que sala origem tem pessoas
        aux = lista;
        
        while (aux != NULL)
        {
            if (aux->localRandom->id == idOrigem)                                                                           //percorrer lista ligada, sempre que encontramos uma pessoa que tem a sua sala igual à sala origem, sabemos que essa sala tem mais um local
            {
                contaPessoasOrig++;
            }
            aux = aux->prox;
        }
       
        if (contaPessoasOrig == 0)                                                                                                          //se o contador das pessoas que a sala tem continuar a 0, quer dizer que não há nenhuma pessoa nessa sala
        {
            printf ("\nErro, o local origem nao tem nenhuma pessoa para ser transferida!\n");
            return;
        }
        else
        {
            flagLocalPessoas = 1;
        }
            
        
    
    }while (flagIDpos == 0 || flagIDori == 0 || flagIDdest  == 0 || flagLiga == 0 || flagLocalPessoas == 0);         //se ainda houver uma flag a zero quer dizer que algo está errado com os ids e temos de pedir de novo

    
     for (x = 0; x<total; x++)
        {
            if (tab[x].id == idDestino)
            {
                capaMaxDest = tab[x].capacidade;
                break;
            }
        }
    
    
    
    //verificar que podemos fazer a transferência sem romper a capacidade do local destino 
    
    do
    {
         aux2 = lista;
         while (aux2 != NULL)
         {
            if (aux2->localRandom->id == idDestino)       //percorrer lista ligada, sempre que encontramos uma pessoa que tem a sua sala igual à sala origem, sabemos que essa sala tem mais um local
            {
                contaPessoasDest++;
            }
            aux2 = aux2->prox;
         }
        
        printf ("\nIndique quantas pessoas pretende transferir do local origem: ");
        scanf ("%d", &numTransf);
        
         
         if (numTransf <= contaPessoasOrig) 
          {
              flagMover2 = 1;
          }
         else 
         {
             printf ("\nErro, esta a transferir mais pessoas do que a sala tem\n");
             return;
         }
       
        vagas = capaMaxDest - contaPessoasDest;
           
                                                                                                                        //verificar que a dest tem capacidade e que existem o numero de pessoas suficientes na sala origem para serem transferidas 
        if (vagas >= numTransf)
        {
           flagMover =1; 
        }
        
        else
        {
            printf ("\nErro, nao ha vagas suficientes neste local\n");
        }
        
    
    }while (flagMover == 0 || flagMover2 == 0);
    

    
    
    //mover as pessoas
    
    contaPessMover = numTransf;
    int pessoaRandom, indice = 0;
    
    for (h = 0; h<total; h++)       //encontrar o local
    {
        if (idDestino == tab[h].id)
        {
            indice = h;
            break;
        }
    }
   
                while (contaPessMover !=0)                                                          //equanto não chegar a 0, ou seja, enquanto não fizer todas as pessoas
                {
                    pessoaRandom = intUniformRnd(1, contaPessoasOrig);    
                    aux3 = lista;           

                            while (aux3 != NULL)                                                        //só se para até se encontrar a pessoa na posição aleat. que nos foi dada 
                            {
                                if (aux3->localRandom->id == idOrigem)                  //se tiver o id origem, decrementamos o pessoaRandom e avançamos na lista
                                {           
                                    pessoaRandom-- ;
                                }
                                if (pessoaRandom == 0)
                                {
                                    aux3->localRandom = &tab[indice];                       //dou à pessoa a sala destino, na posição h que está no array
                                    contaPessMover--;
                                    flagSucesso = 1;
                                    break;
                                }
                                aux3 = aux3->prox ;  
                            }
                                                                                                                        //ao sair, já tenho o aux na pessoa a mudar-lhe a sala
 
                }
       
    
                if (flagSucesso == 1)
                {
                    printf ("\n*Transferecia de pessoas bem sucessida!*\n");
                }
   
}


//menu
char menu()
{
    char n;
    printf ("\n          ***---MENU---***            \n");
    printf("\nOpcao 1 - Avancar uma itereacao na simulacao");
    printf("\nOpcao 2 - Recuar ate 3 iteracoes na simulacao");
    printf("\nOpcao 3 - Apresentar estatisticas da simulacao");
    printf("\nOpcao 4 - Adicionar doente");
    printf("\nOpcao 5 - Transferir pessoas entre dois locais");
    printf("\nOpcao 6 - Mostrar informacao do espaco");
    printf("\nOpcao 7 - Mostrar informacao da populacao");
    printf("\nOpcao 8 - Terminar Simulacao\n");
    printf ("\nEscolha a sua opcao: ");
    fflush(stdin);
    n = getchar();

    return n;
}

//escrever no relatório final as taxas
void taxas_relatoFinal(pPessoa lista, double *taxaI, double *taxaS, double *taxaD, double *taxaR, int *contaIter)
{
    int contaTpessoas = 0, contaI = 0, contaD = 0, contaS = 0, contaR = 0;
    pPessoa aux;
    FILE *f;
    
    *taxaI = *taxaS =  *taxaD = *taxaR = 0;
    
    //calculos das taxas
    aux = lista;
    while (aux != NULL)
    {
        contaTpessoas++;
        if(aux->estado == 'D')
        {
            contaD++;
        }
        else if (aux->estado == 'I')
        {
            contaI++;
        }
        else if (aux->estado == 'S')
        {
            contaS++;
        }
        else if (aux->estado == 'R')
        {
            contaR++;
        }
       
        aux = aux->prox;
    }
    
    *taxaD = contaD / (double) contaTpessoas;
    *taxaI = contaI / (double) contaTpessoas;
    *taxaS = contaS / (double) contaTpessoas;
    *taxaR = contaR / (double) contaTpessoas;
    
    
    //criar ficheiro do relatório
    
    f = fopen ("report.txt", "w");          
    
    if (f == NULL)
    {
        printf ("\nErro ao criar o ficheiro: report.txt!\n");
        fclose (f);
    }
    
    //escrever conteudos no ficheiro
    else 
    {
        fprintf (f, "\n      *****Relatorio final da simulacao*****");
        fprintf (f, "\n---------------------------------------------------------\n");
        fprintf (f, "\n");
        fprintf (f, "\n---A simulacao foi realizada num total de %d dias---\n", *contaIter);
        fprintf (f, "\n**---Taxas finais---**\n");
        fprintf (f, "\n-Num total de %d pessoas-\n", contaTpessoas);
     
        if (*taxaI > 0 )
        {
           fprintf (f, "\nA taxa de pessoas imunes: %.2f", *taxaI);
           fprintf (f, "\nHa %d imunes", contaI);
        }

        if (*taxaD > 0 )
        {
           fprintf (f, "\nA taxa de pessoas doentes: %.2f", *taxaD);
           fprintf (f, "\nHa %d doentes", contaD);
        }

        if (*taxaS > 0)
        {
           fprintf (f, "\nA taxa de pessoas saudaveis: %.2f", *taxaS);
           fprintf (f, "\nHa %d saudaveis", contaS);
        }

        if (*taxaR > 0)
        {
           fprintf (f, "\nA taxa de pessoas recuperados: %.2f", *taxaR);
           fprintf (f, "\nHa %d recuperados", contaR);
        }
        fprintf (f, "\n");
        
        fclose (f);
    }
    
}    


//escrever no relatório final as idades

void idades_relatoFinal (pPessoa lista)
{
    int idadeVelhoD = 0, idadeNovoD = 0, idadeVelhoI = 0, idadeNovoI = 0, idadeNovoS = 0, idadeVelhoS = 0, tempoDoente = 0, idadeMaisDoente = 0;
    int somaDoentes = 0, somaRecu = 0,somaImune = 0, somaSau = 0, numSau = 0, numDoentes = 0, numRecu = 0, numImune = 0;
    float mediaD = 0, mediaR = 0, mediaI = 0, mediaS = 0;
    FILE *f;
    
    //calculos
    while (lista!=NULL)
    {
        if (lista->estado == 'D')
        {
            idadeNovoD = lista->idade;
            somaDoentes = somaDoentes + lista->idade;
            numDoentes ++;
            
            if (tempoDoente < lista->diasDoente)
            {
                idadeMaisDoente = lista->idade;
                tempoDoente = lista->diasDoente;
            }
            
            if (idadeVelhoD < lista->idade)
            {
                idadeVelhoD = lista->idade;
            }
            
            if (idadeNovoD > lista->idade)
            {
                idadeNovoD = lista->idade;
            }
        }
        
        if (lista->estado == 'S')
        {
            idadeNovoS = lista->idade;
            somaSau = somaSau + lista->idade;
            numSau++;
            
            if (idadeVelhoS < lista->idade)
            {
                idadeVelhoS = lista->idade;
            }
            
            if (idadeNovoS > lista->idade)
            {
                idadeNovoS = lista->idade;
            }
        }
        
        if (lista->estado == 'R')
        {
            somaRecu = somaRecu + lista->idade;
            numRecu++;
        }
        
        if (lista->estado == 'I')
        {
            idadeNovoI = lista->idade;
            somaImune = somaImune + lista->idade;
            numImune++;
            
            if (idadeVelhoI < lista->idade)
            {
                idadeVelhoI = lista->idade;
            }
            
            if (idadeNovoI > lista->idade)
            {
                idadeNovoI = lista->idade;
            }
        }
    
        lista = lista->prox;
    }
    
    mediaD = somaDoentes / (float) numDoentes;
    mediaR = somaRecu / (float) numRecu;
    mediaI = somaImune / (float) numImune;
    mediaS = somaSau / (float) numSau;
    
    
    //escrever ficheiro
    f = fopen ("report.txt", "a");
    
    if (f == NULL)
    {
        printf ("\nErro ao escrever o ficheiro: report.txt!\n");
        fclose (f);
    }
    else
    {
        fprintf (f, "\n");
        fprintf (f, "\n**---Estatistica das idades da populacao---**\n");
        if (idadeVelhoD>0)
        {
           fprintf (f, "\nA idade da pessoa mais velha doente e de: %d", idadeVelhoD);
        }
        else 
            fprintf (f, "\nNao existe pessoa mais velha doente,");

        if (idadeNovoD > 0)
        {
           fprintf (f, "\nA idade da pessoa mais nova doente e de: %d\n", idadeNovoD);
        }
        else
            fprintf (f, "\nNao existe pessoa mais nova doente.\n");

          if (idadeVelhoS>0)
        {
           fprintf (f, "\nA idade da pessoa mais velha saudavel e de: %d", idadeVelhoS);
        }
        else 
            fprintf (f, "\nNao existe pessoa mais velha saudavel.");

        if (idadeNovoS > 0)
        {
           fprintf (f, "\nA idade da pessoa mais nova saudavel e de: %d\n", idadeNovoS);
        }
        else
            fprintf (f, "\nNao existe pessoa mais nova saudavel.\n");

        if (idadeVelhoI > 0)
        {
           fprintf (f, "\nA idade da pessoa mais velha imune e de: %d", idadeVelhoI);
        }
        else
            fprintf (f, "\nNao existe pessoas mais velha imune.");
        if (idadeNovoI > 0)
        {
           fprintf (f, "\nA idade da pessoa mais nova imune e de: %d\n", idadeNovoI);
        }
        else
            fprintf (f, "\nNao existe pessoa mais nova imune.\n");

        if (mediaD > 0)
        {
           fprintf (f, "\nA media das idades das pessoas doentes e de: %.2f", mediaD);
        }

        if (mediaS > 0)
        {
           fprintf (f, "\nA media das idades das pessoas saudaveis e de: %.2f", mediaS);
        }

        if (mediaI > 0)
        {
            fprintf (f, "\nA media das idades das pessoas imunes e de: %.2f", mediaI);
        }

        if (mediaR > 0)
        {
           fprintf (f, "\nA media das idades das pessoas recuperadas e de: %.2f \n", mediaR);
        }

        if (idadeMaisDoente > 0 && tempoDoente > 0)
        {
           fprintf (f, "\nA pessoa que esta ha mais tempo doente tem %d anos e esta doente ha %d dias\n", idadeMaisDoente, tempoDoente);
        }
        
        fclose (f);
    }
    
}

void pess_espac_reltoFinal (local *tab, pPessoa lista, int total)
{
    int i, j;
    FILE *f;
    
    f = fopen ("report.txt", "a");      //"a" porque é aberto para anexar ficheiros no fim dese ficheiro
    
    if (f == NULL)
    {
        printf("Erro a abrir o ficheiro: report.txt!\n");
        return;
    }
    
    else
    {
        fprintf (f, "\n");
        fprintf (f, "\n**---Caracteristicas da populacao mais recente---**\n");
        
        while (lista != NULL)
        {
            fprintf(f, "\nNome: %s\n Idade: %d\n Estado: %c\n Esta colocada no local com ID %d \n", lista->identificador, lista->idade,lista->estado, lista->localRandom->id);

            if (lista->estado == 'D')
            {
                fprintf(f, "Esta doente ha %d dias\n", lista->diasDoente);
            }
        lista = lista->prox; 
        }
        
        
        fprintf (f, "\n");
        fprintf (f, "\n**---Caracteristicas do espaco mais recente---**\n");
        
        for (i = 0; i < total; i++) 
        {
            fprintf(f, "\n----ID do local: %d\n", tab[i].id);
            fprintf(f, "\nCapacidade do local: %d\n", tab[i].capacidade);

            for (j = 0; j < 3; j++) 
            {
                fprintf(f, "\nVai ligar a: %d\n", tab[i].liga[j]);
            }
        }
        fprintf (f, "\n");
        fclose (f);
    }
}


void distri_pessRelFinal (local *tab, pPessoa lista, int total)
{
    int i, contaPessoas=0;
    pPessoa aux;
    FILE * f;
    
    f = fopen ("report.txt", "a");
    
    if (f == NULL)
    {
        printf("Erro a abrir o ficheiro: report.txt!\n");
        return;
    }
    
    else
    {
        fprintf (f, "\n**--Distribuicao das pessoa por sala--**\n");

        for (i=0; i<total; i++)
        {
            aux = lista;        //colocar o aux sempre no inicio
            contaPessoas =0;        //colocar de novo o contador a 0 para conseguir fazer a contagem da proxima sala corretamente
            while (aux != NULL)         //percorrer toda a lista para ver se há alguma pessoa que tem o id do primeiro local e fazer o mesmo para os seguintes locais
            {
                if (tab[i].id == aux->localRandom->id)      //se essa pessoa estiver na sala, então incrementar o nº de pessoas dessa sala e apresentar antes do fim do for para podermos apresentar para todas
                {
                    contaPessoas ++;
                }
                aux = aux->prox;        //avançar na lista para ver todas as pessoas
            }
           fprintf (f,"\n--Local com o ID %d--\n", tab[i].id);
           fprintf (f,"\nEsta sala tem %d pessoas ", contaPessoas);
           fprintf (f,"\n");

        }
    }   
}

void criarFicheiro_ultimaPop (pPessoa lista)
{
    char fichUtilizador[30];
    FILE *f;
    
    printf ("\nIndique o nome do ficheiro que servira para a informacao da ultima populacao: ");
    scanf ("%s", fichUtilizador);
    
    f = fopen (fichUtilizador, "w");
    
     if (f == NULL)
     {
        printf("Erro ao abrir o ficheiro: %s\n", fichUtilizador);
        fclose(f);
        return;
     }
     else
     {
        fprintf (f, "\n     ****Populacao existente na ultima iteracao****");
        fprintf (f, "\n---------------------------------------------------------------\n");
         
         while (lista != NULL)
        {
            fprintf(f, "\nNome: %s\n Idade: %d\n Estado: %c\n Esta colocada no local com ID %d \n", lista->identificador, lista->idade,lista->estado, lista->localRandom->id);

            if (lista->estado == 'D')
            {
                fprintf(f, " Esta doente ha %d dias\n", lista->diasDoente);
            }
        lista = lista->prox; 
        }
         
         fclose (f);
     }
}

void termina_simulacao (local *tab, pPessoa lista, int contaIter, int total)
{
    double taxaS, taxaD, taxaI, taxaR;
    
    taxas_relatoFinal (lista, &taxaI, &taxaS, &taxaD, &taxaR, &contaIter);
    idades_relatoFinal (lista);
    pess_espac_reltoFinal (tab, lista, total);
    distri_pessRelFinal (tab, lista, total);
    criarFicheiro_ultimaPop (lista);
    
}