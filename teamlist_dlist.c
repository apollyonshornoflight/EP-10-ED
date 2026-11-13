#include "teamlist.h"
#include <stdlib.h>


struct node {
    Team *team;
    struct node *next;
    struct node *prev;
};

struct teamlist {
    struct node *head;
    struct node *tail;
    int size;
};

TeamList *tl_create(void) {
    TeamList *tl = malloc(sizeof(struct teamlist));
    if (tl == NULL) return NULL;

    tl->head = NULL;
    tl->tail = NULL;
    tl->size = 0;
    return tl;
}

void tl_append(TeamList *tl, Team *t) {
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL) return;

    n->team = t;
    n->next = NULL;
    n->prev = tl->tail;

    if (tl->tail == NULL)
        tl->head = n;
    else
        tl->tail->next = n;

    tl->tail = n;
    tl->size++;
}

int tl_size(TeamList *tl) {
    return tl->size;
}

Team *tl_get(TeamList *tl, int i) {
    struct node *cur = tl->head;
    for (int k = 0; k < i; k++)
        cur = cur->next;
    return cur->team;
}

void tl_free(TeamList *tl) {
    if (tl == NULL) return;

    struct node *cur = tl->head;
    while (cur != NULL) {
        struct node *next = cur->next; 
        team_free(cur->team);
        free(cur);
        cur = next;
    }
    free(tl);
}