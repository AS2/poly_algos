#include <stdlib.h>
#include <string.h>

#include "XORlist.h"

#pragma warning(disable: 4996)

int AddElementToList(xorList_t* list, char *newElement) {
  list_t* tmp1, *tmp2, *tmp3, 
    *newEl;

  if (strlen(newElement) >= MAX_STR)
    return 0;

  if (list->first == NULL) {
    if (!(newEl = (list_t*)malloc(sizeof(list_t))))
      return 0;
    
    strncpy(newEl->str, newElement, MAX_STR);
    newEl->xorAddress = 0;

    list->first = newEl;
    return 1;
  }
  else if (list->second == NULL) {
    if (!(newEl = (list_t*)malloc(sizeof(list_t))))
      return 0;

    strncpy(newEl->str, newElement, MAX_STR);
    newEl->xorAddress = 0;

    list->second = newEl;
    return 1;
  }
  else if (list->first != NULL && list->second != NULL && (list->first->xorAddress == 0 && list->second->xorAddress == 0)) {
    if (!(newEl = (list_t*)malloc(sizeof(list_t))))
      return 0;

    strncpy(newEl->str, newElement, MAX_STR);
    list->first->xorAddress = ((int)newEl ^ (int)list->second);
    list->second->xorAddress = ((int)list->first ^ (int)newEl);
    newEl->xorAddress = ((int)list->second ^ (int)list->first);
    return 1;
  }
  else {
    if (!(newEl = (list_t*)malloc(sizeof(list_t))))
      return 0;

    strncpy(newEl->str, newElement, MAX_STR);

    tmp1 = list->first;
    tmp2 = tmp3 = list->second;
    while ((list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress)) != list->first) {
      tmp3 = (list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress));
      tmp1 = tmp2;
      tmp2 = tmp3;
    }
    tmp3->xorAddress = ((int)tmp1 ^ (int)newEl);
    newEl->xorAddress = ((int)tmp3 ^ (int)list->first);
    list->first->xorAddress = ((int)newEl ^ (int)list->second);

    return 1;
  }
}

int FindElementInListByKey(xorList_t* list, char* key) {
  list_t* tmp1, * tmp2, * tmp3;

  if (strlen(key) >= MAX_STR)
    return 0;

  if (list->first == NULL)
    return 0;
  else if (list->second == NULL) {
    if (!strcmp(list->first->str, key))
      return 1;
    else
      return 0;
  }
  else {
    if (!strcmp(list->first->str, key))
      return 1;
    else if (!strcmp(list->second->str, key))
      return 1;
  
    tmp1 = list->first;
    tmp2 = tmp3 = list->second;
    while ((list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress)) != list->first) {
      tmp3 = (list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress));
      tmp1 = tmp2;
      tmp2 = tmp3;
      if (!strcmp(tmp3->str, key))
        return 1;
    }
    return 0;
  }
}

int DeleteElementFromListByKey(xorList_t* list, char* key) {
  list_t* tmp1, * tmp2, * tmp3, *tmp4;

  if (strlen(key) >= MAX_STR)
    return 0;

  // если 0 элементов
  if (list->first == NULL)
    return 0;
  // если 1 элемент
  else if (list->second == NULL) {
    if (!strcmp(list->first->str, key)) {
      free(list->first);
      list->first = NULL;
      return 1;
    }
    else
      return 0;
  }
  // если 2 элемент
  else if (list->first != NULL && list->second != NULL && (list->first->xorAddress == 0 && list->second->xorAddress == 0)) {
    if (!strcmp(list->first->str, key)) {
      free(list->first);
      list->first = list->second;
      list->second = NULL;
      return 1;
    }
    else if (!strcmp(list->second->str, key)) {
      free(list->second);
      list->second = NULL;
      return 1;
    }
    else
      return 0;
  }
  // если 3 и более элемента
  else {
    tmp1 = list->first;
    tmp2 = list->second;
    tmp3 = (list_t*)((int)tmp1 ^ tmp2->xorAddress);

    // если 3 элемента
    if ((list_t*)((int)tmp2 ^ tmp3->xorAddress) == list->first) {
      if (!strcmp(list->first->str, key)) {
        list->second->xorAddress = tmp3->xorAddress = 0;
        free(list->first);
        list->first = list->second;
        list->second = tmp3;
        return 1;
      }
      else if (!strcmp(list->second->str, key)) {
        list->first->xorAddress = tmp3->xorAddress = 0;
        free(list->second);
        list->second = tmp3;
        return 1;
      }
      if (!strcmp(tmp3->str, key)) {
        list->first->xorAddress = list->second->xorAddress = 0;
        free(tmp3);
        return 1;
      }
      else
        return 0;
    }

    // если больше 3х элементов
    if (!strcmp(list->first->str, key)) {
      tmp1 = (list_t*)(list->first->xorAddress ^ (int)list->second);
      tmp1->xorAddress = (tmp1->xorAddress ^ (int)list->first) ^ (int)list->second;
      list->second->xorAddress = (int)tmp3 ^ (int)tmp1;
      free(list->first);
      list->first = list->second;
      list->second = tmp3;
      return 1;
    }
    else if (!strcmp(list->second->str, key)) {
      tmp1 = (list_t *)(list->first->xorAddress ^ (int)list->second);
      tmp2 = (list_t *)((int)list->second ^ tmp3->xorAddress);
      list->first->xorAddress = (int)tmp1 ^ (int)tmp3;
      tmp3->xorAddress = (int)list->first ^ (int)tmp2;
      free(list->second);
      list->second = tmp3;
      return 1;
    }

    while ((list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress)) != list->first) {
      tmp3 = (list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress));
      if (!strcmp(tmp3->str, key)) {
        tmp4 = (list_t *)((int)tmp2 ^ (int)(tmp3->xorAddress));
        tmp2->xorAddress = (int)tmp1 ^ (int)(tmp4);
        tmp4->xorAddress = (int)tmp2 ^ ((int)tmp3 ^ (int)(tmp4->xorAddress));
        free(tmp3);
        return 1;
      }
      tmp1 = tmp2;
      tmp2 = tmp3;
    }
    return 0;
  }
}

