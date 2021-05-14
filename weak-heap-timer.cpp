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

class WeakHeap
{
    private:
        int size;
        int capacity;
        int *heap;
        int *reverse;

    public:

        WeakHeap()
        {
            size=0;
            capacity=0;
            heap = reverse = nullptr;
        }

        WeakHeap(int cap)
        {
            size = 0;
            capacity = cap;
            heap = new int[cap];
            reverse = new int[cap];
        }

        int Grandparent(int j){
            
            while(j%2==0)
                j=j/2;

            return floor(j/2);
        }

        int returnMin(){

            if(size==0)
                return -1;

            return heap[0];
        }

        void Merge(int i, int j){

            if(heap[i]>heap[j] && size!=2){
                swap(heap[i],heap[j]);
                reverse[j]=(!reverse[j]);
            }
        }

        void restoreWeakHeap(int m){
            
            int x=1;

            while( ((2*x) + reverse[x]) < size-1){
                x=(2*x) + reverse[x];
            }

            while(x>0){
                Merge(m,x);
                x=floor(x/2);
            }
        }

        int extractMin(){

            swap(heap[0],heap[size-1]);
            restoreWeakHeap(0);

            size--;

            return heap[size];
        }

        void decreaseKey(int x, int newkey){
            
            heap[x]=newkey;

            while(x!=0 && (heap[Grandparent(x)] > heap[x])){
                
                swap(heap[Grandparent(x)],heap[x]);
                reverse[x]=(!reverse[x]);
                x=Grandparent(x);
            }
        }

        int DeleteElement(int idx){

            int x=heap[idx];

            decreaseKey(idx, INT_MIN);

            //Display();

            extractMin();

            //Display();

            return x;
        }

        void Insert(int key){

            heap[size]=key;
            int x=size;
            reverse[x]=0;

            while(x!=0 && (heap[Grandparent(x)] > heap[x])){

                swap(heap[Grandparent(x)],heap[x]);
                reverse[x]=(!reverse[x]);
                x=Grandparent(x);
            }

            size++;
        }

        void Display(){

            cout<<"\n Weak heap: ";
            for(int i=0;i<size;i++){
                cout<<heap[i]<<" ";
            }
            cout<<"\n";
            /* cout<<"\n Displaying rev array: ";
            for(int i=0;i<size;i++){
                cout<<reverse[i]<<" ";
            }
            cout<<"\n"; */
        }
};

WeakHeap* createWeakHeap(int capacity)
{
    auto WH = new WeakHeap(capacity);
    vector<int> random_elements;
    iota(random_elements.begin(), random_elements.end(), 1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (random_elements.begin(), random_elements.end(), std::default_random_engine(seed));
    for(auto i : random_elements)
        WH->Insert(i);
    return WH;
}

void print_times()
{
    ofstream insert("Insert-WH.txt");
    ofstream decrease_key("DecreaseKey-WH.txt");
    ofstream extract_min("ExtractMin-WH.txt");
    ofstream del("Delete-WH.txt");

    for(auto i = 10; i <=1000000; i += 1000)
    {
        auto wh1 = createWeakHeap(i);
        auto wh2 = createWeakHeap(i);
		
	    double time_elapsed1 = timeMyFunction([&wh1](auto x){ wh1->Insert(x);}, i%1053);
	    printDetails(cout, "Weak Heap", "Insert", time_elapsed1, i);
	    printDetails(insert, "Weak Heap", "Insert", time_elapsed1, i);

        double time_elapsed2 = timeMyFunction([&wh1](){wh1->extractMin();});
        printDetails(cout, "Weak Heap", "Extract Min", time_elapsed2, i);
        printDetails(extract_min, "Weak Heap", "Extract Min", time_elapsed2, i);

        double time_elapsed3 = timeMyFunction([&wh1](int idx, int y){ wh1->decreaseKey(idx, y);}, i%1053, i%17);
        printDetails(cout, "Weak Heap", "Decrease Key", time_elapsed3, i);
        printDetails(decrease_key, "Weak Heap", "Decrease Key", time_elapsed3, i);

        double time_elapsed4 = timeMyFunction([&wh1](auto idx){ wh1->DeleteElement(idx);}, i%1053);
        printDetails(cout, "Weak Heap", "Delete", time_elapsed4, i);
        printDetails(del, "Weak Heap", "Delete", time_elapsed4, i);
    }
}

int main(){

    WeakHeap W(100);

    W.Insert(7);
    W.Display();
    W.Insert(6);
    W.Display();
    W.Insert(5);
    W.Display();
    W.Insert(4);
    W.Display();
    W.Insert(9);
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    W.Insert(10);
    W.Display();
    W.Insert(1);
    W.Display();
    W.Insert(2);
    W.Display();
    W.Insert(3);
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    //
    W.decreaseKey(5,3);
    cout<<endl<<"Decreased key at index "<<5<<" to "<<3;
    W.Display(); 
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    cout<<endl<<"Deleted "<<W.DeleteElement(2)<<" at index 4";
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    
    print_times();

    return 0;
}