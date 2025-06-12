#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// tamanho das variáveis
#define TAM_NOME sizeof(char) * 255
#define TAM_IDADE sizeof(char) * 5
#define TAM_EMAIL sizeof(char) * 255
#define TAM_PESSOA (TAM_NOME + TAM_IDADE + TAM_EMAIL)
#define FIM (MENU + TAM_PESSOA)
#define MENU sizeof(int) + sizeof(long long int)


//funções principais
void ReallocAdiciona(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim);
void Adiciona(char **nome, char **idade, char **email, void **fim, void *temp, long long int **qtdAlocada);
bool Busca(void *pBuffer, char *nome, void **temp, void *fim);
void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, long long int **qtdAlocada, void *temp, void **fim);
void Lista(void *pBuffer, void **temp, void *fim);

//funções para pular campos
void ProxCampo(void **temp);
void ProxPessoa(void **temp);

//funções de ordenação
void OrdenaNome(void *pBuffer, long long int **qtdAlocada, char **nome, void **temp, void *fim);
void OrdenaIdade(void *pBuffer, long long int **qtdAlocada, char **idade, void **temp, void *fim);
void OrdenaEmail(void *pBuffer, long long int **qtdAlocada, char **email, void **temp, void *fim);

//menu
void Menu(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp,
          void (*ordenar)(void *, long long int **, char **, void **, void *),
          char *campoOrdenacao);

