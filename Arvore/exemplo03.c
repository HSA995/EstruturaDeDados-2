#include <stdio.h>
#include <stdlib.h>

typedef struct No {

    int Chave;

    // No caso de árvore binária (Máximo 2 filhos)
    struct No * Esquerda;// 1° filho
    struct No * Direita;// 2° filho
    

    
            

}No;

No * criarNo(int Chave) {

    No * N = (No *) malloc(sizeof(No));
    if(N == NULL) { 
        printf("ERRO: Nao ha memoria para o No\n");
        return NULL;
    }

    N-> Chave = Chave;
    N->Esquerda = NULL;
    N->Direita = NULL;
   
    return N;
}

int main(){

    //Nó de nivel 1

    No * Raiz = criarNo(10);

    // Nós de nivel 2

    Raiz -> Esquerda = criarNo(12);
    Raiz -> Direita = criarNo(15);

    // Nós de nivel 3

    Raiz ->Esquerda->Esquerda = criarNo(16);
    Raiz ->Esquerda->Direita = criarNo(18);
    Raiz->Direita->Direita = criarNo(20);

    // Nós de nivel 4

    Raiz ->Direita->Direita->Esquerda = criarNo(17);
    Raiz ->Direita->Direita->Direita = criarNo(19);
}