#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct celula{
        char nome[100];
        int ano;
        int id;
        struct celula *prox;
};


// int comparar(struct celula *cabeca);
// void insere_ordem(char nome[100], int ano_ordem, int id, struct celula *cabeca);
// void ler_string(char nome[100], int tamanho);
// void imprime(struct celula * cabeca);
// int menu();
// void remover(int id, struct celula *cabeca);
// void grava_arquivo (struct celula * cabeca);
// void le_arquivo (struct celula * cabeca);





int main(){
    int opcao;
    struct celula  *cabeca, *q,*p;
    int ano,id;
    char nome[100];

    cabeca = (struct celula *) malloc (sizeof(struct celula));
    cabeca->prox=NULL;


    for(;;){
        opcao = menu();
    switch (opcao) {

        case 1: 
        printf("\n\n\tNome do filme: ");
        getchar();
        ler_string(nome, 100);
        printf("\tAno do filme: ");
        scanf("%d", &ano);
        printf("\tID do filme: ");
        id = comparar(cabeca);
        insere_ordem(nome, ano, id, cabeca);
        break;

        case 2: 
        printf("\nDigite o ID do filme a ser removido: ");
        scanf("%d", &id);
        remover(id, cabeca);
        break;

        case 3: imprime(cabeca);
        break; 

        case 4:
                grava_arquivo(cabeca);
                break;

        case 5:
                le_arquivo(cabeca);
                break;

        case 6:
        p=cabeca;
        q=cabeca->prox;

        while (q != NULL) {
        free(p);
		p=q;
		q=q->prox;

 }
        free(p);
        exit(0);
        break;


}

}

}

void ler_string(char nome[100], int tamanho)
{
	int i = 0;
	char c;

	c = getchar();
	while ((c != '\n') && (i < tamanho)) {
		nome[i++] = c;
		c = getchar();
	}
	nome[i] = '\0';

	if (c != '\n') {
		c = getchar();
		while ((c != '\n') && (c != EOF)) {
			c = getchar();
		}
	}

}

int menu(){
    char c=0;
    int i;
    i=0;

do{
    printf("\n\n\t1. Inserir um filme\n");
    printf("\t2. Remover um filme\n");
    printf("\t3. Imprimir lista\n");
    printf("\t4. Gravar arquivo\n");
    printf("\t5. Ler arquivo\n");
    printf("\t6. Sair\n");
    printf("\tEscolha uma opcao: ");

   if (scanf("%d", &i) != 1)
    while(c=getchar() != '\n' && c !=EOF);
   
}   
 while (i<=0 || i>6);

    return i;
    }

void insere_ordem(char nome[100], int ano_ordem, int id, struct celula *cabeca){
    struct celula *novo,*p,*q;
    int n;

    p=cabeca;
    q=cabeca->prox;
    
    novo = (struct celula *) malloc(sizeof(struct celula));
    
        strcpy(novo->nome,nome);
        novo->ano=ano_ordem;
        novo->id=id;



    
    while (q!=NULL && q->ano<ano_ordem) {
        p=q;
        q=q->prox;
    }

    if (q != NULL && q->ano == ano_ordem) {
        n = strcmp(q->nome, nome);
        if (n > 0) {  
            novo->prox = q;
            p->prox = novo;
        } else if (n < 0) {  
            novo->prox = q->prox;
            q->prox = novo;
        } else {  
            novo->prox = q->prox;
            q->prox = novo;
        }
    }

    else{
        novo->prox=q;
        p->prox=novo;
    }
}


int comparar (struct celula *cabeca){
    int id;
    struct celula *p,*q;

    scanf("%d", &id);

    do{
    p=cabeca;
    q=cabeca->prox;
    
    while (q != NULL && q->id!=id){
        p=q;
        q=q->prox;
    }

        if (q==NULL)
            break;
        else {
            printf("\nID repetido, digite outro: ");
            scanf("%d", &id);
        }
    } while(1);
         return id;
    }

   




void imprime(struct celula * cabeca){
    struct celula *p;

        printf("\n\t----------------------------------------------------------\n");
    for (p=cabeca->prox;p !=NULL;p=p->prox){

        printf("\n\tFilme: %s\n", p->nome);
        printf("\tAno de lancamento: %d\n", p->ano);
        printf("\tID: %d\n", p->id);
        printf("\n\t----------------------------------------------------------\n");


    }


}

void remover(int id, struct celula *cabeca){
        struct celula *p,*q;
        p=cabeca;
        q=cabeca->prox;

       while(q!=NULL && q->id!=id){
        p=q;
        q=q->prox;
       }

       if(q==NULL)
       printf("\nID nao encontrado");
       else {        
        p->prox=q->prox;
        free(q);
      }
}

void grava_arquivo (struct celula * cabeca){
    struct celula *q;

    FILE *arq;

    arq = fopen("Dados.txt", "w");

    for(q=cabeca->prox;q!=NULL;q=q->prox){
        fprintf(arq, "%s\n", q->nome);
        fprintf(arq,"%d\n",q->ano);
        fprintf(arq,"%d\n",q->id);

    }

    fclose(arq);


}

void le_arquivo (struct celula * cabeca){
    struct celula *novo;
    FILE *arq;
    char lixo,nome[100];

    arq = fopen("Dados.txt", "r");

    while (fscanf(arq,"%[^\n]s",nome)==1){

        novo = (struct celula *) malloc(sizeof(struct celula));

        strcpy(novo->nome, nome);
        fscanf(arq, "%c", &lixo);
        fscanf(arq,"%d", &novo->ano);
        fscanf(arq, "%c", &lixo);
        fscanf(arq,"%d", &novo->id);
        fscanf(arq, "%c", &lixo);


    novo->prox=cabeca->prox;
    cabeca->prox=novo;


    }
fclose(arq);

}