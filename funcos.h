#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    size_t size;
} AVLTree;

//habilidades pré-definidas
typedef struct {
    int id;
    const char* name;
    int cost;
} Skill;

Skill available_skills[10] = {
    {1, "Fireball", 10},
    {2, "Heal", 15},
    {3, "Shield", 20},
    {4, "Teleport", 25},
    {5, "Invisibility", 30},
    {6, "Lightning", 35},
    {7, "Ice Storm", 40},
    {8, "Mind Control", 45},
    {9, "Earthquake", 50},
    {10, "Dragon Breath", 55}
};

//nome e custo por ID
bool get_skill_info(int id, const char** name, int* cost) {
    for (int i = 0; i < 10; i++) {
        if (available_skills[i].id == id) {
            *name = available_skills[i].name;
            *cost = available_skills[i].cost;
            return true;
        }
    }
    return false;
}

//habilidades ordenadas com nomes
static void skills_in_order(AVLNode *n) {
    if (!n) return;
    skills_in_order(n->left);
    const char* name;
    int cost;
    if (get_skill_info(n->key, &name, &cost)) {
        printf("%d: %s ", n->key, name);
    } else {
        printf("%d ", n->key);
    }
    skills_in_order(n->right);
}

void print_skills_inorder(const AVLTree* t) {
    if (!t || !t->root) {
        printf("Nenhuma habilidade adquirida.\n");
        return;
    }
    printf("Habilidades ordenadas por ID:\n");
    skills_in_order(t->root);
    printf("\n");
}


//Funções feitas em sala de aula
static int Max(int a, int b) {
    return a > b ? a : b;
}

static int Height(AVLNode *n) {
    return n ? n->height : 0;
}

static int Balance_factor(AVLNode *n) {
    return n ? Height(n->left) - Height(n->right) : 0;
}

static AVLNode* Make_node(int key) {
    AVLNode *n = (AVLNode*)malloc(sizeof(AVLNode));
    if (!n) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

static void Free_node(AVLNode *n) {
    if (!n) return;
    Free_node(n->left);
    Free_node(n->right);
    free(n);
}

static AVLNode* Rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *aux = x->right;
    
    x->right = y;
    y->left = aux;
    
    y->height = 1 + Max(Height(y->left), Height(y->right));
    x->height = 1 + Max(Height(x->left), Height(x->right));
    return x;
}

static AVLNode* Rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *aux = y->left;
    
    y->left = x;
    x->right = aux;
    
    x->height = 1 + Max(Height(x->left), Height(x->right));
    y->height = 1 + Max(Height(y->left), Height(y->right));
    return y;
}

static AVLNode* Balance(AVLNode *n) {
    if (!n) return NULL;
    n->height = 1 + Max(Height(n->left), Height(n->right));
    int balanceFactor = Balance_factor(n);
    
    if (balanceFactor > 1 && Balance_factor(n->left) >= 0)
        return Rotate_right(n);
    if (balanceFactor > 1 && Balance_factor(n->left) < 0) {
        n->left = Rotate_left(n->left);
        return Rotate_right(n);
    }
    if (balanceFactor < -1 && Balance_factor(n->right) <= 0)
        return Rotate_left(n);
    if (balanceFactor < -1 && Balance_factor(n->right) > 0) {
        n->right = Rotate_right(n->right);
        return Rotate_left(n);
    }
    return n;
}

static AVLNode* Insert_recursive(AVLNode *node, int key, bool *inserted) {
    if (!node) {
        *inserted = true;
        return Make_node(key);
    }
    if (key < node->key)
        node->left = Insert_recursive(node->left, key, inserted);
    else if (key > node->key)
        node->right = Insert_recursive(node->right, key, inserted);
    else
        return node;
    
    return Balance(node);
}

static AVLNode* Min_node(AVLNode *n) {
    while (n && n->left) n = n->left;
    return n;
}

static AVLNode* Remove_recursiv(AVLNode *node, int key, bool *removed) {
    if (!node) return NULL;
    
    if (key < node->key) {
        node->left = Remove_recursiv(node->left, key, removed);
    } else if (key > node->key) {
        node->right = Remove_recursiv(node->right, key, removed);
    } else {
        *removed = true;
        
        if (node->left == NULL) {
            AVLNode *temp = node;
            node = node->right;
            free(temp);
            return node;
        } else if (node->right == NULL) {
            AVLNode *temp = node;
            node = node->left;
            free(temp);
            return node;
        } else {
            AVLNode *sucessor = Min_node(node->right);
            node->key = sucessor->key;
            node->right = Remove_recursiv(node->right, sucessor->key, removed);
        }
    }
    return Balance(node);
}

static bool Search_recursive(AVLNode *n, int key) {
    if (!n) return false;
    if (key == n->key) return true;
    return key < n->key ? Search_recursive(n->left, key) : Search_recursive(n->right, key);
}

