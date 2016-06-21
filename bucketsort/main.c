#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int min=0,max=0;

//lalalaaa

typedef struct node
{
    int key;
    struct node *next;
} NodeT;

typedef struct
{
    int count;
    NodeT *first;
    NodeT *last;
} ListT;

ListT* createList()
{
    ListT *listPtr=(ListT*) malloc(sizeof(ListT));
    if (listPtr)
    {
        listPtr->count=0;
        listPtr->first=listPtr->last=NULL;
    }
    return listPtr;
}

NodeT* createNode (int key)
{
    NodeT *nodePtr=(NodeT*) malloc (sizeof(NodeT));
    if (nodePtr)
    {
        nodePtr->key=key;
        nodePtr->next=NULL;
    }
    return nodePtr;
}

int enqueue (ListT *listPtr, NodeT *nodePtr)
{
    if (listPtr)
    {
        if (listPtr->count==0)
            listPtr->first=listPtr->last=nodePtr;
        else
            listPtr->last->next=nodePtr;
        listPtr->count++;
        return 1;
    }
    return 0;
}

NodeT* dequeue (ListT *listPtr)
{
    NodeT *nodePtr;
    if (listPtr->first!=NULL)
    {
        nodePtr=listPtr->first;
        listPtr->first=listPtr->first->next;
        listPtr->count--;
        if (listPtr->first==NULL)
            listPtr->last=NULL;
        return nodePtr;
    }
    return NULL;
}

void kiiras(ListT *listPtr) //kiirja minden lista elemeit, nem fontos, csak teszteleshez irtam
{
    NodeT *nodePtr;
    if (listPtr!=NULL)
    {
        nodePtr=listPtr->first;
        while (nodePtr!=NULL)
        {
            printf("%d ",nodePtr->key);
            nodePtr=nodePtr->next;
        }
    }
    return;
}

int beolvasas(FILE *f, int numbers, int a[50])
{
    f=fopen("input.txt","r");
    int i=0;
    while (fscanf(f,"%d",&a[i])!=EOF)
    {
        i++;
        numbers++;
    }
    for (i=0; i<numbers; i++)
        printf("a[%d]= %d\n",i,a[i]);
    return numbers;
}

void minmax (int a[50],int numbers, int *min, int *max)//megkeresi a min s max elemet
{
    *min=INT_MAX;
    *max=INT_MIN;
    int i;
    for (i=0; i<numbers; i++)
    {
        if (a[i]>*max)
        {
            *max=a[i];
        }
        if (a[i]<*min)
        {
            *min=a[i];
        }
    }
    return;
}

int** allocation(int nrofbuckets, ListT **buckets)//annyi bucketot allocal amennyit kell
{
    int i;
    buckets=(ListT**) malloc (sizeof(ListT*)*nrofbuckets);
    if (buckets==NULL)
        printf("NULL\n");
    for (i=0; i<nrofbuckets; i++)
        buckets[i]=NULL;
    return buckets;
}

void bucketsort (int a[50],int nrofbuckets, ListT **buckets, int numbers)
{
    int i,index;
    NodeT *nodePtr;
    for (i=0; i<numbers; i++)//van 12 intervallumunk (99+13=112, 112/10+1=12, a 0 indexu lesz a [-20,10) intervallum
        //es igy tovabb (gondolom, ertheto)
    {
        if (a[i]<0)
        {
            index=(a[i]/10)+1;//ha a[i] negativ, 1-et kell hozzaadni h megkapd a megfelelo indexet
    //pl -13/10=-1, de az indexe 0 kell legyen (mert ez a 0.-ik bucket)
        }
        else
        {

            index=(a[i]/10)+2;// 22/10=2, de a 4.-ik bucketban kell legyen, ezert ezekhez 2-t kell hozzaadni
        }
        if (buckets[index]==NULL)
        {
            buckets[index]=createList(); //minden bucket null, szoval csak akkor csinalunk listat, ha szuksegunk van
            //egyaltalan arra a bucketra, es ha az addig meg nem letezett
        }
        nodePtr=createNode(a[i]);
        enqueue(buckets[index],nodePtr);
    }
    for (i=0; i<nrofbuckets; i++)
    {
        printf("bucket nr %d: ",i);
        kiiras(buckets[i]);
        printf("\n");
    }
    return;
}

void toArray (int a[50],ListT **buckets, int nrofbuckets, int numbers) //attesszuk a bucketok elemeit egy sorozatba
{
    int i,k=0;
    for (i=0; i<nrofbuckets;i++)
    {
        if (buckets[i]!=NULL)
        {
            if (buckets[i]->count!=1)//ha tobb mint egy eleme van, akkor betesszuk az elemet az a[] sorozatba, kitoroljuk
                //az elso elemet a listatol, majd azt is betesszuk, amig ki nem urul a lista
            {
                while (buckets[i]->count!=0)
                {
                    a[k]=buckets[i]->first->key;
                    if (dequeue(buckets[i])==NULL)
                        printf("NULL\n");
                    k++;
                }
            }
            else
            {
                a[k]=buckets[i]->first->key;
                k++;
            }
        }
    }
    printf("\nnew array after putting the elements back to a[]:\n");
    for (i=0; i<numbers; i++)
        printf("%d ",a[i]);
    printf("\n");
}

void insertionSort (int a[50],int numbers)
{
    int index=0,i,insert=0;
    for (i=0; i<numbers; i++)
    {
        insert=a[i];
        index=i;
        //elindulsz az elso elementtol, es megnezed h az element nagyobb-e az elotte levonel
        //ha igen, kicsereled az ertekeit es mesz visszafele es lecsekkolod minden elementparra
        //de gondolom ezt tudjatok
        while (index>0 && a[index-1]>insert)
        {
            int c=a[index-1];
            a[index-1]=a[index];
            a[index]=c;
            index--;
        }
    }
    printf("new array after insertion sort:\n");
    for (i=0; i<numbers; i++)
    {
        printf("%d ",a[i]);
    }
}

int main()
{
    FILE *f=NULL;
    int numbers=0,i;
    int a[50];
    numbers=beolvasas(f,numbers,a);
    minmax(a,numbers,&min,&max);
    int nrofbuckets=(max-min)/10+1; // a bucketok 10 szamot tartalmazhatnak, igy felosztottam oket 10-es csoportokra.
    printf("nr of buckets= %d\n",nrofbuckets);
    ListT **buckets;
    ListT *listPtr;
    listPtr=(ListT*) malloc (sizeof(ListT)*nrofbuckets);
    buckets=allocation(nrofbuckets,buckets);
    bucketsort(a,nrofbuckets,buckets,numbers);
    toArray(a,buckets,nrofbuckets,numbers);
    insertionSort(a,numbers);
    return 0;
}