int DeleteElementFromListByAddress(xorList_t* list, int address) {
  list_t* tmp1, * tmp2, * tmp3, *tmp4;

  // если у нас 0-2 элементов, то XOR список не сможет образоваться
  if (list->first == NULL || list->second == NULL)
    return 0;
  else if (list->first->xorAddress == 0 && list->second->xorAddress == 0)
      return 0;
  else {
    tmp1 = list->first;
    tmp2 = list->second;
    tmp3 = (list_t*)((int)tmp1 ^ tmp2->xorAddress);

    // если 3 элемента
    if ((list_t *)((int)tmp2 ^ tmp3->xorAddress) == list->first) {
      if (list->first->xorAddress == address) {
        list->second->xorAddress = tmp3->xorAddress = 0;
        free(list->first);
        list->first = list->second;
        list->second = tmp3;
        return 1;
      }
      else if (list->second->xorAddress == address) {
        list->first->xorAddress = tmp3->xorAddress = 0;
        free(list->second);
        list->second = tmp3;
        return 1;
      }
      else if (tmp3->xorAddress == address) {
        list->first->xorAddress = list->second->xorAddress = 0;
        free(tmp3);
        return 1;
      }
      else
        return 0;
    }

    // если больше трех
    if (list->first->xorAddress == address) {
      tmp1 = (list_t*)(list->first->xorAddress ^ (int)list->second);
      tmp1->xorAddress = (tmp1->xorAddress ^ (int)list->first) ^ (int)list->second;
      list->second->xorAddress = (int)tmp3 ^ (int)tmp1;
      free(list->first);
      list->first = list->second;
      list->second = tmp3;
      return 1;
    }
    else if (list->second->xorAddress == address) {
      tmp1 = (list_t*)(list->first->xorAddress ^ (int)list->second);
      tmp2 = (list_t*)((int)list->second ^ tmp3->xorAddress);
      list->first->xorAddress = (int)tmp1 ^ (int)tmp3;
      tmp3->xorAddress = (int)list->first ^ (int)tmp2;
      free(list->second);
      list->second = tmp3;
      return 1;
    }

    while ((list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress)) != list->first) {
      tmp3 = (list_t*)((int)tmp1 ^ (int)(tmp2->xorAddress));
      if (tmp3->xorAddress == address) {
        tmp4 = (list_t*)((int)tmp2 ^ (int)(tmp3->xorAddress));
        tmp2->xorAddress = (int)tmp1 ^ (int)(tmp4);
        tmp4->xorAddress = (int)tmp2 ^ ((int)tmp3 ^ (int)(tmp4->xorAddress));
        free(tmp3);
        return 1;
      }
      tmp1 = tmp2;
      tmp2 = tmp3;
    }
    return 0;
  }
}

int IterationList(xorList_t* list) {
  list_t* tmp;

  if (list->first == NULL || list->second == NULL || (list->first->xorAddress == 0 && list->second->xorAddress == 0))
    return 0;
  else {
    tmp = (list_t *)((int)list->first ^ list->second->xorAddress);
    list->first = list->second;
    list->second = tmp;
    return 1;
  }
}

int clearList(xorList_t* list) {
  list_t* tmp1, * tmp2;

  if (list->first == NULL)
    return 1;
  else if (list->second == NULL) {
    free(list->first);
    list->first = NULL;
    return 1;
  }
  else if (list->first != NULL && list->second != NULL && (list->first->xorAddress == 0 && list->second->xorAddress == 0)) {
    free(list->first);
    free(list->second);
    list->first = list->second = NULL;
    return 1;
  }
  else {
    tmp1 = (list_t*)((int)list->first ^ list->second->xorAddress);
    tmp2 = (list_t*)((int)list->second ^ tmp1->xorAddress);
   
    while (tmp2 != list->first) {
      list->second->xorAddress = (int)list->first ^ (int)tmp2;
      tmp2->xorAddress = (int)list->second ^ ((int)tmp1 ^ tmp2->xorAddress);
      free(tmp1);

      tmp1 = (list_t*)((int)list->first ^ list->second->xorAddress);
      tmp2 = (list_t*)((int)list->second ^ tmp1->xorAddress);
    }
    free(tmp1);
    free(list->second);
    free(list->first);
    list->first = list->second = NULL;
    return 1;
  }
}
