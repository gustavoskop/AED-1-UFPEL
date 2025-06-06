#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Adiciona();
void Busca();
void Remove();
void Lista();


int main()
{
    void *pBuffer;
    pBuffer = malloc(sizeof(int) * 2); // menu + qtd de pessoas
    if(pBuffer == NULL)
        printf("memoria nao alocada");


    int *menu = NULL, *qtdDePessoas = NULL, *idade = NULL;
    char *nome = NULL, *email = NULL;


    menu = (int *) pBuffer;
    *menu = 0;
    qtdDePessoas = ((int *) pBuffer + 1);
    *qtdDePessoas = 0;

    while (*menu != 5){
        printf("Escolha uma opcao: \n \
        1. Adicionar uma pessoa \n \
        2. Remover uma pessoa \n \
        3. Buscar um nome \n \
        4. Listar\n \
        5. Sair\n");
        scanf("%d", menu);

        switch (*menu){
            case 1:
            Adiciona();
            break;

            case 2:
            Remove();
            break;

            case 3:
            Busca();
            break;

            case 4:
            Lista();
            break;

            case 5:
            break;

            default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }

}