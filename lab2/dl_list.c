#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    char text[1000];
}Node;

int size = 0;
Node* head = NULL;

void add(Node* new)
{
    if(head == NULL)
    {
        head = new;
        new->next = head;
        new->prev = head;
        size += 1;
        return;
    }
    Node* temp = head;
    while(temp->next != head)
        temp = temp->next;
    temp->next = new;
    new->next = head;
    new->prev = temp;
    head->prev = new;
    size += 1;
    return;
}

int removeItem(int index)
{
    if(index < 1)
        {
            printf("Index out of bounds on remove\n");
            return 0;
        }
    if(head == NULL || size == 0)
    {
        printf("Cannot Remove from empty List\n");
        return 0;
    }
    Node* temp = head; 

    for(int i = 1; i < index; i++)
    {
        if(temp->next == head)
        {
            printf("Index out of bounds on remove\n");
            return 0;
        }
        temp = temp->next;
    }
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next; 
    if(temp == head)
        head = temp->next;
    if(size == 1)
        head = NULL;
    temp->next = NULL;
    temp->prev = NULL;
    
    free(temp);
    size -= 1;
    return 0;
}
void printList()
{
    if(size == 0)
    {
        printf("Cannot Print An Empty List\n");
        return;
    }
    Node* temp = head; 
    while(temp->next != head)
    {
        printf("%s\n", temp->text);
        temp = temp->next;
    }
    printf("%s\n", temp->text);
    return;
}

void pushString()
{
    Node* new = (Node*)malloc(sizeof(Node));
    if(new == NULL)
    {
        printf("Error Creating Node\n");
        return;
    }
    printf("insert text\n");
    scanf("%s", &new->text);
    add(new);
    printf("done push string\n");
    return;
}
void deleteItem()
{
    int index = 0;
    printf("insert list index\n");
    scanf("%d", &index);
    removeItem(index);
    printf("done delete item\n");
    return;
}
void startScreen()
{
    printf("\n1 push string\n");
    printf("2 print list\n");
    printf("3 delete item\n");
    printf("4 end program\n");
}
void freeList()
{
    if(size == 0)
        return;
    Node *temp = head; 
    while(temp->next != head)
    {
        if(size == 0)
            return;
        removeItem(1);
    }
    removeItem(1);
    return;
}
int main()
{
    int input = 0;
    while(1)
    {
        input = 0;
        startScreen();
        scanf("%d", &input);
        if(input == 1)
            pushString();
        else if(input == 2)
            printList();
        else if(input == 3)
            deleteItem();
        else if(input == 4)
        {
            freeList();
            return 0;
        }
        else
        {
            printf("Invalid Input\n");
            exit(EXIT_FAILURE);
        }
    } 
    return 0;
}