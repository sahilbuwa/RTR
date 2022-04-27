#include<stdio.h>
#include<stdlib.h>

struct Node
{
    float data;
    struct Node *next;
    struct Node *prev;
};

typedef struct Node node_t;

// Function Declarations
node_t* CreateLinkedList();
void Display(node_t*);
void InsertAtPos(node_t*,float,int);
void DeleteAtPos(node_t*,int);
void Search(node_t*,float);
void InsertAtFirst(node_t*,float);
void InsertAtLast(node_t*,float);
void FreeList(node_t*);

int main(void)
{
    node_t* p_head;
    printf("\n**********************************************************************************\nHead of List is Created\n");
    p_head=CreateLinkedList();
    Display(p_head);

    printf("\n**********************************************************************************\nInserting Elements in List\n\n");
    InsertAtFirst(p_head,1.1f);
    Display(p_head);

    InsertAtFirst(p_head,2.2f);
    Display(p_head);

    InsertAtFirst(p_head,3.3f);
    Display(p_head);

    InsertAtLast(p_head,4.4f);
    Display(p_head);

    InsertAtPos(p_head,5.5f,1);
    Display(p_head);

    InsertAtPos(p_head,6.6f,5);
    Display(p_head);

    InsertAtPos(p_head,7.7f,10);
    Display(p_head);

    InsertAtPos(p_head,7.7f,7);
    Display(p_head);

    printf("\n**********************************************************************************\nSearching of Elements from List\n\n");
    Search(p_head,5.5f);
    Display(p_head);

    Search(p_head,25.3f);
    Display(p_head);

    printf("\n**********************************************************************************\nDeletion of Elements of List\n\n");
    DeleteAtPos(p_head, 1);
    Display(p_head);

    DeleteAtPos(p_head, 3);
    Display(p_head);

    DeleteAtPos(p_head, 14);
    Display(p_head);

    DeleteAtPos(p_head, 4);
    Display(p_head);

    printf("\n**********************************************************************************\nFinal List\n\n");
    Display(p_head);

    FreeList(p_head);
    Display(p_head);

    return(0);
}

node_t* CreateLinkedList()
{
    node_t * p_head = malloc(sizeof(struct Node));
    if(p_head==NULL)
    {
        puts("Memory not allocated.\n");
        exit(0);
    }
    p_head->data='\0';
    p_head->next=NULL;
    p_head->prev=NULL;
    return p_head;
}

void Display(node_t* p_head)
{
    struct Node *p_traverse;

    if(p_head->next!=NULL)
    {
        p_traverse=p_head->next;
        
        while(p_traverse!=NULL)
        {
            
            printf("|%f|->",p_traverse->data);
            p_traverse=p_traverse->next;
        }
        printf("\n----------------------------------------------------\n");

    }
    else
    {
        printf("List is empty.\n");
    }
}

void InsertAtFirst(node_t* p_head,float data)
{
    node_t* p_newnode;
    p_newnode=malloc(sizeof(struct Node));
    if(p_newnode==NULL)
    {
        puts("Memory not allocated.\n");
        exit(0);
    }
    if(p_head->next==NULL)
    {
        p_newnode->next=NULL;
        p_newnode->data=data;
        p_newnode->prev=p_head;
        p_head->next=p_newnode;
    }
    else
    {
        p_head->next->prev=p_newnode;
        p_newnode->next=p_head->next;
        p_newnode->data=data;
        p_newnode->prev=p_head;
        p_head->next = p_newnode;
    }
}

void InsertAtLast(node_t* p_head,float data)
{
    node_t* p_newnode;
    p_newnode=malloc(sizeof(struct Node));
    if(p_newnode==NULL)
    {
        puts("Memory not allocated.\n");
        exit(0);
    }
    if(p_head->next==NULL)
    {
        p_newnode->next=NULL;
        p_newnode->data=data;
        p_newnode->prev=p_head;
        p_head->next=p_newnode;
    }
    else
    {
        node_t* p_traverse;
        p_traverse=p_head->next;
        while((p_traverse->next)!=NULL)
        {
            p_traverse=p_traverse->next;
        }
        if((p_traverse->next)==NULL)
        {
            p_newnode->next=NULL;
            p_newnode->data=data;
            p_newnode->prev=p_traverse;
            p_traverse->next=p_newnode;
        }
    }
}

