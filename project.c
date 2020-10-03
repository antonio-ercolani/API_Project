#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERINPUT 100

typedef struct alberoorig {
    char *id_ent;
    struct alberoorig *padre;
    struct alberoorig *sx;
    struct alberoorig *dx;
} nodo_orig;

typedef nodo_orig* puntnodoorig;

typedef struct alberodest {
    char *id_ent;
    struct alberodest *padre;
    struct alberodest *sx;
    struct alberodest *dx;
} nodo_dest;
typedef nodo_dest* puntnododest;

typedef struct alberoto {
    char *id_to;
    int counter;
    struct alberoto *padre;
    struct alberoto *sx;
    struct alberoto *dx;
} nodoto;
typedef nodoto* puntnodoto;

typedef struct alberorelazioni {
    struct alberorelazioni *padre;
    struct alberorelazioni *sx;
    struct alberorelazioni *dx;
    char *id_rel;
    puntnodoto testato;
} nodorel;

typedef nodorel* puntnodorel;

typedef struct alberorelazionien {
    struct alberorelazionien *padre;
    struct alberorelazionien *sx;
    struct alberorelazionien *dx;
    char *id_rel;
    puntnododest testadest;
    puntnodoorig testaorig;
} nodorelen;

typedef nodorelen* puntnodorelen;

typedef struct alberoentita {
    char *id_ent;
    struct alberoentita *padre;
    struct alberoentita *sx;
    struct alberoentita *dx;
    puntnodorelen testarel;
} nodoen;

typedef nodoen* puntnodoen;

void addent(char*);
void addrel(char*,char*,char*);
puntnodorel add_rel(char*);
int add_dest(puntnodorelen,char*,char*,puntnodoen);
void add_orig(puntnodorelen,char*,char*,puntnodoen);
void add_to(puntnodorel,char*);
void report(puntnodorel);
void sottoreport(puntnodoto, int);
void calcmax(puntnodoto);
void delrel(char*,char*,char*);
void search_del_orig(char*,char*,char*);
void delete_dest(puntnododest,puntnodorelen);
puntnododest treemin_dest(puntnododest);
void transplant_dest(puntnododest, puntnododest,puntnodorelen);
void delete_orig(puntnodoorig,puntnodorelen);
puntnodoorig treemin_orig(puntnodoorig);
void transplant_orig(puntnodoorig, puntnodoorig,puntnodorelen);
void decrease_to(char*,char*);
void deleteto(puntnodoto,puntnodorel);
puntnodoto treeminto(puntnodoto);
void transplantto(puntnodoto,puntnodoto,puntnodorel);
void deleterel(puntnodorel);
puntnodorel treeminrel(puntnodorel);
void transplantrel(puntnodorel,puntnodorel);
void delete_rel_en(puntnodorelen,puntnodoen);
puntnodorelen treemin_rel(puntnodorelen);
void transplant_rel(puntnodorelen, puntnodorelen,puntnodoen);
void delent(char*);
void visit_rel(puntnodorelen,char*);
void visit_orig(puntnodoorig,char *,puntnodorelen);
void visit_dest(puntnododest,char*,puntnodorelen);
void deleteent(puntnodoen);
void transplanten(puntnodoen,puntnodoen);
puntnodoen treeminen(puntnodoen);
void search_del_dest(char*,char*,char*);
void decrease_to_sum(char*,char*);

puntnodoen testaen;

puntnodorel testarel;

int appcalc;

int counter;

