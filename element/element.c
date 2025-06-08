#include "element.h"
#include <stdbool.h>
/*
   [Element function]
*/
Elements *New_Elements(int label)
{
    Elements *pObj;
    pObj = (Elements *)malloc(sizeof(Elements));
    pObj->inter_len = 0;
    pObj->dele = false;
    // setting object member
    pObj->label = label;
    pObj->pDerivedObj = NULL;
    pObj->Update = NULL;
    pObj->Interact = NULL;
    pObj->Draw = NULL;
    pObj->Destroy = NULL;
    return pObj;
}
