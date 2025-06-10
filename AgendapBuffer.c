#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_NOME sizeof(char) * 255
#define TAM_IDADE sizeof(char) * 5
#define TAM_EMAIL sizeof(char) * 255
#define TAM_PESSOA (TAM_NOME + TAM_IDADE + TAM_EMAIL)
#define FIM (MENU + TAM_PESSOA)
#define MENU sizeof(int) + sizeof(long long int)

void ReallocAdiciona(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void **temp);

void MenuSemOrdenar(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp);
void MenuNome(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp);
void MenuIdade(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp);
void MenuEmail(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp);

void Adiciona(char **nome, char **idade, char **email, void **fim, void *temp, long long int **qtdAlocada);
bool Busca(void *pBuffer, char *nome, void **temp, void *fim);
void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, void *temp, void **fim);
void Lista(void *pBuffer, void **temp, void *fim);
void ProxCampo(void **temp);
void ProxPessoa(void **temp);
void OrdenaNome(void *pBuffer, long long int **qtdAlocada, char *nome, void **temp, void *fim);
void OrdenaIdade(void *pBuffer, long long int **qtdAlocada, char *idade, void **temp, void *fim);
void OrdenaEmail(void *pBuffer, long long int **qtdAlocada, char *email, void **temp, void *fim);

int main()
{
comeco:
    void *pBuffer;
    pBuffer = malloc(MENU + TAM_PESSOA); // menu + variaveis iniciais
    if (pBuffer == NULL)
    {
        printf("Erro ao alocar memoria");
        exit(0);
    }

    int *menu = NULL;
    char *nome = NULL, *email = NULL, *idade = NULL;
    void *fim = NULL, *temp = NULL;
    long long int *qtdAlocada = NULL;

    menu = (int *)pBuffer;
    *menu = 0;
    qtdAlocada = sizeof(int) + (long long int *)pBuffer;
    nome = (pBuffer + MENU);
    idade = (pBuffer + MENU + TAM_NOME);
    email = (pBuffer + MENU + TAM_NOME + TAM_IDADE);
    fim = (pBuffer + FIM);

    printf("\nQual agenda voce deseja escolher?\n\n1.Sem ordenacao\n2.Ordenado por nome\n3.Ordenado por idade\n4.Ordenado por e-mail\n5.Sair\n  Escolha uma opcao: ");
    scanf("%d", menu);
    getchar();

    while (*menu != 5)
        switch (*menu)
        {
        case 1:
            MenuSemOrdenar(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp);
            break;
        case 2:
            MenuNome(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp);
            break;
        case 3:
            MenuIdade(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp);
            break;
        case 4:
            MenuEmail(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp);
            break;
        case 5:
            break;
        case 6:
            free(pBuffer);
            pBuffer = NULL;
            nome = NULL;
            idade = NULL;
            email = NULL;
            menu = NULL;
            qtdAlocada = NULL;
            temp = NULL;
            fim = NULL;
            goto comeco;
        default:
            printf("Por favor, digite um numero valido.");
            break;
        }

    free(pBuffer);
    pBuffer = NULL;
    nome = NULL;
    idade = NULL;
    email = NULL;
    menu = NULL;
    qtdAlocada = NULL;
    temp = NULL;
    fim = NULL;
    return 0;
}

