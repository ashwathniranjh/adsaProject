#include<bits/stdc++.h>
using namespace std;

struct Node //Structure for a Binomial Heap Node, consisting of five fields -> data, degree, parent, child and sibling
{
    int data;
    int degree;
    Node *parent;
    Node *child;
    Node *sibling;

    Node()
    {
        data = 0;
        degree = 0;
        parent = child = sibling = nullptr;
    }

    Node(int x)
    {
        data = x;
        degree = 0;
        parent = child = sibling = nullptr;
    }
};

class binomialHeap
{
    private:
        
        Node *roots; //Head for the Linked List to store the roots of the various min-binomial trees part of the binomial heaps
        Node *min;   //Pointer pointing to the minimum value node to the heap
        
        void linkTrees(Node* y,  Node *z) //Function to link two binomial trees of degree k, and make it degree k+1
        {
            assert(y->degree == z->degree && y->data <= z->data); //checking if degree of binomial trees to be linked is the same
            
            z->parent = y;
            y->child = z;
            z->sibling = y->child;
            y->degree++;
            
            return;
        }
        
        Node* mergeRoots(binomialHeap *h1, binomialHeap *h2) //Function to merge the root-lists of two binomial heaps
        {
            Node *dummy = new Node();
            Node *tmp = dummy;

            Node *L1 = h1->roots;
            Node *L2 = h2->roots;

            if(!L1)
                return L2;

            if(!L2)
                return L1;

            while(L1 != nullptr || L2 != nullptr)
            {
                if(L1 == nullptr)
                {
                    tmp->sibling = L2;
                    tmp = tmp->sibling;
                    L2 = L2->sibling;
                }
                else if(L2 == nullptr)
                {
                    tmp->sibling = L1;
                    tmp = tmp->sibling;
                    L1 = L1->sibling;
                }
                else
                {
                    if(L1->degree <= L2->degree)
                    {
                        tmp->sibling = L1;
                        tmp = tmp->sibling;
                        L1 = L1->sibling;
                    }
                    else
                    {
                        tmp->sibling = L2;
                        tmp = tmp->sibling;
                        L2 = L2->sibling;
                    }

                }
            }

            return (tmp->sibling);
        }

    public:
        binomialHeap()
        {
            this->roots = nullptr; //Empty Binomial Heap
        }
        
        binomialHeap(Node* n)
        {
            this->roots = n; //Initialising Binomial Heap with a single node
        }

        bool isEmpty()  //Function to return if the binomial heap is empty or not
        {
            return (roots == nullptr);
        }

        void insert(Node* new_node) //Insertion to a Binomial Heap, is nothing but creating a new Heap with the new node, and merging it with the
                                    //pre-existing heap
        {
            merge(new binomialHeap(new_node));
        }

        void merge(binomialHeap* h2)
        {
            binomialHeap *H; //H = h1 U h2
            H->roots = this->mergeRoots(this, h2);
            
			if(H->roots == nullptr) //Union of two empty binomial heaps, hence returning an empty heap
            {
            	this->roots = nullptr;
            	this->min = nullptr;
				return;
            	
			}
			
			Node *prev = nullptr;
			Node *curr = H->roots;
			Node *next = curr->sibling;
			
			//Traversing through merged root list of non-decreasing order, we need to ensure that there exists no trees of the same order in the binomial heap
			
			while(next != nullptr)
			{
				
				//If curr node and next node dont have same degree, move ahead
				//If curr node, next node, and next's sibling have the same degree, we move ahead
				if(curr->degree != next->degree || (next->sibling != nullptr) && (curr->degree == next->degree) && (curr->degree == next->sibling->degree))
				{
					prev = curr;
					curr = next;
					next = curr->sibling;
				}
				//If curr node and next node are the only two trees with same degree, we merge accordingly respective data values, and relink the root lists to ensure 
				//proper traversal
				else if(curr->data <= next->data)
				{
					curr->sibling = next->sibling;
					linkTrees(curr, next);
					next = curr->sibling;
				}
				else
				{
					if(prev == nullptr)
						H->roots = next;
					else
						prev->sibling = next;
					
					linkTrees(next, curr);
					curr = next;
					next = curr->sibling;
				}
			}
			
			//Updating root list, to the new root list of H = h1 union h2
			this->roots = H->roots;
			this->min = H->roots;
			
			//Searching for min, in the new union heap, and setting it to that node
			Node *tmp = this->roots;
			while(tmp != nullptr)
			{
				if(tmp->data < this->min->data)
					this->min = tmp;
				tmp = tmp->sibling;
			}
            
			return;
        }

        Node *extractMin(){
                Node *minNode = this -> min;
                // delete minNode from the list of roots
                Node *y = nullptr;
                Node *x = this -> roots;
                //loop to get the sibling of minNode
                while (x != minNode)
                {
                    y = x;
                    x = x -> sibling;
                }
                //replacing siblings of minNode and its siblings
                if (y == nullptr) this -> roots = x -> sibling;
                else y -> sibling = x -> sibling;

                // reverse the list of minNode's children
                Node *revChildPtr = nullptr;
                Node *cur = minNode -> child;
                while (cur != nullptr)
                {
                    Node *next = cur -> sibling;
                    cur -> sibling = revChildPtr;
                    revChildPtr = cur;
                    cur = next;
                }

                // merge the main list and the child list of minNode
                binomialHeap *tempHeap = new binomialHeap();
                tempHeap -> roots = revChildPtr;
                this -> merge(tempHeap);

                return minNode;
        }
        void decreaseKey(Node *x, int updatedData){
                // Precondition: x -> data > updatedData
                x -> data = updatedData;
                Node *y = x;
                Node *z = y -> parent;
                //bringing the decreased node to parents pos if its smaller
                while (z != NULL && y -> data < z -> data)
                {
                    // swap contents
                    swap(y -> data, z -> data);
                    
                    y = z;
                    z = y -> parent;
                }
                //replacing min with decreased node if its smaller
                if (y -> data < this -> min -> data) this -> min = y;
        }
        void deleteNode(Node *x){
            //Making the node smallest in the heap and then doing extract min operation
            decreaseKey(x, INT_MIN);
            extractMin();
        }
};

int main()
{
    binomialHeap *bh = new binomialHeap();
    
    Node *x = new Node(11);
    Node *y = new Node(5);
    
    bh -> insert(x);
    bh -> insert(y);
    bh -> insert(new Node(3));
    bh -> insert(new Node(8));
    bh -> insert(new Node(4));
    
    bh -> decreaseKey(x, 2);
    
    while (!bh -> isEmpty())
    {
        cout<<bh -> extractMin() -> data<<" ";

    }
    cout<<"\n";
    return 0;
}