static void In_order_print(AVLNode *n) {
    if (!n) return;
    In_order_print(n->left);
    printf("%d ", n->key);
    In_order_print(n->right);
}

static void Print_recursiv(AVLNode *n, int depth, char branch) {
    if (!n) return;
    Print_recursiv(n->right, depth + 1, '/');
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    if (depth) printf("%c ", branch);
    printf("%d (h=%d)\n", n->key, n->height);
    Print_recursiv(n->left, depth + 1, '\\');
}

// Funções públicas
void Avl_print_inorder(const AVLTree *t) {
    In_order_print(t ? t->root : NULL);
    printf("\n");
}

AVLTree* Avl_create(void) {
    AVLTree *t = (AVLTree*)malloc(sizeof(AVLTree));
    if (!t) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    t->root = NULL;
    t->size = 0;
    return t;
}

void Avl_destroy(AVLTree *t) {
    if (!t) return;
    Free_node(t->root);
    free(t);
}

bool Avl_insert(AVLTree *t, int key) {
    bool inserted = false;
    t->root = Insert_recursive(t->root, key, &inserted);
    if (inserted) t->size++;
    return inserted;
}

bool Avl_remove(AVLTree *t, int key) {
    bool removed = false;
    t->root = Remove_recursiv(t->root, key, &removed);
    if (removed) t->size--;
    return removed;
}

bool Avl_search(const AVLTree *t, int key) {
    return Search_recursive(t ? t->root : NULL, key);
}

size_t Avl_size(const AVLTree *t) {
    return t ? t->size : 0;
}

void Avl_print(const AVLTree *t) {
    if (!t || !t->root) {
        printf("Arvore vazia\n");
        return;
    }
    Print_recursiv(t->root, 0, '*');
}

//função especifica para o jogo
//essa função mostra o menu do jogo
//aqui é onde as ações de inserir, remover e ver a impreção da arvore acontecem
void skills_menu(AVLTree* skills_tree, int* points) {
    const int MAX_SKILLS = 7;
    int option;
    while (1) {
        printf("\n\t=== Menu ===\n"
               "Pontos atuais: %d | Habilidades: %zu / 10 (max %d)\n", *points, Avl_size(skills_tree), MAX_SKILLS);
        printf("1. Ver habilidades\n"
               "2. Comprar nova habilidade\n"
               "3. Remover habilidade\n"
               "4. Sair\n"
               "Escolha: ");
        scanf("%d", &option);

        if (option == 1) {
            printf("\n\t--- Sua Arvore de habilidades ---\n");
            Avl_print(skills_tree);
            printf("\n--- Habilidades in-order (IDs) ---\n");
            print_skills_inorder(skills_tree);
            printf("Consulte os nomes abaixo:\n");
            for (int i = 0; i < 10; i++) {
                printf("%d: %s (custo %d)\n", available_skills[i].id, available_skills[i].name, available_skills[i].cost);
            }
        } else if (option == 2) {
            size_t current_size = Avl_size(skills_tree);
            if (current_size >= MAX_SKILLS) {
                printf("Voce ja tem o maximo de %d habilidades! Remova uma para comprar nova.\n", MAX_SKILLS);
                continue;
            }
            printf("\n--- Habilidades disponiveis ---\n");
            for (int i = 0; i < 10; i++) {
                printf("%d. %s (custo: %d)", available_skills[i].id, available_skills[i].name, available_skills[i].cost);
                if (Avl_search(skills_tree, available_skills[i].id)) {
                    printf(" - JA ADQUIRIDA\n");
                } else {
                    printf(" - Disponivel\n");
                }
            }
            printf("Escolha o ID: ");
            int id;
            scanf("%d", &id);
            const char* name;
            int cost;
            if (!get_skill_info(id, &name, &cost)) {
                printf("ID invalido!\n");
                continue;
            }
            if (Avl_search(skills_tree, id)) {
                printf("Habilidade ja adquirida!\n");
                continue;
            }
            if (*points < cost) {
                printf("Pontos insuficientes! Precisa de %d.\n", cost);
                continue;
            }
            if (Avl_insert(skills_tree, id)) {
                *points -= cost;
                printf("Comprada: %s! Pontos restantes: %d\n", name, *points);
            } else {
                printf("Erro na insercao.\n");
            }
        } else if (option == 3) {
            printf("\nEscolha o ID para remover: ");
            int id;
            scanf("%d", &id);
            if (!Avl_search(skills_tree, id)) {
                printf("Habilidade nao adquirida!\n");
                continue;
            }
            const char* name;
            int cost;
            if (!get_skill_info(id, &name, &cost)) {
                printf("ID invalido!\n");
                continue;
            }
            if (Avl_remove(skills_tree, id)) {
                *points += cost;
                printf("Removida: %s! Reembolsados %d pontos. Total: %d\n", name, cost, *points);
            } else {
                printf("Erro na remocao.\n");
            }
        } else if (option == 4) {
            return;
        } else {
            printf("Opcao invalida!\n");
        }
    }
}
