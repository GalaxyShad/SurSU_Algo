#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct Node* pNode;
typedef void (*pFunction)(pNode);
typedef int Item;

struct Node {
    pNode Left;
    pNode Right;
    Item Data;
    int Size;
};

//---------------------Создание узла----------------------------------------
pNode NewNode(Item D) {
    pNode pN = malloc(sizeof(struct Node));

    pN->Left = 0;
    pN->Right = 0;
    pN->Data = D;
    pN->Size = 0;

    return pN;
}

int GetSize(pNode root) {
	if (!root) return 0; 
	return root->Size; 
}

void FixSize(pNode root) {
	root->Size = GetSize(root->Left)+GetSize(root->Right)+1; 
}

//---------------------Прямой обход-----------------------------------------
void VisitPre(pNode root, pFunction Function) {
    if (!root) return;

    Function(root);
    VisitPre(root->Left, Function);
    VisitPre(root->Right, Function);
}

//---------------------Польская запись-----------------------------------------
void PrintPoly(pNode root) {
    if (!root) {
        printf("_");
        return;
    }

    printf("(%d", root->Data);
    printf(" ");
    PrintPoly(root->Left);
    printf(" ");
    PrintPoly(root->Right);
    printf(") ");
}

//---------------------Симметричный обход---------------------------------------
void VisitSim(pNode root, pFunction Function) {
    if (!root) return;
    
    VisitSim(root->Left, Function);
    Function(root);
    VisitSim(root->Right, Function);
}


//---------------------Обратный обход---------------------------------------
void VisitPost(pNode root, pFunction Function) {
    if (!root) return;
    
    VisitPost(root->Left, Function);
    VisitPost(root->Right, Function);
    Function(root);
}

//---------------------Вывод на экран---------------------------------------
void Print(pNode root) {
    if (!root) return;
    printf("%d ", root->Data);
}

//---------------------Вставка узла-----------------------------------------
void Insert(pNode *proot, Item D) {
#define root (*proot)
    if (!root)
        root = NewNode(D);
    else if (D < root->Data)
        Insert(&(root->Left), D);
    else
        Insert(&(root->Right), D);

    FixSize(root);
#undef root
}


//---------------------Поиск узла-------------------------------------------
pNode Find(pNode root, pNode *parent, int *LR, Item D) {
    *parent = 0;
    *LR = 0;
    while (root) {
        if (D < root->Data) {
            *parent = root;
            *LR = 0;
            root = root->Left;
        } else if (D > root->Data) {
            *parent = root;
            *LR = 1;
            root = root->Right;
        } else 
            return root;
    }
    return 0;
}

//---------------------Поиск правого минимального---------------------------
pNode FindMinRight(pNode root, pNode *parent) {
    while (root->Left) {
        *parent = root;
        root = root->Left;
    }
    return root;
}

//---------------------Удаление одного узла---------------------------------
void Delete(pNode root) {
    if (root) free(root);
}

//---------------------Удаление узла с заданным ключом----------------------
void DeleteNode(pNode *ROOT, Item D) {
    int LR = 0;
    pNode parent = 0, root = Find(*ROOT, &parent, &LR, D);
    if (!root) return;
    
    if (root->Left && root->Right) {
        pNode MinRightParent = root;
        pNode MinRight = FindMinRight(root->Right, &MinRightParent);
        MinRight->Left = root->Left;
        MinRightParent->Left = MinRight->Right;
        MinRight->Right = root->Right;
        if (parent) {
            if (LR) parent->Right = MinRight;
            else    parent->Left = MinRight;
        } else
            *ROOT = MinRight;
    } else if (parent)
        if (LR) parent->Right = root->Right ? root->Right : root->Left;
        else    parent->Left  = root->Right ? root->Right : root->Left;
    else
        *ROOT = root->Right ? root->Right : root->Left;
    Delete(root);
    
}

//---------------------------Поворот вправо---------------------------------
void RotateRight(pNode *pLev0) {
#define Lev0 (*pLev0)
    pNode Lev1 = Lev0->Left;
    Lev0->Left = Lev1->Right;
    Lev1->Right = Lev0;
    Lev0 = Lev1;
#undef Lev0
}

//---------------------------Поворот влево----------------------------------
void RotateLeft(pNode *pLev0) {
#define Lev0 (*pLev0)
    pNode Lev1 = Lev0->Right;
    Lev0->Right = Lev1->Left;
    Lev1->Left = Lev0;
    Lev0 = Lev1;
#undef Lev0
}

