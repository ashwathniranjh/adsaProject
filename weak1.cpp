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

            cout<<"\n Displaying weak heap: ";
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
    W.Insert(10);
    W.Display();
    W.Insert(1);
    W.Display();
    W.Insert(2);
    W.Display();
    W.Insert(3);
    W.Display();
    cout<<endl<<"Deleted min element: "<<W.extractMin();
    W.Display();
    W.decreaseKey(0,1);
    W.Display();
    

    return 0;
}