void MenuSemOrdenar(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp)
{

    while (**menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n6. Escolher outra agenda\n  \nEscolha uma opcao: ");
        scanf("%d", *menu);
        getchar();

        switch (**menu)
        {
        case 1:
            printf("\nEscreva o nome: ");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            printf("Escreva a idade: ");
            fgets(*idade, TAM_IDADE, stdin);
            (*idade)[strcspn(*idade, "\n")] = '\0';

            printf("Escreva o email: ");
            fgets(*email, TAM_EMAIL, stdin);
            (*email)[strcspn(*email, "\n")] = '\0';

            temp = *fim;
            ReallocAdiciona(pBuffer, menu, qtdAlocada, nome, idade, email, fim, &temp);
            Adiciona(nome, idade, email, fim, temp, qtdAlocada);
            break;

        case 2:
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';
            Remove(pBuffer, nome, idade, email, menu, temp, fim);
            break;

        case 3:
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            if (Busca(*pBuffer, *nome, &temp, *fim))
            {
                printf("\n--------------------------");
                printf("\n\tNome: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tIdade: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tE-mail: %s\n", (char *)temp);
                printf("--------------------------\n");
            }
            break;

        case 4:
            Lista(*pBuffer, &temp, *fim);
            break;

        case 5:
            return;

        case 6:
            return;

        default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }
}

void ReallocAdiciona(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void **temp)
{
    void *tempTemp = *temp;
    void *tempAdiciona = *pBuffer;
    **qtdAlocada = (sizeof(char) * strlen(*nome) + 1 + sizeof(char) * strlen(*idade) + 1 + sizeof(char) * strlen(*email) + 1);
    *pBuffer = realloc(*pBuffer, (((long long int)((char *)*fim - (char *)*pBuffer)) +
                                  (sizeof(char) * strlen(*nome) + 1 + sizeof(char) * strlen(*idade) + 1 + sizeof(char) * strlen(*email) + 1)));
    if (*pBuffer == NULL)
    {
        printf("Nao foi possivel realocar memoria");
        exit(0);
    }

    if (tempAdiciona != *pBuffer)
    {
        *menu = (int *)*pBuffer;
        *qtdAlocada = sizeof(int) + (long long int *)*pBuffer;
        *nome = (*pBuffer + MENU);
        *idade = (*pBuffer + MENU + TAM_NOME);
        *email = (*pBuffer + MENU + TAM_NOME + TAM_IDADE);
        *fim = (char *)(*pBuffer) + (long long int)(((char *)*fim - (char *)tempAdiciona));
        *temp = (char *)(*pBuffer) + (long long int)(((char *)tempTemp) - (char *)tempAdiciona);
    }
    tempTemp = NULL;
    tempAdiciona = NULL;
}

void Adiciona(char **nome, char **idade, char **email, void **fim, void *temp, long long int **qtdAlocada)
{
    strcpy((char *)temp, *nome);
    temp = (char *)temp + strlen(*nome) + 1;
    strcpy((char *)temp, *idade);
    temp = (char *)temp + strlen(*idade) + 1;
    strcpy((char *)temp, *email);
    temp = (char *)temp + strlen(*email) + 1;
    *fim = (char *)*fim + **qtdAlocada;
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
        printf("\n\tE-mail: %s\n", (char *)*temp);
        ProxCampo(temp);
        printf("--------------------------");
    }
    printf("\n");
}

void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, void *temp, void **fim)
{
    void *tempRemove = NULL;
    void *tempRealloc = *pBuffer;
    if (Busca(*pBuffer, *nome, &tempRemove, *fim))
    {
        temp = tempRemove;
        ProxPessoa(&temp);

        memmove(tempRemove, temp, (long long int)((char *)*fim - (char *)temp));

        *pBuffer = realloc(*pBuffer, (long long int)((char *)((char *)*fim - (char *)*pBuffer) - (char *)(((char *)temp - (char *)tempRemove))));
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
            *fim = (char *)*pBuffer + (long long int)((char *)*fim - (char *)tempRealloc) - (long long int)((char *)temp - (char *)tempRemove);
        }
        else
        {
            *fim = (char *)*fim - (long long int)((char *)temp - (char *)tempRemove);
        }

        printf("\n  Nome Removido!\n");
    }
    tempRemove = NULL;
    tempRealloc = NULL;
}

void ProxPessoa(void **temp)
{
    ProxCampo(temp);
    ProxCampo(temp);
    ProxCampo(temp);
}

void OrdenaNome(void *pBuffer, long long int **qtdAlocada, char *nome, void **temp, void *fim)
{
    *temp = pBuffer + FIM;
    if (*temp == fim)
        return;
    while (strcmp(nome, *temp) > 0 && *temp != fim)
        ProxPessoa(temp);
    if (*temp == fim)
        return;
    else
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
}

void OrdenaIdade(void *pBuffer, long long int **qtdAlocada, char *idade, void **temp, void *fim)
{
    *temp = pBuffer + FIM;
    if (*temp == fim)
        return;
    char *tempIdade = pBuffer + FIM;
    ProxCampo((void *)&tempIdade);

    while (atoi(tempIdade) <= atoi(idade) && *temp != fim)
    {
        ProxPessoa(temp);
        ProxPessoa((void *)(&tempIdade));
    }
    if (*temp == fim)
        return;
    else
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
    tempIdade = NULL;
}

void OrdenaEmail(void *pBuffer, long long int **qtdAlocada, char *email, void **temp, void *fim)
{
    *temp = pBuffer + FIM;
    if (*temp == fim)
        return;
    char *tempEmail = pBuffer + FIM;
    ProxCampo((void *)&tempEmail);
    ProxCampo((void *)&tempEmail);

    while (strcmp(email, *temp) > 0 && *temp != fim)
    {
        ProxPessoa(temp);
        ProxPessoa((void *)(&tempEmail));
    }
    if (*temp == fim)
        return;
    else
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
    tempEmail = NULL;
}

