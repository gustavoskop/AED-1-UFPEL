#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define TAM_NOME sizeof(char) * 100
#define TAM_IDADE sizeof(char) * 5 
#define TAM_EMAIL sizeof(char) * 100
#define TAM_PESSOA (TAM_NOME + TAM_IDADE + TAM_EMAIL)
#define FIM (MENU + TAM_PESSOA)
#define MENU sizeof(int) 

void Adiciona(void** pBuffer, int* menu, char* nome, char* idade, char* email, void **fim);
bool Busca(void* pBuffer, char* nome, void** temp, void *fim);
void Remove();
void Lista();
void ProxCampo(void** temp);


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
    fim  = (pBuffer + FIM);

       printf("%lld\n", (long long int)((char *)fim - (char *)pBuffer));

    while (*menu != 5){
        printf("\n\tEscolha uma opcao: \n \
        1. Adicionar uma pessoa \n \
        2. Remover uma pessoa \n \
        3. Buscar um nome \n \
        4. Listar\n \
        5. Sair\n");
        scanf("%d", menu);
        getchar();

        switch (*menu){
            case 1:
            Adiciona(&pBuffer, menu, nome, idade, email, &fim);
            break;

            case 2:
           // Remove();
            break;

            case 3:
            printf("\nDigite o nome da pessoa: ");
            fgets(nome, TAM_NOME, stdin);
            nome [strcspn(nome, "\n")] = '\0';
            if (Busca(pBuffer, nome, &temp, fim)){
                printf("\n\tNome: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tIdade: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tEmail: %s", (char *)temp);
            }
            break;

            case 4:
            Lista(pBuffer, &temp, fim);
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

void Adiciona(void** pBuffer, int* menu, char* nome, char* idade, char* email, void **fim){

    printf("\n\tEscreva o nome: ");
    fgets(nome, TAM_NOME, stdin);
    nome [strcspn(nome, "\n")] = '\0';

    printf("\tEscreva a idade: ");
    fgets(idade, TAM_IDADE, stdin);
    idade [strcspn(idade, "\n")] = '\0';

    printf("\tEscreva o email: ");
    fgets(email, TAM_EMAIL, stdin);
    email [strcspn(email, "\n")] = '\0';



    *pBuffer = realloc(*pBuffer, (((long long int)((char *)*fim - (char *)*pBuffer)) + \
                                        (sizeof(char) * strlen(nome) + 1 + sizeof(char) * strlen(idade) + 1 + sizeof(char) * strlen(email) + 1)));
    if(*pBuffer == NULL)
        printf("nao foi possivel realocar memoria");

    menu = (int *)*pBuffer;
    nome = (*pBuffer + MENU);
    idade = (*pBuffer + MENU + TAM_NOME);
    email = (*pBuffer + MENU + TAM_NOME + TAM_IDADE);

    *fim = (char *)(*pBuffer) + ((char *)*fim - (char *)*pBuffer);


    strcpy((char *)*fim, nome);
    *fim = (char *)*fim + strlen(nome) + 1;
    strcpy((char *)*fim, idade);
    *fim = (char *)*fim + strlen(idade) + 1;
    strcpy((char *)*fim, email);
    *fim = (char *)*fim + strlen(email) + 1;
    
    printf("%lld\n", (long long int)((char *)*fim - (char*) *pBuffer));

}

void ProxCampo(void** temp){
    *temp = (char *)*temp + strlen((char *)*temp) + 1;
    }



bool Busca(void* pBuffer, char* nome, void** temp, void *fim){
    *temp = pBuffer + FIM;

    if (*temp == NULL){
        printf("\nAgenda vazia!");
        return false;   
    }
    else {
        while(*temp != fim){
            if (strcmp(nome, (char *)*temp) == 0)
                return true;
            else {
                ProxCampo(temp);
                ProxCampo(temp);
                ProxCampo(temp);
        }
        }
        printf("\nNome nao encontrado!");
        return false;
    }
    
}

void Lista(void* pBuffer, void** temp, void *fim){
    *temp = pBuffer + FIM;

        printf("\n--------------------------");
    while (*temp != fim){
        printf("\n\tNome: %s", (char *)*temp);
        ProxCampo(temp);
        printf("\n\tIdade: %s", (char *)*temp);
        ProxCampo(temp);
        printf("\n\tEmail: %s\n", (char *)*temp);
        ProxCampo(temp);
        printf("--------------------------");
    }
}