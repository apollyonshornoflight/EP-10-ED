#include "team.h"
#include "teamlist.h"
#include <stdio.h>
#include <string.h>


#define LINE_SIZE 256
#define NAME_WIDTH 15


static int utf8_extra_bytes(const char *s) {
    int extras = 0;
    for (; *s != '\0'; s++)
        if (((unsigned char)*s & 0xC0) == 0x80)
            extras++;
    return extras;
}

int main(void) {
    TeamList *tl = tl_create();

    FILE *f = fopen("teams.csv", "r");
    if (f == NULL) {
        perror("teams.csv");
        tl_free(tl);
        return 1;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';

        int id;
        char name[LINE_SIZE];
        if (sscanf(line, "%d,%255[^\n]", &id, name) == 2) {
            Team *t = team_create(id, name);
            if (t != NULL)
                tl_append(tl, t);
        }
    }
    fclose(f);

    printf("%-3s %-*s %-3s %-3s %-3s %-3s %-3s %-3s %s\n",
           "ID", NAME_WIDTH, "Time", "V", "E", "D", "GM", "GS", "S", "PG");

    for (int i = 0; i < tl_size(tl); i++) {
        Team *t = tl_get(tl, i);

        int w  = team_wins(t);
        int d  = team_draws(t);
        int l  = team_losses(t);
        int gm = team_goals_scored(t);
        int gs = team_goals_conceded(t);
        int s  = gm - gs;
        int pg = 3 * w + d;

        const char *name = team_name(t);
        int pad = utf8_extra_bytes(name);

        printf("%-3d %-*s%*s %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
               team_id(t), NAME_WIDTH, name, pad, "",
               w, d, l, gm, gs, s, pg);
    }

    tl_free(tl);
    return 0;
}
