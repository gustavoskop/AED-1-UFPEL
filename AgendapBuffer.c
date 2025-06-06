#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TAM_NOME sizeof(char) * 100
#define TAM_IDADE sizeof(int) 
#define TAM_EMAIL sizeof(char) * 100
#define TAM_PESSOA (TAM_NOME + TAM_IDADE + TAM_EMAIL)
#define MENU sizeof(int)

void Adiciona(void *pBuffer, char* nome, char * idade, char* email, void * fim);
void Busca();
void Remove();
void Lista();


int main()
{
    void *pBuffer;
    pBuffer = malloc(sizeof(int) + TAM_PESSOA); // menu + variaveis iniciais
    if(pBuffer == NULL)
        printf("memoria nao alocada");


    int *menu = NULL;
    char *nome = NULL, *email = NULL, *idade = NULL;
    void *fim = NULL, *temp = NULL;


    menu = (int *) pBuffer;
    *menu = 0;

 

    nome = (pBuffer + MENU);
    idade = (pBuffer + MENU + TAM_NOME);
    email = (pBuffer + MENU + TAM_NOME + TAM_IDADE);
    fim = (pBuffer + MENU + TAM_PESSOA);

       printf("%lld", fim - pBuffer);

    while (*menu != 5){
        printf("Escolha uma opcao: \n \
        1. Adicionar uma pessoa \n \
        2. Remover uma pessoa \n \
        3. Buscar um nome \n \
        4. Listar\n \
        5. Sair\n");
        scanf("%d", menu);
        getchar();

        switch (*menu){
            case 1:
            Adiciona(pBuffer, nome, idade, email, fim);
            break;

            case 2:
           // Remove();
            break;

            case 3:
           // Busca();
            break;

            case 4:
            //Lista();
            break;

            case 5:
            break;

            default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }
        free(pBuffer);
        pBuffer = NULL;

}

void Adiciona(void* pBuffer, char* nome, char* idade, char* email, void* fim){


    printf("\n\tEscreva o nome: ");
    fgets(nome, TAM_NOME, stdin);
    nome [strcspn(nome, "\n")] = '\0';

    printf("\n\tEscreva a idade: ");
    fgets(idade, TAM_IDADE, stdin);
    idade [strcspn(idade, "\n")] = '\0';

    printf("\n\tEscreva o email: ");
    fgets(email, TAM_EMAIL, stdin);
    email [strcspn(email, "\n")] = '\0';



    pBuffer = realloc(pBuffer, (((long long int)(fim - pBuffer)) + (sizeof(char) * strlen(nome) + sizeof(int) + sizeof(char) * strlen(email))));

    sscanf(nome, "%s", fim);
    fim = fim + sizeof(char) * strlen(nome);
    sscanf(idade, "%d", fim);
    fim = fim + sizeof(int);
    sscanf(email, "%s", fim);
    fim = fim + sizeof(char) * strlen(email);

    fim = fim + strlen(nome) + sizeof(int) + strlen(email);





}