//-------------------------Вставка в корень---------------------------------
void InsertRoot(pNode *proot, Item D) {
#define root (*proot)
    if (!root)
        root = NewNode(D);
    else if (D < root->Data) {
        InsertRoot(&(root->Left), D);
        RotateRight(&(root));
    } else {
        InsertRoot(&(root->Right), D);
        RotateLeft(&(root));
    }
#undef root
}

//---------------------Вставка узла-----------------------------------------
void InsertRandom(pNode *proot, Item D) {
#define root (*proot)
    if (!root)
        root = NewNode(D);
    else if (rand() % (root->Size+1) == 0)
        InsertRoot(proot, D);
    else if (D < root->Data)
        Insert(&(root->Left), D);
    else
        Insert(&(root->Right), D);
    
    FixSize(root);
#undef root
}

int max(int a, int b) {
    return (a > b) ? a : b; 
}

//---------------------Высота дерева-----------------------------------------
int Height(pNode root) {
    return (root == NULL) ? 0 : 1 + max(Height(root->Left), Height(root->Right));
}







//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void TestTrees() {
    pNode bTreeRoot = NewNode(4);

    Insert(&bTreeRoot, 6);
    Insert(&bTreeRoot, 3);
    Insert(&bTreeRoot, 5);
    Insert(&bTreeRoot, 2);
    Insert(&bTreeRoot, 7);
    Insert(&bTreeRoot, 9);
    Insert(&bTreeRoot, 10);

    VisitPre(bTreeRoot, Print);
    printf("\n");
    VisitSim(bTreeRoot, Print);
    printf("\n");
    VisitPost(bTreeRoot, Print);
    printf("\n");

    PrintPoly(bTreeRoot);
    printf("\n");

    printf("%d %d\n", GetSize(bTreeRoot), Height(bTreeRoot));
    
    //////////////////////////

    bTreeRoot = NewNode(4);

    InsertRandom(&bTreeRoot, 6);
    InsertRandom(&bTreeRoot, 3);
    InsertRandom(&bTreeRoot, 5);
    InsertRandom(&bTreeRoot, 2);
    InsertRandom(&bTreeRoot, 7);
    InsertRandom(&bTreeRoot, 9);
    InsertRandom(&bTreeRoot, 10);

    VisitPre(bTreeRoot, Print);
    printf("\n");
    VisitSim(bTreeRoot, Print);
    printf("\n");
    VisitPost(bTreeRoot, Print);
    printf("\n");

    PrintPoly(bTreeRoot);
    printf("\n");

    printf("%d %d", GetSize(bTreeRoot), Height(bTreeRoot));
}


void DoMeasurements() {
    const int maxSize = 500;

    pNode bTreeSerial = NULL;
    pNode bTreeRandom = NULL;
    for (int i = 0; i < maxSize; i++) {
        Insert(&bTreeSerial, i);
        InsertRandom(&bTreeRandom, i);

        printf("%d;%d;%d\n", i, Height(bTreeSerial), Height(bTreeRandom));
    }

    printf("\n\nRandom Keys\n");

    bTreeSerial = NULL;
    bTreeRandom = NULL;
    for (int i = 0; i < maxSize; i++) {
        Insert(&bTreeSerial, rand() % maxSize);
        InsertRandom(&bTreeRandom, rand() % maxSize);

        printf("%d;%d;%d\n", i, Height(bTreeSerial), Height(bTreeRandom));
    }

    printf("\n");
}

int CountNotOddNumbers(pNode root) {
    int count = 0;

    if (!root) return 0;
    if (root->Left && root->Right && root->Data % 2 != 0) count++;
        
    return CountNotOddNumbers(root->Left) + count + CountNotOddNumbers(root->Right);
}

void TestNotOdd() {
    srand(time(0));

    pNode bTreeRoot = NULL;

    // for (int i = 0; i < 1001; i++) InsertRandom(&bTreeRoot, i);
    for (int i = 0; i < 1001; i++) Insert(&bTreeRoot, rand() % 1001);

    PrintPoly(bTreeRoot);
    printf("\n");
    printf("%d %d", CountNotOddNumbers(bTreeRoot), Height(bTreeRoot));

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

int main() {
    system("chcp 1251");
    // TestTrees();
    // DoMeasurements();
    TestNotOdd();
    return 0;
}