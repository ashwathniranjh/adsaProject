#include<bits/stdc++.h>
using namespace std;

//Have written code as per the Pseudo Code on the given link: https://www.cs.princeton.edu/~chazelle/pubs/sheap.pdf
//Something got to do with initial rank, declaration. They have talked about trivial initialisation of Node and Head
//I used a constructor, default values might be wrong, I havent got a clue

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
    Node *queue;
    Head *next, *prev, *suffix_min;
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
        Node *sift(Node* );
    
    public:
        SoftHeap()
        {
            header = new Head();
            tail = new Head();
            tail->rank = std::numeric_limits<int>::max();
            header->next = tail;
            tail->prev = header;
            r = 0;
        }

        SoftHeap(int r) : SoftHeap()
        {
            header = new Head();
            tail = new Head();
            tail->rank = std::numeric_limits<int>::max();
            header->next = tail;
            tail->prev = header;
			this->r = r;
        }

        void insert(int key);
        void meld(Node *q);
        int deleteMin();

		bool isEmpty()
		{
			return (header == nullptr);
		}


};

void SoftHeap::insert(int key)
{
    cout<<"Inserting Key"<<endl;
	Node *q;
    Cell *cell;
    cell = new Cell();
    cell->data = key;
    cell->nextCell = nullptr;
    cout<<"Check-1"<<endl;

    q = new Node();
    q->rank = 0;
    q->ckey = key;
    q->head = cell;
    q->tail = cell;

	cout<<"Calling Meld"<<endl;
    this->meld(q);
    cout<<"INsert Succesful"<<endl;
}


void SoftHeap::fix_min_list(Head *head)
{
	Head *tmp_min;
	if(head->next == tail)
		tmp_min = head;
	else
		tmp_min = head->next->suffix_min;
	
	while(head != header)
	{
		if(head->queue->ckey < tmp_min->queue->ckey)
		{
			tmp_min = head;
		}
		
		head->suffix_min = tmp_min;
		head = head->prev;
	}
	cout<<"fix-min-lkist successful"<<endl;
}



void SoftHeap::meld(Node *q)
{
    
	cout<<"Meld Check - 1"<<endl;
	Head *head, *prev_head, *to_head = header->next;
	cout<<"CECNOEC"<<endl;
    Node *top, *bottom;
    cout<<"wronwernc"<<endl;
    cout<<q->rank<<endl;
    cout<<to_head->rank;
    while(q->rank > to_head->rank)
    {
    	cout<<"Meld Check - 2"<<endl;
		to_head = to_head->next;
    	cout<<"Meld Check - 2"<<endl;
	}
    
    cout<<"Check - 5"<<endl;
    prev_head = to_head->prev;
    
    while(q->rank == to_head->rank)
    {
        cout<<"Meld Check - 3"<<endl;
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
        cout<<"Meld Check - 4"<<endl;
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
    cout<<"Meld Successful"<<endl;
}

Node* SoftHeap::sift(Node *v)
{
	Node *tmp;
	v->head = v->tail = nullptr;
	if(v->next == nullptr && v->child == nullptr)
	{
		v->ckey = std::numeric_limits<int>::max();
		return v;
	}
	
	v->next = sift(v->next);
	
	if(v->next->ckey > v->child->ckey)
	{
		tmp = v->child;
		v->child = v->next;
		v->next = tmp;
	}
	
	if(v->rank > r && (v->rank % 2 == 1 || v->child->rank < v->rank-1))
	{
		v->next = sift(v->next);
		if(v->next->ckey > v->child->ckey)
		{
			tmp = v->child;
			v->child = v->next;
			v->next = tmp;
		}
		
		if(v->next->ckey != std::numeric_limits<int>::max() && v->next->head == nullptr)
		{
			v->next->tail->nextCell = v->head;
			v->head = v->next->head;
			
			if(v->tail == nullptr)
				v->tail = v->next->tail;
			
			v->ckey = v->next->ckey;
		}
		
	}
	
	if(v->child->ckey == std::numeric_limits<int>::max())
	{
		if(v->child->ckey == std::numeric_limits<int>::max())
		{
			v->child = nullptr;
			v->next = nullptr;
		}
		else
		{
			v->child = v->next->child;
			v->next = v->next->next;
		}
	}
	
	return v;
}

int SoftHeap::deleteMin()
{
	Node *tmp;
	int min, childcount;
	Head *h = header->next->suffix_min;
	while(h->queue->head != nullptr)
	{
		tmp = h->queue;
		childcount = 0;
		while(tmp->next != nullptr)
		{
			tmp = tmp->next;
			childcount++;
		}
		if(childcount < h->rank/2)
		{
			h->prev->next = h->next;
			h->next->prev = h->prev;
			fix_min_list(h->prev);
			tmp = h->queue;
			while(tmp->next != nullptr)
			{
				meld(tmp->child);
				tmp = tmp->next;
			}
		}
		else
		{
			h->queue = sift(h->queue);
			if(h->queue->ckey == std::numeric_limits<int>::max())
			{
				h->prev->next = h->next;
				h->next->prev = h->prev;
				h = h->prev;
			}
			fix_min_list(h);
		}
		h = header->next->suffix_min;
	}
	
	min = h->queue->head->data;
	h->queue->head = h->queue->head->nextCell;
	if(h->queue->head == nullptr) 
		h->queue->tail = nullptr;	
		
	return min;
}


int main()
{
	SoftHeap H1(5);
	vector<int> arr(100);
	iota(arr.begin(), arr.end(), 1);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	shuffle (arr.begin(), arr.end(), std::default_random_engine(seed));

	for(auto &i : arr)
	{
		H1.insert(i);
		cout<<"INserted - "<<i<<endl;
		assert(!H1.isEmpty());
	}
}