void InsertAtPos(node_t* p_head,float data,int pos)
{
    int npos=pos;
    node_t* p_newnode;
    p_newnode=malloc(sizeof(struct Node));
    if(p_newnode==NULL)
    {
        puts("Memory not allocated.\n");
        exit(0);
    }
    if((p_head->next==NULL) && (pos==1) ) // First thikani add karnyasathi
    {
        p_newnode->next=p_head;
        p_newnode->data=data;
        p_newnode->prev=p_head;
        p_head->next=p_newnode;
        return;
    }

    node_t* p_traverse;
    p_traverse=p_head;
    while(((p_traverse->next)!=NULL) && (pos>1) )
    {
        p_traverse=p_traverse->next;
        pos--;
    }
    if(pos==1)
    {
        if(p_traverse->next!=NULL) // 1st ani last sodun saglyansathi
        {
            p_newnode->next=p_traverse->next;
            p_traverse->next->prev=p_newnode;
            p_newnode->data=data;
            p_newnode->prev=p_traverse;
            p_traverse->next=p_newnode;
        }
        else // last index  la add kela tr
        {
            p_traverse->next=p_newnode;
            p_newnode->data=data;
            p_newnode->next=NULL;
            p_newnode->prev=p_traverse;
        }
    }
    else
    {
        printf("\n----------------------------------------------------\n");
        printf("\nList does not have %dth position.\n",npos);
        printf("\n----------------------------------------------------\n");
    }
}

void DeleteAtPos(node_t* p_head, int pos)
{
    int npos = pos,trav_pos=1;
    node_t* p_traverse = NULL;
    node_t* p_temp = NULL;
    if(pos==1) // 1st node delete karaichi ahe.
    {
        if(p_head->next->next==NULL) // List madhe 1ch element ahe.
        {
            p_temp=p_head->next;
            p_head->next=NULL;
            free(p_temp);
            p_temp->next=NULL;
            p_temp->prev=NULL;
            printf("\n----------------------------------------------------\n");
            printf("\nNode %d deleted successfully\n", npos);
            printf("\n----------------------------------------------------\n");
        }
        else // List madhe 1 peksha jast element ahet.
        {
            p_temp = p_head->next;
            p_temp->next->prev=p_head;
            p_head->next = p_temp->next;
            free(p_temp);
            p_temp->next = NULL;
            p_temp->prev = NULL;
            printf("\n----------------------------------------------------\n");
            printf("\nNode %d deleted successfully\n", npos);
            printf("\n----------------------------------------------------\n");
        }
    }
    else // 1st node sodun bakichyansathi.
    {
        p_traverse=p_head->next;
        while ((trav_pos<(pos-1)) && (p_traverse->next!= NULL))
        {
            p_traverse = p_traverse->next;
            trav_pos++;
        }
        if (trav_pos==(pos-1))
        {
            p_temp = p_traverse->next;
            if(p_temp->next!=NULL) // first ani last sodun saglya nodes sathi.
            {
                p_temp->next->prev=p_traverse;
                p_traverse->next = p_temp->next;
                free(p_temp);
                p_temp->next = NULL;
                p_temp->prev = NULL;
            }
            else // Last node sathi.
            {
                p_traverse->next=NULL;
                p_temp->next=NULL;
                p_temp->prev=NULL;
                free(p_temp);
            }
            printf("\n----------------------------------------------------\n");
            printf("\nNode %d deleted successfully\n", npos);
            printf("\n----------------------------------------------------\n");
        }
        else // position sapdli nai tr
        {
            printf("\n----------------------------------------------------\n");
            printf("%d position is Invalid !!! Deletion Unsuccessfull.\n", npos);
            printf("\n----------------------------------------------------\n");
        }
    }
}

void Search(node_t* p_head,float data)
{
    struct Node *p_traverse;
    int pos=1;

    if(p_head->next!=NULL)
    {
        p_traverse=p_head->next;
        
        while((p_traverse->data!=data) && (p_traverse->next != NULL))
        {
            p_traverse=p_traverse->next;
            pos++;
        }
        if(p_traverse->data==data)
        {
            printf("%f is found at position %d.",data,pos);
        }
        else
        {
            printf("Given data not found.\n");
        }
        printf("\n----------------------------------------------------\n");

    }
    else
    {
        printf("List is empty.\n");
        
    }
}


void FreeList(node_t* p_head)
{
    node_t* p_run = NULL;
    node_t* p_temp = NULL;
    p_run = p_head;
    while (p_run != NULL)
    {
        p_temp = p_run->next;
        p_run->next = NULL;
        p_run->prev = NULL;
        free(p_run);
        p_run = p_temp;
    }
    printf("\nList Destroyed!!\n\n");
}