int main() {
    char id_ent[BUFFERINPUT];
    char comando[BUFFERINPUT];
    char id_orig[BUFFERINPUT];
    char id_dest[BUFFERINPUT];
    char id_rel[BUFFERINPUT];
    testaen = NULL;
    testarel = NULL;
    scanf("%s",comando);
    while (strcmp(comando,"end")!=0) {
        if (strcmp(comando,"addent")==0) {
            scanf("%s", id_ent);
            addent(id_ent);
        } else if (strcmp(comando,"addrel")==0) {
            scanf("%s", id_orig);
            scanf("%s", id_dest);
            scanf("%s", id_rel);
            addrel(id_orig,id_dest,id_rel);
        } else if (strcmp(comando,"report")==0) {
            if (testarel == NULL) {
                fputs("none\n", stdout);
            } else {
                report(testarel);
                fputs("\n", stdout);
            }
        } else if (strcmp(comando,"delrel")==0) {
            scanf("%s", id_orig);
            scanf("%s", id_dest);
            scanf("%s", id_rel);
            delrel(id_orig,id_dest,id_rel);
        } else if (strcmp(comando,"delent") == 0) {
            scanf("%s", id_ent);
            delent(id_ent);

        }
        scanf("%s",comando);
    }
    return 0;
}
void addent(char * id_ent) {
    puntnodoen padre = NULL;
    puntnodoen curr = testaen;
    while (curr != NULL) {
        padre = curr;
        if ((strcmp(id_ent,curr->id_ent) == 0)) {
            return;
        } else if (strcmp(id_ent,curr->id_ent) < 0) {
            curr = curr->sx;
        } else {
            curr = curr->dx;
        }
    }
    curr = (puntnodoen) malloc (sizeof(nodoen));
    curr->id_ent = (char *) malloc(sizeof(char) * strlen(id_ent) + 1);
    strcpy(curr->id_ent,id_ent);
    curr->sx = NULL;
    curr->dx = NULL;
    curr->padre = padre;
    curr->testarel = NULL;
    if (padre == NULL) {
        testaen = curr;
    } else if (strcmp(id_ent,padre->id_ent) < 0) {
        padre->sx = curr;
    } else {
        padre->dx = curr;
    }
}

void addrel(char * id_orig, char * id_dest, char *id_rel) {
    puntnodoen curr_orig = testaen;
    puntnodoen curr_dest = testaen;
    puntnodorel curr_rel = testarel;
    while ((curr_orig != NULL) && (strcmp(curr_orig->id_ent, id_orig) != 0)) {
        if (strcmp(id_orig, curr_orig->id_ent) < 0) {
            curr_orig = curr_orig->sx;
        } else {
            curr_orig = curr_orig->dx;
        }
    }
    if (curr_orig != NULL) {
        while ((curr_dest != NULL) && (strcmp(curr_dest->id_ent, id_dest) != 0)) {
            if (strcmp(id_dest, curr_dest->id_ent) < 0) {
                curr_dest = curr_dest->sx;
            } else {
                curr_dest = curr_dest->dx;
            }
        }
        if (curr_dest != NULL) {
            curr_rel = add_rel(id_rel);
            if (add_dest(curr_orig->testarel,curr_dest->id_ent,curr_rel->id_rel,curr_orig)) {
                add_orig(curr_dest->testarel,curr_orig->id_ent,curr_rel->id_rel,curr_dest);
                add_to(curr_rel,curr_dest->id_ent);
            }
        }
    }
}

void add_to(puntnodorel testarel1, char* id_dest) {
    puntnodoto padre = NULL;
    puntnodoto curr = testarel1->testato;
    while (curr != NULL) {
        padre = curr;
        if ((strcmp(id_dest,curr->id_to) == 0)) {
            curr->counter++;
            return;
        } else if (strcmp(id_dest,curr->id_to) < 0) {
            curr = curr->sx;
        } else {
            curr = curr->dx;
        }
    }
    curr = (puntnodoto) malloc (sizeof(nodoto));
    curr->id_to = id_dest;
    curr->sx = NULL;
    curr->dx = NULL;
    curr->padre = padre;
    curr->counter = 1;
    if (padre == NULL) {
        testarel1->testato = curr;
    } else if (strcmp(id_dest,padre->id_to) < 0) {
        padre->sx = curr;
    } else {
        padre->dx = curr;
    }
}

