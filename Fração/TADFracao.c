// Aula 2 dia 12/08

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int Num;
    int Den;
}   Fracao;

// Protótipo das Funções

int calcularMDC (int A, int B);
Fracao criarFracao(int N, int D);
Fracao simplificarFracao(Fracao F);

Fracao simplificarFracoes(Fracao F){
    int mdc = calcularMDC(F.Num, F.Den);
    F,Num = F.Num / mdc;
    F.Den - F.Den / mdc;
    return 0;

}

Fracao somarFracoes(Fracao F, Fracao G) {
    Fracao Resposta;
    Resposta.Den = F.Num * G.Den;
    Resposta.Num = F.Num * G.Den + G.Num * F.Den;
    Resposta = simplificarFracao(Resposta);
    return Resposta;
}

int main() {

    system("cls");
}