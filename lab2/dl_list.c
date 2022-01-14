#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    Node* next;
    Node* prev;
    char text[1000];
}Node;

Node* head = NULL;

void add(Node* new)
{
    if(head == NULL)
    {
        head = new;
        new->next = head;
        new->prev = head;
        return;
    }
    Node* temp = head;
    while(temp->next != head)
    {
        temp = temp->next;
    }
    temp->next = new;
    new->next = head;
    head->prev = new;
    return;
}

void remove(int index)
{
    if(head == NULL)
    {
        printf("Cannot Remove from empty List\n");
        return;
    }
    Node* temp = head; 

    for(int i = 1; i < index; i++)
    {
        if(temp->next = head)
        {
            printf("Index out of bounds on remove\n");
            return;
        }
        temp = temp->next;
    }
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next; 
    if(temp == head)
        head = temp->next;
    temp->next = NULL;
    temp->prev = NULL;
    free(temp);
    return;
}
void printList()
{
    if(head == NULL)
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
    int index;
    printf("insert list index\n");
    scanf("&d", &index);
    remove(index);
    return;
}
int main()
{
    return 0; 
}