#include <iostream>
using namespace std;

// ==========================================
// ESTRUTURA DA FILA DE SENHAS
// ==========================================
struct NoSenha {
    int senha;
    NoSenha *prox;
};

struct Fila {
    NoSenha *inicio;
    NoSenha *fim;
    int qtde;
};

Fila* initFila() {
    Fila *f = new Fila();
    f->inicio = NULL;
    f->fim = NULL;
    f->qtde = 0;
    return f;
}

int isFilaVazia(Fila *f) {
    return (f->inicio == NULL);
}

void enqueue(Fila *f, int senha) {
    NoSenha *novo = new NoSenha();
    novo->senha = senha;
    novo->prox = NULL;

    if (isFilaVazia(f)) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->qtde++;
}

int dequeue(Fila *f) {
    if (isFilaVazia(f)) {
        return -1;
    }
    NoSenha *temp = f->inicio;
    int senha = temp->senha;
    f->inicio = f->inicio->prox;
    
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    delete temp;
    f->qtde--;
    return senha;
}

void printFila(Fila *f) {
    if (isFilaVazia(f)) {
        cout << "(Nenhuma senha atendida por este guiche ainda)" << endl;
        return;
    }
    NoSenha *atual = f->inicio;
    while (atual != NULL) {
        cout << "[ " << atual->senha << " ] ";
        atual = atual->prox;
    }
    cout << endl;
}

void freeFila(Fila *f) {
    NoSenha *atual = f->inicio;
    while (atual != NULL) {
        NoSenha *prox = atual->prox;
        delete atual;
        atual = prox;
    }
    delete f;
}

// ==========================================
// ESTRUTURA DA LISTA DE GUICHÊS
// ==========================================
struct Guiche {
    int id;
    Fila *senhasAtendidas; // Cada guichê tem sua própria fila de atendidas
    Guiche *prox;
};

struct ListaGuiches {
    Guiche *inicio;
    int qtde;
};

ListaGuiches* initLista() {
    ListaGuiches *l = new ListaGuiches();
    l->inicio = NULL;
    l->qtde = 0;
    return l;
}

// Busca se o guichê já existe
Guiche* buscarGuiche(ListaGuiches *l, int id) {
    Guiche *atual = l->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Adiciona um novo guichê na lista
int inserirGuiche(ListaGuiches *l, int id) {
    if (buscarGuiche(l, id) != NULL) {
        return 0; // Erro: ID já existente
    }
    
    Guiche *novo = new Guiche();
    novo->id = id;
    novo->senhasAtendidas = initFila(); // Inicializa a fila interna do guichê
    novo->prox = l->inicio; // Insere no início da lista para simplificar
    l->inicio = novo;
    l->qtde++;
    return 1;
}

void freeLista(ListaGuiches *l) {
    Guiche *atual = l->inicio;
    while (atual != NULL) {
        Guiche *prox = atual->prox;
        freeFila(atual->senhasAtendidas); // Limpa a fila interna primeiro
        delete atual;
        atual = prox;
    }
    delete l;
}

// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================
int main() {
    Fila *senhasGeradas = initFila();
    ListaGuiches *listaGuiches = initLista();
    
    int opcao;
    int controleSenha = 1;

    do {
        cout << "\n========================================" << endl;
        cout << "        SISTEMA DE ATENDIMENTO 2.0" << endl;
        cout << "   Senhas aguardando: " << senhasGeradas->qtde << endl;
        cout << "   Guiches abertos  : " << listaGuiches->qtde << endl;
        cout << "========================================" << endl;
        cout << "0. Sair" << endl;
        cout << "1. Gerar senha" << endl;
        cout << "2. Abrir guiche" << endl;
        cout << "3. Realizar atendimento" << endl;
        cout << "4. Listar senhas atendidas por guiche" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cout << "----------------------------------------" << endl;

        switch (opcao) {
            case 1: // Gerar Senha
                enqueue(senhasGeradas, controleSenha);
                cout << "[SUCESSO] Senha " << controleSenha << " gerada." << endl;
                controleSenha++;
                break;

            case 2: // Abrir Guichê
                int idNovoGuiche;
                cout << "Digite o ID numérico para o novo guiche: ";
                cin >> idNovoGuiche;
                if (inserirGuiche(listaGuiches, idNovoGuiche)) {
                    cout << "[SUCESSO] Guiche " << idNovoGuiche << " aberto e pronto." << endl;
                } else {
                    cout << "[ERRO] Já existe um guiche aberto com o ID " << idNovoGuiche << "." << endl;
                }
                break;

            case 3: // Realizar Atendimento
                if (isFilaVazia(senhasGeradas)) {
                    cout << "[AVISO] Não há senhas aguardando atendimento." << endl;
                } else if (listaGuiches->qtde == 0) {
                    cout << "[ERRO] Não há nenhum guiche aberto para realizar o atendimento." << endl;
                } else {
                    int idGuicheAtendimento;
                    cout << "Digite o ID do guiche que vai atender: ";
                    cin >> idGuicheAtendimento;
                    
                    Guiche *g = buscarGuiche(listaGuiches, idGuicheAtendimento);
                    if (g == NULL) {
                        cout << "[ERRO] Guiche " << idGuicheAtendimento << " não encontrado." << endl;
                    } else {
                        int senhaChamada = dequeue(senhasGeradas);
                        enqueue(g->senhasAtendidas, senhaChamada);
                        cout << "[ATENDIMENTO] Guiche " << g->id << " chamando a senha: " << senhaChamada << endl;
                    }
                }
                break;

            case 4: // Listar Senhas Atendidas por Guichê
                if (listaGuiches->qtde == 0) {
                    cout << "[AVISO] Nenhum guiche foi aberto ainda." << endl;
                } else {
                    int idGuicheBusca;
                    cout << "Digite o ID do guiche para relatorio: ";
                    cin >> idGuicheBusca;
                    
                    Guiche *g = buscarGuiche(listaGuiches, idGuicheBusca);
                    if (g == NULL) {
                        cout << "[ERRO] Guiche " << idGuicheBusca << " não encontrado." << endl;
                    } else {
                        cout << "Senhas atendidas pelo Guiche " << g->id << ":" << endl;
                        printFila(g->senhasAtendidas);
                    }
                }
                break;

            case 0: // Sair
                if (!isFilaVazia(senhasGeradas)) {
                    cout << "[BLOQUEADO] Impossível encerrar. Existem " 
                         << senhasGeradas->qtde << " senhas na fila de espera." << endl;
                    opcao = -1; // Altera para continuar o loop
                } else {
                    // Calcula o total de senhas atendidas somando as filas de todos os guichês
                    int totalAtendidas = 0;
                    Guiche *atual = listaGuiches->inicio;
                    while (atual != NULL) {
                        totalAtendidas += atual->senhasAtendidas->qtde;
                        atual = atual->prox;
                    }
                    cout << "[ENCERRANDO] Sistema de atendimento finalizado com sucesso." << endl;
                    cout << "Total consolidado de senhas atendidas: " << totalAtendidas << endl;
                }
                break;

            default:
                cout << "[ERRO] Opção inválida!" << endl;
                break;
        }
    } while (opcao != 0);

    // Limpeza rigorosa da memória alocada dinamicamente
    freeFila(senhasGeradas);
    freeLista(listaGuiches);

    return 0;
}
