#include<bits/stdc++.h>
using namespace std;

template <typename Func, typename... Args>
double timeMyFunction(Func func, Args &&...args) {
  auto start_time = std::chrono::steady_clock::now();
  func(args...);
  auto end_time = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_time =
      std::chrono::duration_cast<std::chrono::duration<double>>(end_time -
                                                                start_time);
  return elapsed_time.count();
}

//Helper Fxn to print to a file
void printDetails(ostream &os, const string &algo_name, const string &case_name,
                  double time_elapsed, size_t size) {
  os << algo_name << ":"<< case_name << ":" << size << ": " << fixed
     << setprecision(30) << time_elapsed << "\n";
}


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

void binomialHeap::insert(heapNode* x){     //Insertion to a Binomial Heap, is nothing but creating a new Heap with the new node, and merging it with the       //pre-existing heap
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

binomialHeap* createRandomHeap(int size)
{
    binomialHeap *bh = new binomialHeap();
    vector<int> random_elements(size);
    iota(random_elements.begin(), random_elements.end(), 1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (random_elements.begin(), random_elements.end(), std::default_random_engine(seed));
    for(auto i : random_elements)
        bh->insert(new heapNode(i));
    return bh;
}

void print_times()
{
    ofstream insert("Insert-BH.txt");
    ofstream decrease_key("DecreaseKey-BH.txt");
    ofstream extract_min("ExtractMin-BH.txt");
    ofstream merge("Merge-BH.txt");

    for(auto i = 10; i <=1000000; i += 1000)
    {
        auto bh1 = createRandomHeap(i);
        auto bh2 = createRandomHeap(i);
		
	    double time_elapsed1 = timeMyFunction([&bh1](auto x){ bh1->insert(new heapNode(x));}, i+1);
	    printDetails(cout, "Binomial Heap", "Insert", time_elapsed1, i);
	    printDetails(insert, "Binomial Heap", "Insert", time_elapsed1, i);

        double time_elapsed2 = timeMyFunction([&bh1](){bh1->extractMin();});
        printDetails(cout, "Binomial Heap", "Extract Min", time_elapsed2, i);
        printDetails(extract_min, "Binomial Heap", "Extract Min", time_elapsed2, i);

        heapNode* node_to_add = new heapNode(i+1);
        bh2->insert(node_to_add);
        double time_elapsed3 = timeMyFunction([&bh2](heapNode *x, int y){ bh2->decreaseKey(x, y);}, node_to_add, i%100);
        printDetails(cout, "Binomial Heap", "Decrease Key", time_elapsed3, i);
        printDetails(decrease_key, "Binomial Heap", "Decrease Key", time_elapsed3, i);

        double time_elapsed4 = timeMyFunction([&bh1](auto x){ bh1->merge(x);}, bh2);
        printDetails(cout, "Binomial Heap", "Merge", time_elapsed4, i);
        printDetails(merge, "Binomial Heap", "Merge", time_elapsed4, i);
     
    }
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
        cout<<h2 -> extractMin() -> data<<"\n";
    }
    cout<<"\n";

    print_times();
    return 0;
}
