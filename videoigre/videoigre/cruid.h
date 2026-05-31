#ifndef CRUID_H
#define CRUID_H

#define FILE_NAME "igre.txt"

typedef struct {
    int id;
    char name[50];
    char genre[50];
    int year;
    int price;
} Igra;

int usporediPoId(const void* a, const void* b);
int usporediPoGodini(const void* a, const void* b);
int usporediPoCijeni(const void* a, const void* b);
void stvoriIgru();
void citajIgru();

extern Igra* popisIgara;
extern int brojIgara;

#endif
