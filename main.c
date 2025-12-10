#include "funcoes.h"

int main() {
    AVLTree* skills_tree = Avl_create();
    int points = 0;
    const int MAX_SKILLS = 7;

    printf("\t=== Aprendiz de Mago ===\n"
           "Voce e um jovem aprendiz de mago em um mundo desconhecido.\n"
           "Para comecar o treinamento de hoje voce deve fazer escolhas para adquirir pontos e aumentar suas habilidades\n"
           "Boa sorte meu jovem, escolha:\n"
           "1. Lutar contra um goblin \n"
           "2. Estudar antigos livros\n");
    printf("Escolha (1 ou 2): ");
    int choice;
    scanf("%d", &choice);
    int gained = 0;
    if (choice == 1) {
        gained = 20;
        printf("Voce derrotou o goblin! +20 pontos.\n");
    } else if (choice == 2) {
        gained = 15;
        printf("Voce ganhou conhecimento! +15 pontos.\n");
    } else {
        printf("Escolha invalida. Comecando com 0 pontos.\n");
    }
    points += gained;
    printf("Pontos iniciais: %d\n\n", points);

    printf("Deseja acessar o menu de habilidades (1) ou continuar a jornada (2)? ");
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {
        skills_menu(skills_tree, &points);
    }

    for (int stage = 1; stage <= 5; stage++) {
        gained = 0;
        printf("\n--- Situacao %d ---\n", stage);

        if (stage == 1) {
            printf("Voce encontra um animal ferido na floresta.\n"
                   "1. Cura-lo com magia\n"
                   "2. Deixa-lo e continuar\n"
                   "Escolha (1 ou 2): ");
            scanf("%d", &choice);
            if (choice == 1) gained = 15;
            else gained = 5;
        } else if (stage == 2) {
            printf("Uma esfinge bloqueia seu caminho com um enigma.\n"
                   "1. Resolver o enigma \n"
                   "2. Fugir pela floresta\n"
                   "Escolha (1 ou 2): ");
            scanf("%d", &choice);
            if (choice == 1) gained = 20;
            else gained = 10;
        } else if (stage == 3) {
            printf("Voce descobre um bau de tesouro, mas parece armadilhado\n"
                   "1. Abrir com cuidado \n"
                   "2. Ignorar e prosseguir\n"
                   "Escolha (1 ou 2): ");
            scanf("%d", &choice);
            if (choice == 1) gained = 25;
            else gained = 10;
        } else if (stage == 4) {
            printf("Uma vila esta sendo atacada por bandidos.\n"
                   "1. Lutar contra os bandidos \n"
                   "2. Negociar uma tregua\n"
                   "Escolha (1 ou 2): ");
            scanf("%d", &choice);
            if (choice == 1) gained = 30;
            else gained = 20;
        } else if (stage == 5) {
            printf("Voce enfrenta um dragao guardiao.\n"
                   "1. Usar magia poderosa\n"
                   "2. Passar furtivamente\n"
                   "Escolha (1 ou 2): ");
            scanf("%d", &choice);
            if (choice == 1) gained = 40;
            else gained = 25;
        }

        points += gained;
        printf("Voce ganhou %d pontos! Total agora: %d\n", gained, points);

        printf("\nDeseja acessar o menu de habilidades (1) ou continuar a jornada (2)? ");
        scanf("%d", &opt);
        if (opt == 1) {
            skills_menu(skills_tree, &points);
        }
        printf("\n");
    }

    printf("\nJornada principal concluida! Agora gerencie suas habilidades ou saia.\n");
    int option;
    while (1) {
        printf("Deseja acessar o menu de habilidades (1) ou sair do jogo (4)? ");
        scanf("%d", &option);
        if (option == 1) {
            skills_menu(skills_tree, &points);
        } else if (option == 4) {
            printf("Obrigado por jogar! Habilidades finais: %zu\n", Avl_size(skills_tree));
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    }

    Avl_destroy(skills_tree);
    return 0;
} 