void MenuNome(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp)
{

    while (**menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n6. Escolher outra agenda\n  \nEscolha uma opcao: ");
        scanf("%d", *menu);
        getchar();

        switch (**menu)
        {
        case 1:
            printf("\nEscreva o nome: ");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            printf("Escreva a idade: ");
            fgets(*idade, TAM_IDADE, stdin);
            (*idade)[strcspn(*idade, "\n")] = '\0';

            printf("Escreva o email: ");
            fgets(*email, TAM_EMAIL, stdin);
            (*email)[strcspn(*email, "\n")] = '\0';

            ReallocAdiciona(pBuffer, menu, qtdAlocada, nome, idade, email, fim, &temp);
            OrdenaNome(*pBuffer, qtdAlocada, *nome, &temp, *fim);
            Adiciona(nome, idade, email, fim, temp, qtdAlocada);
            break;

        case 2:
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';
            Remove(pBuffer, nome, idade, email, menu, temp, fim);
            break;

        case 3:
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            if (Busca(*pBuffer, *nome, &temp, *fim))
            {
                printf("\n--------------------------");
                printf("\n\tNome: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tIdade: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tE-mail: %s\n", (char *)temp);
                printf("--------------------------\n");
            }
            break;

        case 4:
            Lista(*pBuffer, &temp, *fim);
            break;

        case 5:
            return;

        case 6:
            return;

        default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }
}

void MenuIdade(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp)
{
    while (**menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n6. Escolher outra agenda\n  \nEscolha uma opcao: ");
        scanf("%d", *menu);
        getchar();

        switch (**menu)
        {
        case 1:
            printf("\nEscreva o nome: ");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            printf("Escreva a idade: ");
            fgets(*idade, TAM_IDADE, stdin);
            (*idade)[strcspn(*idade, "\n")] = '\0';

            printf("Escreva o email: ");
            fgets(*email, TAM_EMAIL, stdin);
            (*email)[strcspn(*email, "\n")] = '\0';

            ReallocAdiciona(pBuffer, menu, qtdAlocada, nome, idade, email, fim, &temp);
            OrdenaIdade(*pBuffer, qtdAlocada, *idade, &temp, *fim);
            Adiciona(nome, idade, email, fim, temp, qtdAlocada);
            break;

        case 2:
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';
            Remove(pBuffer, nome, idade, email, menu, temp, fim);
            break;

        case 3:
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            if (Busca(*pBuffer, *nome, &temp, *fim))
            {
                printf("\n--------------------------");
                printf("\n\tNome: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tIdade: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tE-mail: %s\n", (char *)temp);
                printf("--------------------------\n");
            }
            break;

        case 4:
            Lista(*pBuffer, &temp, *fim);
            break;

        case 5:
            return;

        case 6:
            return;

        default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }
}

void MenuEmail(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp)
{
    while (**menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n6. Escolher outra agenda\n  \nEscolha uma opcao: ");
        scanf("%d", *menu);
        getchar();

        switch (**menu)
        {
        case 1:
            printf("\nEscreva o nome: ");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            printf("Escreva a idade: ");
            fgets(*idade, TAM_IDADE, stdin);
            (*idade)[strcspn(*idade, "\n")] = '\0';

            printf("Escreva o email: ");
            fgets(*email, TAM_EMAIL, stdin);
            (*email)[strcspn(*email, "\n")] = '\0';

            ReallocAdiciona(pBuffer, menu, qtdAlocada, nome, idade, email, fim, &temp);
            OrdenaEmail(*pBuffer, qtdAlocada, *email, &temp, *fim);
            Adiciona(nome, idade, email, fim, temp, qtdAlocada);
            break;

        case 2:
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';
            Remove(pBuffer, nome, idade, email, menu, temp, fim);
            break;

        case 3:
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            if (Busca(*pBuffer, *nome, &temp, *fim))
            {
                printf("\n--------------------------");
                printf("\n\tNome: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tIdade: %s", (char *)temp);
                ProxCampo(&temp);
                printf("\n\tE-mail: %s\n", (char *)temp);
                printf("--------------------------\n");
            }
            break;

        case 4:
            Lista(*pBuffer, &temp, *fim);
            break;

        case 5:
            return;

        case 6:
            return;

        default:
            printf("Por favor, digite um numero valido.");
            break;
        }
    }
}