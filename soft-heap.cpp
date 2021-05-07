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
        nextCell = NULL;
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
        next = child = NULL;
        head = tail = NULL;
    }
};



struct Head
{
    Node *queue;
    Head *next, *prev, *suffix_min;
    int rank; 

    Head()
    {
        queue = NULL;
        next = prev = suffix_min = NULL;
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
			return (header == NULL);
		}


};

void SoftHeap::insert(int key)
{
    cout<<"Inserting Key"<<endl;
	Node *q;
    Cell *cell = new Cell();
    cell->data = key;
    cell->nextCell = NULL;
    cout<<"Check-1"<<endl;

    q = new Node();
    q->rank = 0;
    q->ckey = key;
    q->head = cell;
    q->tail = cell;

	cout<<"Calling Meld"<<endl;
    this->meld(q);
    cout<<"Insert Succesful"<<endl;
}


void SoftHeap::fix_min_list(Head *head)
{
	Head *tmp_min;
	if(head->next == tail){
		cout<<"**1\n";
		tmp_min = head;
	}
	else{
		tmp_min = head->next->suffix_min;
		cout<<"**2\n";
	}
		
	
	while(head != NULL && header != head )
	{
		cout<<"**3\n";
		if(head->queue->ckey < tmp_min->queue->ckey)
		{
			cout<<"**4\n";
			tmp_min = head;
		}
		
		head->suffix_min = tmp_min;
		head = head->prev;
	}
	cout<<"fix-min-list successful"<<endl;
}



void SoftHeap::meld(Node *q)
{
    
	cout<<"Meld Check - 1"<<endl;
	Head *head= header->next, *prev_head= header->next, *to_head = header->next;
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
        cout<<"Meld Check - 4"<<q->rank<<endl;
    }
	cout<<"*****\n";
    if(prev_head == to_head->prev){
		head = new Head();
		cout<<"*****1\n";
	}   
    else{
		cout<<"*****2\n";
		head = prev_head->next;
	}
        
    
    head->queue = q;
    head->rank = q->rank;
    head->next = to_head;
    prev_head->next = head;
    to_head->prev = head;
	cout<<"*****3\n";
    this->fix_min_list(head);
    cout<<"Meld Successful"<<endl;
}

Node* SoftHeap::sift(Node *v)
{
	Node *tmp;
	v->head = v->tail = NULL;
	if(v->next == NULL && v->child == NULL)
	{
		v->ckey = std::numeric_limits<int>::max();
		return v;
	}
	
	v->next = this->sift(v->next);
	
	if(v->next->ckey > v->child->ckey)
	{
		tmp = v->child;
		v->child = v->next;
		v->next = tmp;
	}
	
	if(v->rank > this->r && (v->rank % 2 == 1 || v->child->rank < v->rank-1))
	{
		v->next = this->sift(v->next);
		if(v->next->ckey > v->child->ckey)
		{
			tmp = v->child;
			v->child = v->next;
			v->next = tmp;
		}
		
		if(v->next->ckey != std::numeric_limits<int>::max() && v->next->head != NULL)
		{
			v->next->tail->nextCell = v->head;
			v->head = v->next->head;
			
			if(v->tail == NULL)
				v->tail = v->next->tail;
			
			v->ckey = v->next->ckey;
		}
		
	}
	
	if(v->child->ckey == std::numeric_limits<int>::max())
	{
		if(v->child->ckey == std::numeric_limits<int>::max())
		{
			v->child = NULL;
			v->next = NULL;
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
	Head *h = this->header->next->suffix_min;
	while(h->queue->head != NULL)
	{
		tmp = h->queue;
		childcount = 0;
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
			childcount++;
		}
		if(childcount < h->rank/2)
		{
			h->prev->next = h->next;
			h->next->prev = h->prev;
			this->fix_min_list(h->prev);
			tmp = h->queue;
			while(tmp->next != NULL)
			{
				this->meld(tmp->child);
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
	if(h->queue->head == NULL) 
		h->queue->tail = NULL;	
		
	return min;
}


int main()
{
	SoftHeap H1(5);
	vector<int> arr(5);
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

