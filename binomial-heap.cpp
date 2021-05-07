#include<bits/stdc++.h>
using namespace std;

/*
 Binomial Heap data structure satisfying the heap property, with an efficient merge operation.
 Useful for a priority queue implementation where merges are frequent.
 Complexity:    O(1) for first
                O(log N) for insert, merge, extractMin, decreaseKey and delete
*/

struct heapNode      //Structure for a Binomial Heap Node, consisting of five fields -> data, degree, parent, child and sibling
{
    int data, degree;
    heapNode *sibling, *parent, *child;
    
    heapNode()
    {
        this -> data = 0;
        this -> degree = 0;
        this -> sibling = this -> parent = this -> child = NULL;
    }
    
    heapNode(int data)
    {
        this -> data = data;
        this -> degree = 0;
        this -> sibling = this -> parent = this -> child = NULL;
    }
};

class binomialHeap
{
    heapNode *root;
    heapNode *min;
    void mergeTreeUtil(heapNode*, heapNode*);       //Function to link two binomial trees of degree k, and make it degree k+1
    heapNode* mergeRoots(binomialHeap*, binomialHeap*);       //Function to merge the root-lists of two binomial heaps
    
public:
    binomialHeap(){
    this -> root = NULL;        //Empty Binomial Heap
}
    binomialHeap(heapNode* x){       //Initialising Binomial Heap with a single node
    this -> root = x;
}
    bool isEmpty();
    void insert(heapNode* x);
    void merge(binomialHeap*);
    heapNode* first();
    heapNode* extractMin();
    void decreaseKey(heapNode*, int);
    void Delete(heapNode*);
};




void binomialHeap::mergeTreeUtil(heapNode *y, heapNode *z)
{
    assert(y->degree == z->degree && y->data >= z->data); //checking if degree of binomial trees to be linked is the same
    y -> parent = z;
    y -> sibling = z -> child;
    z -> child = y;
    z -> degree = z -> degree + 1;
}

heapNode* binomialHeap::mergeRoots(binomialHeap *x, binomialHeap *y)        //Function to merge the root-lists of two binomial heaps
{
    heapNode *merged = new heapNode();
    heapNode *end = merged;
    
    heapNode *L1 = x -> root;
    heapNode *L2 = y -> root;
    if (L1 == NULL) return L2;
    if (L2 == NULL) return L1;
    while (L1 != NULL || L2 != NULL)
    {
        if (L1 == NULL)
        {
            end -> sibling = L2;
            end = end -> sibling;
            L2 = L2 -> sibling;
        }
        else if (L2 == NULL)
        {
            end -> sibling = L1;
            end = end -> sibling;
            L1 = L1 -> sibling;
        }
        else
        {
            if (L1 -> degree < L2 -> degree)
            {
                end -> sibling = L1;
                end = end -> sibling;
                L1 = L1 -> sibling;
            }
            else
            {
                end -> sibling = L2;
                end = end -> sibling;
                L2 = L2 -> sibling;
            }
        }
    }
    return (merged -> sibling);
}

bool binomialHeap::isEmpty()        //Function to return if the binomial heap is empty or not
{
    if(this -> root == NULL)
        return 1;
    else
        return 0;
}

void binomialHeap::insert(heapNode* x){     //Insertion to a Binomial Heap, is nothing but creating a new Heap with the new node, and merging it with the
                                            //pre-existing heap
    this->merge(new binomialHeap(x));
}

void binomialHeap::merge(binomialHeap *h2)
{
    binomialHeap *tempHeap = new binomialHeap();
    tempHeap -> root = mergeRoots(this, h2);        //H = h1 U h2
    
    if (tempHeap -> root == NULL)       //Union of two empty binomial heaps, hence returning an empty heap
    {
        this -> root = NULL;
        this -> min = NULL;
        return;
    }
    
    heapNode *prev = NULL;
    heapNode *curr = tempHeap -> root;
    heapNode *next = curr -> sibling;

    //Traversing through merged root list of non-decreasing order, we need to ensure that there exists no trees of the same order in the binomial heap
    while (next != NULL)
    {
        		//If curr node and next node dont have same degree, move ahead
				//If curr node, next node, and next's sibling have the same degree, we move ahead
        if (curr -> degree != next -> degree || (next -> sibling != NULL && next -> sibling -> degree == curr -> degree))
        {
            prev = curr;
            curr = next;
        }
        //If curr node and next node are the only two trees with same degree, we merge accordingly respective data values, and relink the root lists to ensure 
		//proper traversal
        else if (curr -> data <= next -> data)
        {
            curr -> sibling = next -> sibling;
            mergeTreeUtil(next, curr);
        }
        else
        {
            if (prev == NULL) tempHeap -> root = next;
            else prev -> sibling = next;
            mergeTreeUtil(curr, next);
            curr = next;
        }
        next = curr -> sibling;
    }
    //Updating root list, to the new root list of H = h1 union h2
    this -> root = tempHeap -> root;
    this -> min = tempHeap -> root;
    heapNode *tmp = this -> root;

    //Searching for min, in the new union heap, and setting it to that node
    while (tmp != NULL)
    {
        if (tmp -> data < this -> min -> data) this -> min = tmp;
        tmp = tmp -> sibling;
    }
}

heapNode* binomialHeap::extractMin()
{
    heapNode *minNode = this -> min;
    // delete minNode from the list of root
    heapNode *prev = NULL;
    heapNode *x = this -> root;
    //loop to get the sibling of minNode
    while (x != minNode)
    {
        prev = x;
        x = x -> sibling;
    }

    //replacing siblings of minNode and its siblings
    if (prev == NULL) this -> root = x -> sibling;
    else prev -> sibling = x -> sibling;
    
    // reverse the list of minNode's children
    heapNode *revChd = NULL;
    heapNode *tmp = minNode -> child;
    
    while (tmp != NULL)
    {
        heapNode *next = tmp -> sibling;
        tmp -> sibling = revChd;
        revChd = tmp;
        tmp = next;
    }
    
    // merge the two lists
    
    binomialHeap *tempHeap = new binomialHeap();
    tempHeap -> root = revChd;
    this -> merge(tempHeap);
    
    return minNode;
}

void binomialHeap::decreaseKey(heapNode *x, int newKey)
{
    assert(x->data > newKey);
    x -> data = newKey;
    heapNode *y = x;
    heapNode *z = y -> parent;
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

void binomialHeap::Delete(heapNode *x)
{
    //Making the node smallest in the heap and then doing extract min operation
    decreaseKey(x, INT_MIN);
    extractMin();
}

int main()
{
    binomialHeap *h2 = new binomialHeap();
    
    heapNode *x = new heapNode(11);
    heapNode *y = new heapNode(5);
    
    h2 -> insert(x);
    h2 -> insert(y);
    h2 -> insert(new heapNode(3));
    h2 -> insert(new heapNode(8));
    h2 -> insert(new heapNode(4));
    
    h2 -> decreaseKey(x, 2);
    
    while (!h2 -> isEmpty())
    {
        printf("%d ", h2 -> extractMin() -> data);
    }
    printf("\n");
    return 0;
}