void add_orig(puntnodorelen curr,char* id_orig,char* id_rel,puntnodoen ent) {
    puntnodoorig curr_orig;
    puntnodoorig padre_orig = NULL;
    puntnodorelen padre_rel = NULL;
    while ((curr != NULL) && (strcmp(id_rel,curr->id_rel) != 0)) {
        padre_rel = curr;
        if (strcmp(id_rel,curr->id_rel) < 0) {
            curr = curr->sx;
        } else {
            curr = curr->dx;
        }
    }
    if (curr != NULL) {
        curr_orig = curr->testaorig;
        while ((curr_orig != NULL) && (strcmp(curr_orig->id_ent, id_orig) != 0)) {
            padre_orig = curr_orig;
            if (strcmp(id_orig, curr_orig->id_ent) < 0) {
                curr_orig = curr_orig->sx;
            } else {
                curr_orig = curr_orig->dx;
            }
        }
        if (curr_orig != NULL) {
            return;
        } else {
            curr_orig = (puntnodoorig) malloc (sizeof(nodo_orig));
            curr_orig->id_ent = id_orig;
            curr_orig->sx = NULL;
            curr_orig->dx = NULL;
            curr_orig->padre = padre_orig;

            if (padre_orig == NULL) {
                curr->testaorig = curr_orig;
            } else if (strcmp(id_orig,padre_orig->id_ent) < 0) {
                padre_orig->sx = curr_orig;
            } else {
                padre_orig->dx = curr_orig;
            }
            return;
        }
    } else {
        curr = (puntnodorelen) malloc (sizeof(nodorelen));
        curr->id_rel = id_rel;
        curr->sx = NULL;
        curr->dx = NULL;
        curr->padre = padre_rel;
        curr->testaorig = NULL;
        curr->testaorig = (puntnodoorig) malloc(sizeof(nodo_orig));
        curr->testaorig->id_ent = id_orig;
        curr->testaorig->padre = NULL;
        curr->testaorig->sx = NULL;
        curr->testaorig->dx = NULL;
        curr->testadest = NULL;
        if (padre_rel == NULL) {
            ent->testarel = curr;
        } else if (strcmp(id_rel,padre_rel->id_rel) < 0) {
            padre_rel->sx = curr;
        } else {
            padre_rel->dx = curr;
        }
        return;
    }
}

int add_dest(puntnodorelen curr,char* id_dest,char* id_rel,puntnodoen ent) {
    puntnododest curr_dest;
    puntnododest padre_dest = NULL;
    puntnodorelen padre_rel = NULL;
    while ((curr != NULL) && (strcmp(id_rel,curr->id_rel) != 0)) {
        padre_rel = curr;
        if (strcmp(id_rel,curr->id_rel) < 0) {
            curr = curr->sx;
        } else {
            curr = curr->dx;
        }
    }
    if (curr != NULL) {
        curr_dest = curr->testadest;
        while ((curr_dest != NULL) && (strcmp(curr_dest->id_ent, id_dest) != 0)) {
            padre_dest = curr_dest;
            if (strcmp(id_dest, curr_dest->id_ent) < 0) {
                curr_dest = curr_dest->sx;
            } else {
                curr_dest = curr_dest->dx;
            }
        }
        if (curr_dest != NULL) {
            return 0;
        } else {
            curr_dest = (puntnododest) malloc (sizeof(nodo_dest));
            curr_dest->id_ent = id_dest;
            curr_dest->sx = NULL;
            curr_dest->dx = NULL;
            curr_dest->padre = padre_dest;
            if (padre_dest == NULL) {
                curr->testadest = curr_dest;
            } else if (strcmp(id_dest,padre_dest->id_ent) < 0) {
                padre_dest->sx = curr_dest;
            } else {
                padre_dest->dx = curr_dest;
            }
            return 1;
        }
    } else {
        curr = (puntnodorelen) malloc (sizeof(nodorelen));
        curr->id_rel = id_rel;
        curr->sx = NULL;
        curr->dx = NULL;
        curr->padre = padre_rel;
        curr->testaorig = NULL;
        curr->testadest = (puntnododest) malloc(sizeof(nodo_dest));
        curr->testadest->id_ent = id_dest;
        curr->testadest->padre = NULL;
        curr->testadest->sx = NULL;
        curr->testadest->dx = NULL;
        curr->testaorig = NULL;
        if (padre_rel == NULL) {
            ent->testarel = curr;
        } else if (strcmp(id_rel,padre_rel->id_rel) < 0) {
            padre_rel->sx = curr;
        } else {
            padre_rel->dx = curr;
        }
        return 1;
    }
}

puntnodorel add_rel(char * id_rel) {
    puntnodorel padre = NULL;
    puntnodorel curr = testarel;
    while (curr != NULL) {
        padre = curr;
        if ((strcmp(id_rel,curr->id_rel) == 0)) {
            return curr;
        } else if (strcmp(id_rel,curr->id_rel) < 0) {
            curr = curr->sx;
        } else {
            curr = curr->dx;
        }
    }
    curr = (puntnodorel) malloc (sizeof(nodorel));
    curr->id_rel = (char *) malloc(sizeof(char) * strlen(id_rel) + 1);
    strcpy(curr->id_rel, id_rel);
    curr->sx = NULL;
    curr->dx = NULL;
    curr->padre = padre;
    curr->testato = NULL;
    if (padre == NULL) {
        testarel = curr;
        return testarel;
    } else if (strcmp(id_rel,padre->id_rel) < 0) {
        padre->sx = curr;
    } else {
        padre->dx = curr;
    }
    return curr;
}

