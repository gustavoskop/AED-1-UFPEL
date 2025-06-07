#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_NOME sizeof(char) * 255
#define TAM_IDADE sizeof(char) * 5
#define TAM_EMAIL sizeof(char) * 255
#define TAM_PESSOA (TAM_NOME + TAM_IDADE + TAM_EMAIL)
#define FIM (MENU + TAM_PESSOA)
#define MENU sizeof(int)

void Adiciona(void **pBuffer, int **menu, char **nome, char **idade, char **email, void **fim, void *temp);
bool Busca(void *pBuffer, char *nome, void **temp, void *fim);
void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, void **temp, void **fim);
void Lista(void *pBuffer, void **temp, void *fim);
void ProxCampo(void **temp);

int main()
{
    void *pBuffer;
    pBuffer = malloc(sizeof(int) + TAM_PESSOA); // menu + variaveis iniciais
    if (pBuffer == NULL)
    {
        printf("Erro ao alocar memoria");
        exit(0);
    }

    int *menu = NULL;
    char *nome = NULL, *email = NULL, *idade = NULL;
    void *fim = NULL, *temp = NULL;

    menu = (int *)pBuffer;
    *menu = 0;

    nome = (pBuffer + MENU);
    idade = (pBuffer + MENU + TAM_NOME);
    email = (pBuffer + MENU + TAM_NOME + TAM_IDADE);
    fim = (pBuffer + FIM);

    while (*menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n  \nEscolha uma opcao: ");
        scanf("%d", menu);
        getchar();

        switch (*menu)
        {
        case 1:
            printf("\nEscreva o nome: ");
            fgets(nome, TAM_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Escreva a idade: ");
            fgets(idade, TAM_IDADE, stdin);
            idade[strcspn(idade, "\n")] = '\0';

            printf("Escreva o email: ");
            fgets(email, TAM_EMAIL, stdin);
            email[strcspn(email, "\n")] = '\0';

            Adiciona(&pBuffer, &menu, &nome, &idade, &email, &fim, temp);
            break;

        case 2:
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(nome, TAM_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            Remove(&pBuffer, &nome, &idade, &email, &menu, &temp, &fim);
            break;

        case 3:
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(nome, TAM_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0';

            if (Busca(pBuffer, nome, &temp, fim))
            {
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
    nome = NULL;
    idade = NULL;
    email = NULL;
    menu = NULL;
    temp = NULL;
    fim = NULL;
    return 0;
}

void Adiciona(void **pBuffer, int **menu, char **nome, char **idade, char **email, void **fim, void *temp)
{

    temp = *pBuffer;
    *pBuffer = realloc(*pBuffer, (((long long int)((char *)*fim - (char *)*pBuffer)) +
                                  (sizeof(char) * strlen(*nome) + 1 + sizeof(char) * strlen(*idade) + 1 + sizeof(char) * strlen(*email) + 1)));
    if (*pBuffer == NULL)
    {
        printf("Nao foi possivel realocar memoria");
        exit(0);
    }

    if (temp != *pBuffer)
    {
        *menu = (int *)*pBuffer;
        *nome = (*pBuffer + MENU);
        *idade = (*pBuffer + MENU + TAM_NOME);
        *email = (*pBuffer + MENU + TAM_NOME + TAM_IDADE);
        *fim = (char *)(*pBuffer) + (long long int)(((char *)*fim - (char *)temp));
    }

    strcpy((char *)*fim, *nome);
    *fim = (char *)*fim + strlen(*nome) + 1;
    strcpy((char *)*fim, *idade);
    *fim = (char *)*fim + strlen(*idade) + 1;
    strcpy((char *)*fim, *email);
    *fim = (char *)*fim + strlen(*email) + 1;
}

void ProxCampo(void **temp)
{
    *temp = (char *)*temp + strlen((char *)*temp) + 1;
}

bool Busca(void *pBuffer, char *nome, void **temp, void *fim)
{
    *temp = pBuffer + FIM;

    if (*temp == NULL)
    {
        printf("\n  Agenda vazia!\n");
        return false;
    }
    else
    {
        while (*temp != fim)
        {
            if (strcmp(nome, (char *)*temp) == 0)
                return true;
            else
            {
                ProxCampo(temp);
                ProxCampo(temp);
                ProxCampo(temp);
            }
        }
        printf("\n  Nome nao encontrado!\n");
        return false;
    }
}

void Lista(void *pBuffer, void **temp, void *fim)
{
    *temp = pBuffer + FIM;

    if (fim == pBuffer + FIM)
    {
        printf("\n  Agenda vazia!\n");
        return;
    }
    printf("\n--------------------------");
    while (*temp != fim)
    {
        printf("\n\tNome: %s", (char *)*temp);
        ProxCampo(temp);
        printf("\n\tIdade: %s", (char *)*temp);
        ProxCampo(temp);
        printf("\n\tEmail: %s\n", (char *)*temp);
        ProxCampo(temp);
        printf("--------------------------");
    }
    printf("\n");
}

void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, void **temp, void **fim)
{
    void *tempRemove = NULL;
    void *tempRealloc = *pBuffer;
    if (Busca(*pBuffer, *nome, &tempRemove, *fim))
    {

        *temp = tempRemove;
        ProxCampo(temp);
        ProxCampo(temp);
        ProxCampo(temp);

        memmove(tempRemove, *temp, (long long int)((char *)*fim - (char *)*temp));

        *pBuffer = realloc(*pBuffer, (long long int)((char *)((char *)*fim - (char *)*pBuffer) - (char *)(((char *)*temp - (char *)tempRemove))));
        if (*pBuffer == NULL)
        {
            printf("\nNao foi possivel realocar memoria\n");
            exit(0);
        }

        if (tempRealloc != *pBuffer)
        {
            *menu = (int *)(*pBuffer);
            *nome = (*pBuffer + MENU);
            *idade = (*pBuffer + MENU + TAM_NOME);
            *email = (*pBuffer + MENU + TAM_NOME + TAM_IDADE);
            *fim = (char *)*pBuffer + (long long int)((char *)*fim - (char *)tempRealloc) - (long long int)((char *)*temp - (char *)tempRemove);
        }
        else
        {
            *fim = (char *)*fim - (long long int)((char *)*temp - (char *)tempRemove);
        }

        printf("\n  Nome Removido!\n");
    }
}