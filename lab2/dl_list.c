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

int main()
{
    return 0; 
}