void report(puntnodorel curr) {
    if (curr==NULL) return;
    report(curr->sx);
    fputs(curr->id_rel, stdout);
    fputs(" ", stdout);
    appcalc = 0;
    calcmax(curr->testato);
    sottoreport(curr->testato,appcalc);
    printf("%d; ",appcalc);
    report(curr->dx);
}

void calcmax(puntnodoto curr) {
    if (curr == NULL) return;
    calcmax(curr->sx);
    if (curr->counter > appcalc) appcalc = curr->counter;
    calcmax(curr->dx);
}

void sottoreport(puntnodoto curr, int max) {
    if (curr == NULL) return;
    sottoreport(curr->sx,max);
    if (curr->counter == max) {
        fputs(curr->id_to,stdout);
        fputs(" ",stdout);
    }
    sottoreport(curr->dx,max);
}

void delrel(char * id_orig,char* id_dest, char * id_rel) {
    puntnodoen curr_orig = testaen;
    puntnodorelen curr_rel;
    puntnododest curr_sottodest;
    while ((curr_orig != NULL) && (strcmp(curr_orig->id_ent, id_orig) != 0)) {
        if (strcmp(id_orig, curr_orig->id_ent) < 0) {
            curr_orig = curr_orig->sx;
        } else {
            curr_orig = curr_orig->dx;
        }
    }
    if (curr_orig != NULL) {
        curr_rel = curr_orig->testarel;
        while ((curr_rel != NULL) && (strcmp(id_rel,curr_rel->id_rel) != 0)) {
            if (strcmp(id_rel,curr_rel->id_rel) < 0) {
                curr_rel = curr_rel->sx;
            } else {
                curr_rel = curr_rel->dx;
            }
        }
        if (curr_rel != NULL) {
            curr_sottodest = curr_rel->testadest;
            while ((curr_sottodest != NULL) && (strcmp(curr_sottodest->id_ent, id_dest) != 0)) {
                if (strcmp(id_dest, curr_sottodest->id_ent) < 0) {
                    curr_sottodest = curr_sottodest->sx;
                } else {
                    curr_sottodest = curr_sottodest->dx;
                }
            }
            if (curr_sottodest != NULL) {
                delete_dest(curr_sottodest,curr_rel);
                if ((curr_rel->testaorig == NULL) && (curr_rel->testadest == NULL)) {
                    delete_rel_en(curr_rel,curr_orig);
                }
                search_del_orig(id_orig,id_dest,id_rel);
            }
        }
    }
}

void search_del_orig(char * id_orig,char* id_dest, char * id_rel) {
    puntnodoen curr_dest = testaen;
    puntnodorelen curr_rel;
    puntnodoorig curr_sottoorig;
    while (strcmp(curr_dest->id_ent, id_dest) != 0) {
        if (strcmp(id_dest, curr_dest->id_ent) < 0) {
            curr_dest = curr_dest->sx;
        } else {
            curr_dest = curr_dest->dx;
        }
    }
    curr_rel = curr_dest->testarel;
    while (strcmp(id_rel,curr_rel->id_rel) != 0) {
        if (strcmp(id_rel,curr_rel->id_rel) < 0) {
            curr_rel = curr_rel->sx;
        } else {
            curr_rel = curr_rel->dx;
        }
    }
    curr_sottoorig = curr_rel->testaorig;
    while (strcmp(curr_sottoorig->id_ent, id_orig) != 0) {
        if (strcmp(id_orig, curr_sottoorig->id_ent) < 0) {
            curr_sottoorig = curr_sottoorig->sx;
        } else {
            curr_sottoorig = curr_sottoorig->dx;
        }
    }
    delete_orig(curr_sottoorig,curr_rel);
    if ((curr_rel->testaorig == NULL) && (curr_rel->testadest == NULL)) {
        delete_rel_en(curr_rel,curr_dest);
    }
    decrease_to(id_dest,id_rel);
}

