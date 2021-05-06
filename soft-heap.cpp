#include<bits/stdc++.h>
using namespace std;

struct Cell
{
    int data;
    Cell *nextCell;
    Cell()
    {
        data = -1;
        nextCell = nullptr;
    }
};

struct Node
{
    int ckey;
    int rank;
    Node *next, *child;
    Cell *head, *tail; 

    Node()
    {
        ckey = -1;
        rank = 0;
        next = child = nullptr;
        head = tail = nullptr;
    }
};

struct Head
{
    struct Node *queue;
    struct Head *next, *prev, *suffix_min;
    int rank; 

    Head()
    {
        queue = nullptr;
        next = prev = suffix_min = nullptr;
        rank = 0;
    }
};

class SoftHeap
{
    private:
        Head *header, *tail;
        int r;
        void fix_min_list(Head*);
    
    public:
        SoftHeap()
        {
            header = new Head();
            tail = new Head();
            tail->rank = std::numeric_limits<int>::max();
            r = 0;
        }

        SoftHeap(int r) : SoftHeap()
        {
            this->r = r;
        }

        void insert(int key);
        void meld(Node *q);


};

void SoftHeap::insert(int key)
{
    Node *q;
    Cell *cell;
    cell = new Cell();
    cell->data = key;
    cell->nextCell = nullptr;

    q = new Node();
    q->rank = 0;
    q->ckey = key;
    q->head = cell;
    q->tail = cell;

    this->meld(q);
}

void SoftHeap::meld(Node *q)
{
    Head *head, *prev_head, *to_head = header->next;
    Node *top, *bottom;
    while(q->rank > to_head->rank)
        to_head = to_head->next;
    prev_head = to_head->prev;
    
    while(q->rank == to_head->rank)
    {
        if(to_head->queue->ckey > q->ckey)
        {
            top = q;
            bottom = to_head->queue;
        }
        else
        {
            top = to_head->queue;
            bottom = q;
        }

        q = new Node();
        q->ckey = top->ckey;
        q->rank = top->rank + 1;
        q->child = bottom;
        q->next = top;
        q->head = top->head;
        q->tail = top->tail;
        to_head = to_head->next;
    }

    if(prev_head == to_head->prev)
        head = new Head();
    else
        head = prev_head->next;
    
    head->queue = q;
    head->rank = q->rank;
    head->next = to_head;
    prev_head->next = head;
    to_head->prev = head;
    this->fix_min_list(head);
}