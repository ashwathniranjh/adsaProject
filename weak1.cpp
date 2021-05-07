#include<bits/stdc++.h>
using namespace std;

class WeakHeap
{
    private:
        int size;
        int heap[100];
        int reverse[100];

    public:

        WeakHeap()
        {
            size=0;
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

            if(heap[i]>heap[j]){
                swap(heap[i],heap[j]);
                reverse[j]=(!reverse[j]);
            }
        }

        void restoreWeakHeap(int m){
            
            int x=1;

            while( ((2*x) + reverse[x]) < m){
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

            Display();

            extractMin();

            Display();

            /* restoreWeakHeap(0);
            Display(); */
            
            /* int y=heap[size-1];
            decreaseKey(size-1, y); */

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

int main(){

    WeakHeap W;

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
    W.decreaseKey(4,1);
    W.Display();
    /* cout<<endl<<"Deleted "<<W.DeleteElement(4)<<" at index 4";
    W.Display();
    W.decreaseKey(5,3);
    W.Display(); */
    cout<<endl<<"Extracted min element: "<<W.extractMin();
    W.Display();
    

    return 0;
}