void delete_rel_en(puntnodorelen del, puntnodoen relazioneapp) {
    puntnodorelen y;
    if (del->sx == NULL) {
        transplant_rel(del,del->dx,relazioneapp);
    } else if (del->dx == NULL) {
        transplant_rel(del,del->sx,relazioneapp);
    } else {
        y = treemin_rel(del->dx);
        if (y->padre != del) {
            transplant_rel(y,y->dx,relazioneapp);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplant_rel(del,y,relazioneapp);
        y->sx = del->sx;
        y->sx->padre = y;
    }
}

puntnodorelen treemin_rel(puntnodorelen nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplant_rel(puntnodorelen u, puntnodorelen v,puntnodoen relazioneapp) {
    if (u->padre == NULL) {
        relazioneapp->testarel = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;
}

void decrease_to(char* id_dest,char* id_rel) {
    puntnodorel curr_rel = testarel;
    puntnodoto curr_to;
    while (strcmp(id_rel,curr_rel->id_rel) != 0) {
        if (strcmp(id_rel,curr_rel->id_rel) < 0) {
            curr_rel = curr_rel->sx;
        } else {
            curr_rel = curr_rel->dx;
        }
    }
    curr_to = curr_rel->testato;
    while (strcmp(curr_to->id_to,id_dest)!= 0) {
        if (strcmp(id_dest, curr_to->id_to) < 0) {
            curr_to = curr_to->sx;
        } else {
            curr_to = curr_to->dx;
        }
    }
    if (curr_to->counter > 1) {
        curr_to->counter --;
        return;
    } else {
        deleteto(curr_to,curr_rel);
        if (curr_rel->testato == NULL) deleterel(curr_rel);
    }
}

void deleterel(puntnodorel del) {
    puntnodorel y;
    if (del->sx == NULL) {
        transplantrel(del,del->dx);
    } else if (del->dx == NULL) {
        transplantrel(del,del->sx);
    } else {
        y = treeminrel(del->dx);
        if (y->padre != del) {
            transplantrel(y,y->dx);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplantrel(del,y);
        y->sx = del->sx;
        y->sx->padre = y;
    }

}

puntnodorel treeminrel(puntnodorel nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplantrel(puntnodorel u, puntnodorel v) {
    if (u->padre == NULL) {
        testarel = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;
}

void deleteto(puntnodoto del, puntnodorel relazioneapp) {
    puntnodoto y;
    if (del->sx == NULL) {
        transplantto(del,del->dx,relazioneapp);
    } else if (del->dx == NULL) {
        transplantto(del,del->sx,relazioneapp);
    } else {
        y = treeminto(del->dx);
        if (y->padre != del) {
            transplantto(y,y->dx,relazioneapp);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplantto(del,y,relazioneapp);
        y->sx = del->sx;
        y->sx->padre = y;
    }
}

puntnodoto treeminto(puntnodoto nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplantto(puntnodoto u, puntnodoto v,puntnodorel relazioneapp) {
    if (u->padre == NULL) {
        relazioneapp->testato = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;
}

void delete_dest(puntnododest del, puntnodorelen relazioneapp) {
    puntnododest y;
    if (del->sx == NULL) {
        transplant_dest(del,del->dx,relazioneapp);
    } else if (del->dx == NULL) {
        transplant_dest(del,del->sx,relazioneapp);
    } else {
        y = treemin_dest(del->dx);
        if (y->padre != del) {
            transplant_dest(y,y->dx,relazioneapp);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplant_dest(del,y,relazioneapp);
        y->sx = del->sx;
        y->sx->padre = y;
    }
}

puntnododest treemin_dest(puntnododest nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplant_dest(puntnododest u, puntnododest v,puntnodorelen relazioneapp) {
    if (u->padre == NULL) {
        relazioneapp->testadest = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;
}

void delete_orig(puntnodoorig del, puntnodorelen relazioneapp) {
    puntnodoorig y;
    if (del->sx == NULL) {
        transplant_orig(del,del->dx,relazioneapp);
    } else if (del->dx == NULL) {
        transplant_orig(del,del->sx,relazioneapp);
    } else {
        y = treemin_orig(del->dx);
        if (y->padre != del) {
            transplant_orig(y,y->dx,relazioneapp);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplant_orig(del,y,relazioneapp);
        y->sx = del->sx;
        y->sx->padre = y;
    }
}

puntnodoorig treemin_orig(puntnodoorig nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplant_orig(puntnodoorig u, puntnodoorig v,puntnodorelen relazioneapp) {
    if (u->padre == NULL) {
        relazioneapp->testaorig = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;

}

void delent(char* id_ent) {
    puntnodoen curr_en = testaen;
    while ((curr_en != NULL) && (strcmp(curr_en->id_ent, id_ent) != 0)) {
        if (strcmp(id_ent, curr_en->id_ent) < 0) {
            curr_en = curr_en->sx;
        } else {
            curr_en = curr_en->dx;
        }
    }
    if (curr_en != NULL) {
        visit_rel(curr_en->testarel, id_ent);
        deleteent(curr_en);
    }
}

void visit_rel(puntnodorelen curr_rel,char* id_ent) {
    if (curr_rel == NULL) return;
    visit_rel(curr_rel->sx,id_ent);
    counter = 0;
    visit_orig(curr_rel->testaorig,id_ent,curr_rel);
    if (counter > 0) decrease_to_sum(id_ent,curr_rel->id_rel);
    visit_dest(curr_rel->testadest,id_ent,curr_rel);
    visit_rel(curr_rel->dx,id_ent);
}

void visit_orig(puntnodoorig curr, char* id_ent,puntnodorelen relazione) {
    if (curr == NULL) return;
    visit_orig(curr->sx,id_ent,relazione);
    counter++;
    search_del_dest(curr->id_ent,id_ent,relazione->id_rel);
    delete_orig(curr,relazione);
    visit_orig(curr->dx,id_ent,relazione);
}

void decrease_to_sum(char* id_dest,char* id_rel) {
    puntnodorel curr_rel = testarel;
    puntnodoto curr_to;
    while (strcmp(id_rel,curr_rel->id_rel) != 0) {
        if (strcmp(id_rel,curr_rel->id_rel) < 0) {
            curr_rel = curr_rel->sx;
        } else {
            curr_rel = curr_rel->dx;
        }
    }
    curr_to = curr_rel->testato;
    while (strcmp(curr_to->id_to,id_dest)!= 0) {
        if (strcmp(id_dest, curr_to->id_to) < 0) {
            curr_to = curr_to->sx;
        } else {
            curr_to = curr_to->dx;
        }
    }
    if (curr_to->counter > counter) {
        curr_to->counter = curr_to->counter - counter;
        return;
    } else {
        deleteto(curr_to,curr_rel);
        if (curr_rel->testato == NULL) deleterel(curr_rel);
    }

}

void search_del_dest(char * id_orig,char* id_dest, char * id_rel) {
    puntnodoen curr_orig = testaen;
    puntnodorelen curr_rel;
    puntnododest curr_sottodest;
    while (strcmp(curr_orig->id_ent, id_orig) != 0) {
        if (strcmp(id_orig, curr_orig->id_ent) < 0) {
            curr_orig = curr_orig->sx;
        } else {
            curr_orig = curr_orig->dx;
        }
    }
    curr_rel = curr_orig->testarel;
    while (strcmp(id_rel,curr_rel->id_rel) != 0) {
        if (strcmp(id_rel,curr_rel->id_rel) < 0) {
            curr_rel = curr_rel->sx;
        } else {
            curr_rel = curr_rel->dx;
        }
    }
    curr_sottodest = curr_rel->testadest;
    while (strcmp(curr_sottodest->id_ent, id_dest) != 0) {
        if (strcmp(id_dest, curr_sottodest->id_ent) < 0) {
            curr_sottodest = curr_sottodest->sx;
        } else {
            curr_sottodest = curr_sottodest->dx;
        }
    }
    delete_dest(curr_sottodest,curr_rel);
    if ((curr_rel->testadest == NULL) && (curr_rel->testaorig == NULL)) {
        delete_rel_en(curr_rel,curr_orig);
    }
}

void visit_dest(puntnododest curr, char* id_ent,puntnodorelen relazione) {
    if(curr == NULL) return;
    visit_dest(curr->sx,id_ent,relazione);
    search_del_orig(id_ent,curr->id_ent,relazione->id_rel);
    delete_dest(curr,relazione);
    visit_dest(curr->dx,id_ent,relazione);

}

void deleteent(puntnodoen del) {

    puntnodoen y;

    if (del->sx == NULL) {
        transplanten(del,del->dx);
    } else if (del->dx == NULL) {
        transplanten(del,del->sx);
    } else {
        y = treeminen(del->dx);
        if (y->padre != del) {
            transplanten(y,y->dx);
            y->dx = del->dx;
            y->dx->padre = y;
        }
        transplanten(del,y);
        y->sx = del->sx;
        y->sx->padre = y;
    }
}

puntnodoen treeminen(puntnodoen nodo) {
    while (nodo->sx != NULL) {
        nodo = nodo->sx;
    }
    return nodo;
}

void transplanten(puntnodoen u, puntnodoen v) {
    if (u->padre == NULL) {
        testaen = v;
    } else if (u==u->padre->sx) {
        u->padre->sx = v;
    } else u->padre->dx = v;
    if (v!=NULL) v->padre=u->padre;
}