int main()
{
    void *pBuffer;
comeco:

    pBuffer = malloc(MENU + TAM_PESSOA); // menu + qtdAlocada
    if (pBuffer == NULL)
    {
        printf("Erro ao alocar memoria");
        exit(0);
    }

    //inicialização das variáveis
    int *menu = NULL;
    char *nome = NULL, *email = NULL, *idade = NULL;
    void *fim = NULL, *temp = NULL; // fim = ponteiro que sempre aponta para o final do buffer, temp = ponteiro auxiliar para fazer o que precisar
    long long int *qtdAlocada = NULL; // quantidade adicionada depois do realloc

    //onde vai ficar cada variável no buffer
    menu = (int *)pBuffer;
    *menu = 0;
    qtdAlocada = sizeof(int) + (long long int *)pBuffer;
    nome = (pBuffer + MENU);
    idade = (pBuffer + MENU + TAM_NOME);
    email = (pBuffer + MENU + TAM_NOME + TAM_IDADE);
    fim = (pBuffer + FIM);

    //menu inicial, para escolher que tipo de ordenação vai querer utilizar na agenda
    printf("\nQual agenda voce deseja escolher?\n\n1.Sem ordenacao\n2.Ordenado por nome\n3.Ordenado por idade\n4.Ordenado por e-mail\n5.Sair\n  Escolha uma opcao: ");
    scanf("%d", menu);
    getchar();

    while (*menu != 5)
        switch (*menu)
        {
        case 1:
            Menu(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp, NULL, NULL); //sem ordenação
            break;
        case 2:
            Menu(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp, OrdenaNome, nome); //em ordem alfabética pelo nome
            break;
        case 3:
            Menu(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp, OrdenaIdade, idade); //ordem crescen de idade 
            break;
        case 4:
            Menu(&pBuffer, &menu, &qtdAlocada, &nome, &idade, &email, &fim, temp, OrdenaEmail, email); //ordem alfabética pelo email
            break;
        case 5:
            break;//fim do código
        case 6:
        //caso o usuário deseje, enquanto ele estiver dentro da agenda ele pode mudar a forma de ordenação, reiniciando a agenda e voltando para o menu inicial
            free(pBuffer);
            pBuffer = NULL;
            nome = NULL;
            idade = NULL;
            email = NULL;
            menu = NULL;
            qtdAlocada = NULL;
            temp = NULL;
            fim = NULL;
            goto comeco; // volta para a segunda linha do main
        default:
            printf("Por favor, digite um numero valido."); //controle de erro
            break;
        }

        //se o usuário digitar 5, encerra a execução
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

//menu principal
void Menu(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim, void *temp, //passando o endereço das variáveis
          void (*ordenar)(void *, long long int **, char **, void **, void *),//para escolher qual o método de ordenação desejado
          char *campoOrdenacao)
{

    while (**menu != 5)
    {
        printf("\n\n1. Adicionar uma pessoa\n2. Remover uma pessoa\n3. Buscar um nome\n4. Listar\n5. Sair\n6. Escolher outra agenda\n  \nEscolha uma opcao: ");
        scanf("%d", *menu);
        getchar();

        switch (**menu)
        {
        case 1: //adicionar
            printf("\nEscreva o nome: ");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0'; //pega o nome do input

            printf("Escreva a idade: ");
            fgets(*idade, TAM_IDADE, stdin);
            (*idade)[strcspn(*idade, "\n")] = '\0'; //pega a idade do input

            printf("Escreva o email: ");
            fgets(*email, TAM_EMAIL, stdin);
            (*email)[strcspn(*email, "\n")] = '\0'; //pega o email do input

            ReallocAdiciona(pBuffer, menu, qtdAlocada, nome, idade, email, fim); //realloca com o tamanho necessário
            if (ordenar != NULL) //se for null, é porque o usuário não quer ordenação
            {
                ordenar(*pBuffer, qtdAlocada, &campoOrdenacao, &temp, *fim); //define qual o método de ordenação
            }
            else
                temp = *fim;
            Adiciona(nome, idade, email, fim, temp, qtdAlocada);
            break;

        case 2: //remover
            printf("\nDigite o nome a ser removido: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';
            Remove(pBuffer, nome, idade, email, menu, qtdAlocada, temp, fim);
            break;

        case 3: //busca
            printf("\nDigite o nome a ser encontrado: (Maiusculas importam!)\n");
            fgets(*nome, TAM_NOME, stdin);
            (*nome)[strcspn(*nome, "\n")] = '\0';

            if (Busca(*pBuffer, *nome, &temp, *fim)) //se busca retornar true, o nome foi achado e vai ser mostrado no terminal
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

        case 4: //lista
            Lista(*pBuffer, &temp, *fim);
            break;

        case 5: //encerrar o código
            return;

        case 6: //volta para o menu inicial, para selecionar outro método de ordenação
            return;

        default:
            printf("Por favor, digite um numero valido."); //controle de erro
            break;
        }
    }
}

void ReallocAdiciona(void **pBuffer, int **menu, long long int **qtdAlocada, char **nome, char **idade, char **email, void **fim)
{
    void *tempEnder = *pBuffer; //armazena o enderço atual do buffer
    **qtdAlocada = (sizeof(char) * strlen(*nome) + 1 + sizeof(char) * strlen(*idade) + 1 + sizeof(char) * strlen(*email) + 1); // pega o tamanho do nome, idade e email digitados para realocar apenas o tamanho necessário
    *pBuffer = realloc(*pBuffer, (((long long int)((char *)*fim - (char *)*pBuffer)) + **qtdAlocada));//tamanho do buffer + nome + idade + email                          
                                                //fim - pBuffer = tamanho total do pBuffer
    if (*pBuffer == NULL)
    {
        printf("Nao foi possivel realocar memoria");
        exit(0);
    }

    if (tempEnder != *pBuffer) //se o endereço do pBuffer mudou após o realloc, atualiza todos os ponteiros
    {
        *menu = (int *)*pBuffer;
        *qtdAlocada = (long long int *)((char *)*pBuffer + sizeof(int));
        *nome = *pBuffer + MENU;
        *idade = *pBuffer + MENU + TAM_NOME;
        *email = *pBuffer + MENU + TAM_NOME + TAM_IDADE;
        **qtdAlocada = (sizeof(char) * strlen(*nome) + 1 + sizeof(char) * strlen(*idade) + 1 + sizeof(char) * strlen(*email) + 1);
        *fim = (char *)(*pBuffer) + (long long int)(((char *)*fim - (char *)tempEnder));
    }

    tempEnder = NULL;
}

void Adiciona(char **nome, char **idade, char **email, void **fim, void *temp, long long int **qtdAlocada)
{
    //copia o conteúdo do input para seu devido lugar no buffer
    strcpy((char *)temp, *nome);
    temp = (char *)temp + strlen(*nome) + 1; //se for sem ordenação, temp é sempre = fim, 
    strcpy((char *)temp, *idade);        //se for com ordenação, temp é quando a função de ordenação encontra um lugar válido
    temp = (char *)temp + strlen(*idade) + 1;
    strcpy((char *)temp, *email);
    temp = (char *)temp + strlen(*email) + 1;
    *fim = (char *)*fim + **qtdAlocada;
}

void ProxCampo(void **temp)
{
    //percorre o campo atual (nome, idade ou email) até encontrar o final, e retorna o próximo campo
    *temp = (char *)*temp + strlen((char *)*temp) + 1;
}

bool Busca(void *pBuffer, char *nome, void **temp, void *fim)
{
    *temp = pBuffer + FIM; //temp = final do buffer inicial (antes do nome,idade e email do usuário)

    if (*temp == fim)
    {
        printf("\n  Agenda vazia!\n");
        return false;
    }
    else
    {
        while (*temp != fim) //percorre toda agenda até o final
        {
            if (strcmp(nome, (char *)*temp) == 0) //se nome atual for igual ao nome onde temp está apontando
                return true;
            else
            {
                ProxPessoa(temp); // se não for igual, pula 3 campos até a próxima pessoa
            }
        }
        printf("\n  Nome nao encontrado!\n");
        return false;
    }
}

void Lista(void *pBuffer, void **temp, void *fim)
{
    *temp = pBuffer + FIM; //temp = final do buffer inicial

    if (*temp == fim)
    {
        printf("\n  Agenda vazia!\n");
        return;
    }
    printf("\n--------------------------");
    while (*temp != fim) //percorre toda agenda, imprimindo cada campo que o ponteiro temp passa
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

void Remove(void **pBuffer, char **nome, char **idade, char **email, int **menu, long long int **qtdAlocada, void *temp, void **fim)
{
    void *tempRemove = NULL; //ponteiro temporário para encontrar o destino do memmove
    void *tempRealloc = *pBuffer; //aponta para o pBuffer antes do realloc, caso o realloc mude o endereço de pBuffer
    if (Busca(*pBuffer, *nome, &tempRemove, *fim)) 
    {//procura o nome na agenda, se encontrado, tempRemove aponta para o nome a ser eliminado
        temp = tempRemove; 
        ProxPessoa(&temp); //temp agora aponta para o sucessor da pessoa quer quer se eliminar

        //pega todo o bloco de memória a partir de temp e arrasta esse bloco para a esquerda,
        // sobreescrevendo a pessoa quer quer eliminar
        memmove(tempRemove, temp, (long long int)((char *)*fim - (char *)temp));

        //realloca o tamanho do buffer, diminuindo exatamente o tamanho da pessoa eliminada
        *pBuffer = realloc(*pBuffer, (long long int)((char *)((char *)*fim - (char *)*pBuffer) - //(fim - pBuffer) = tamanho atual do bloco
                    (char *)(((char *)temp - (char *)tempRemove)))); //(temp - tempRemove) = tamanho da pessoa eliminada

        if (*pBuffer == NULL)
        {
            printf("\nNao foi possivel realocar memoria\n");
            exit(0);
        }

        if (tempRealloc != *pBuffer)//se o pBuffer mudou de endereço, atualiza os ponteiros
        {
            *menu = (int *)*pBuffer;
            *qtdAlocada = (long long int *)((char *)*pBuffer + sizeof(int));
            *nome = *pBuffer + MENU;
            *idade =*pBuffer + MENU + TAM_NOME;
            *email = *pBuffer + MENU + TAM_NOME + TAM_IDADE;
            *fim = *pBuffer + (long long int)((char *)*fim - (char *)tempRealloc) - (long long int)((char *)temp - (char *)tempRemove);
        }
        else
        {
            *fim = (char *)*fim - (long long int)((char *)temp - (char *)tempRemove); //se não mudou de endereço, muda apenas o ponteiro fim
        }

        printf("\n  Nome Removido!\n");
    }
    tempRemove = NULL;
    tempRealloc = NULL;
}

void ProxPessoa(void **temp) //percorre 3 campos e faz temp apontar para a próxima pessoa
{
    ProxCampo(temp);
    ProxCampo(temp);
    ProxCampo(temp);
}

/* ========================================================================================================================
   | Essa parte toda foi uma tentativa de fazer a agenda ser ordenada por 3 jeitos diferentes: nome, idade ou email.      |
   | Não é eficiente, mas foi divertido de fazer.                                                                         |
   | E se tiver números no email, não ordena corretamente.                                                                |
   ======================================================================================================================== */

void OrdenaNome(void *pBuffer, long long int **qtdAlocada, char **nome, void **temp, void *fim)
{
    //semelhante à função de busca, porém, quando encontra um local válido para inserir o nome,
    //move todas as pessoas da agenda para a direita, no tamanho exato da pessoa atual, para inserir a nova pessoa no local certo
    *temp = pBuffer + FIM;
    *nome = pBuffer + MENU;
    if (*temp == fim)
        return;
    while (strcmp(*nome, *temp) > 0 && *temp != fim)
        ProxPessoa(temp);
    if (*temp == fim)
        return;
    else
        //temp + qtdAlocada = tamanho da pessoa atual
        //fim - temp = tamanho atual do bloco
        //pega todo o bloco de memória, do local válido até o final do bloco, e o move para a direita
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
}

void OrdenaIdade(void *pBuffer, long long int **qtdAlocada, char **idade, void **temp, void *fim)
{
    //semelhante ao OrdenaNome, mas percorre os campos de idade, e transforma o campo idade atual em um inteiro para fazer a comparação
    *temp = pBuffer + FIM;
    *idade = pBuffer + MENU + TAM_NOME;
    if (*temp == fim)
        return;
    char *tempIdade = pBuffer + FIM;
    ProxCampo((void *)&tempIdade);//aponta para a idade da primeira pessoa inserida na lista

    while (atoi(tempIdade) <= atoi(*idade) && *temp != fim)//precisa transformar para inteiro pois 9 é maior que 10 na tabela ascii, se for comparar pelo strcmp
    {
        ProxPessoa(temp);//temp sempre aponta para o nome da pessoa atual, para não ter erro na hora do memmove
        ProxPessoa((void *)(&tempIdade));//percorre os campos de idade
    }
    if (*temp == fim)
        return;
    else
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
    tempIdade = NULL;
}

void OrdenaEmail(void *pBuffer, long long int **qtdAlocada, char **email, void **temp, void *fim)
{
    //mesma coisa que o OrdenaNome, mas percorrendo os campos do email
    *temp = pBuffer + FIM;
    *email = pBuffer + MENU + TAM_NOME + TAM_IDADE;
    if (*temp == fim)
        return;
    char *tempEmail = pBuffer + FIM;
    ProxCampo((void *)&tempEmail);
    ProxCampo((void *)&tempEmail); // aponta para o primeiro email inserido na lista

    while (strcmp(*email, *temp) > 0 && *temp != fim)
    {
        ProxPessoa(temp);//aponta para o nome atual
        ProxPessoa((void *)(&tempEmail));//percorre os campos de email
    }
    if (*temp == fim)
        return;
    else
        memmove((char *)(*temp) + **qtdAlocada, *temp, (long long int)((char *)(fim) - (char *)(*temp)));
    tempEmail = NULL;
}