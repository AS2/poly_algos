#ifndef _XORLIST_H__
#define _XORLIST_H__

#define MAX_STR 20

typedef struct list list_t;
struct list {
  int xorAddress;
  char str[MAX_STR];
};

typedef struct xorList xorList_t;
struct xorList {
  list_t* first;
  list_t* second;
};

int AddElementToList( xorList_t *list, char* newElement );
int FindElementInListByKey(xorList_t* list, char* key);
int DeleteElementFromListByAddress(xorList_t* list, int address);
int DeleteElementFromListByKey(xorList_t* list, char* key);
int IterationList(xorList_t* list);

int clearList(xorList_t*list);

#endif _XORLIST_H__