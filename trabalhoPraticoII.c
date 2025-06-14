#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 10
#define TAMANHO_NOME 100
#define TAMANHO_CURSO 100

typedef struct Item
{
    int matricula;
    char nome[TAMANHO_NOME];
    char curso[TAMANHO_CURSO];
} Item;

typedef struct Celula *Apontador;

typedef struct Celula
{
    Item item;
    Apontador prox;
} Celula;

typedef struct Lista
{
    Apontador primeiro, ultimo;
} Lista;

Lista tabela[TAMANHO_TABELA];

int hash(int matricula)
{
    return matricula % TAMANHO_TABELA;
}

void InicializaLista(Lista *lista)
{
    lista->primeiro = (Apontador)malloc(sizeof(Celula));
    lista->ultimo = lista->primeiro;
    lista->primeiro->prox = NULL;
}

void InicializaTabela()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        InicializaLista(&tabela[i]);
    }
}

void InserirAluno()
{
    Item novo;
    printf("Matricula: ");
    scanf("%d", &novo.matricula);
    getchar();

    int indice = hash(novo.matricula);
    Lista *lista = &tabela[indice];

    Apontador atual = lista->primeiro->prox;
    while (atual != NULL)
    {
        if (atual->item.matricula == novo.matricula)
        {
            printf("Erro: matricula ja existente!\n");
            return;
        }
        atual = atual->prox;
    }

    printf("Nome: ");
    fgets(novo.nome, TAMANHO_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Curso: ");
    fgets(novo.curso, TAMANHO_CURSO, stdin);
    novo.curso[strcspn(novo.curso, "\n")] = '\0';

    Apontador novaCelula = (Apontador)malloc(sizeof(Celula));
    novaCelula->item = novo;
    novaCelula->prox = NULL;

    lista->ultimo->prox = novaCelula;
    lista->ultimo = novaCelula;

    printf("Aluno inserido com sucesso!\n");
}

void RemoverAluno()
{
    int matricula;
    printf("Matricula a remover: ");
    scanf("%d", &matricula);

    int indice = hash(matricula);
    Lista *lista = &tabela[indice];

    Apontador anterior = lista->primeiro;
    Apontador atual = anterior->prox;

    while (atual != NULL)
    {
        if (atual->item.matricula == matricula)
        {
            anterior->prox = atual->prox;
            if (atual == lista->ultimo)
            {
                lista->ultimo = anterior;
            }
            free(atual);
            printf("Aluno removido com sucesso!\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Aluno nao encontrado.\n");
}

void BuscarAluno()
{
    int matricula;
    printf("Matricula a buscar: ");
    scanf("%d", &matricula);

    int indice = hash(matricula);
    Apontador atual = tabela[indice].primeiro->prox;

    while (atual != NULL)
    {
        if (atual->item.matricula == matricula)
        {
            printf("Nome: %s\nCurso: %s\n", atual->item.nome, atual->item.curso);
            return;
        }
        atual = atual->prox;
    }

    printf("Aluno nao encontrado.\n");
}

void ExibirTabela()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        printf("Bucket %d:\n", i);
        Apontador atual = tabela[i].primeiro->prox;
        while (atual != NULL)
        {
            printf("  [%d] %s - %s\n", atual->item.matricula, atual->item.nome, atual->item.curso);
            atual = atual->prox;
        }
    }
}

void Estatisticas()
{
    int total = 0, maior = 0, bucketMaior = 0;

    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        int count = 0;
        Apontador atual = tabela[i].primeiro->prox;
        while (atual != NULL)
        {
            count++;
            atual = atual->prox;
        }
        total += count;
        if (count > maior)
        {
            maior = count;
            bucketMaior = i;
        }
    }

    printf("Total de alunos: %d\n", total);
    printf("Quantidade de buckets: %d\n", TAMANHO_TABELA);
    printf("Media por bucket: %.2f\n", (float)total / TAMANHO_TABELA);
    printf("Bucket com mais colisoes: %d (%d alunos)\n", bucketMaior, maior);
}

void Menu()
{
    int op;
    do
    {
        printf("\n1. Inserir\n2. Remover\n3. Buscar\n4. Exibir\n5. Estatisticas\n0. Sair\nOpcao: ");
        scanf("%d", &op);
        getchar();
        switch (op)
        {
        case 1:
            InserirAluno();
            break;
        case 2:
            RemoverAluno();
            break;
        case 3:
            BuscarAluno();
            break;
        case 4:
            ExibirTabela();
            break;
        case 5:
            Estatisticas();
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

int main()
{
    InicializaTabela();
    Menu();
    return 0;
}