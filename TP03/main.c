#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

int main()
{
     Expressao exp;
     char entrada[512];

     printf("=====================================\n");
     printf("     AVALIADOR DE EXPRESSOES\n");
     printf("=====================================\n\n");

     while (1)
     {
          printf("\nDigite a expressao em POS-FIXA (ou 'sair'):\n> ");
          fgets(entrada, sizeof(entrada), stdin);

          entrada[strcspn(entrada, "\n")] = 0;

          if (strcmp(entrada, "sair") == 0)
          {
               printf("\nFinalizando...\n");
               break;
          }

          if (strlen(entrada) == 0)
          {
               printf("Expressao vazia! Tente novamente.\n");
               continue;
          }

          strcpy(exp.posFixa, entrada);

          char *forma = getFormaInFixa(exp.posFixa);

          if (!forma)
          {
               printf("Erro: expressao pos-fixa invalida!\n");
               continue;
          }

          strcpy(exp.inFixa, forma);

          exp.Valor = getValorPosFixa(exp.posFixa);

          printf("\n-------------------------------------\n");
          printf("Forma Infixa: %s\n", exp.inFixa);
          printf("Valor: %.3f\n", exp.Valor);
          printf("-------------------------------------\n");

          free(forma);
     }

     return 0;
}