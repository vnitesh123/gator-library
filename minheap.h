#include<iostream>
#include<string>
#include<vector>
#include<climits>

using namespace std;

// This entity captures all the information of a patron in the book reservation
// id of patron, priority of patron and time at which the reservation for book was made
struct PatronInfo{
    int patronID;
    int priorityNumber;
    long int timeOfReservation;
};


// This class implements the heap data structure required to manage the reservation of a book in the gator Library
class Heap{
    public:

    // List of patrons (heap)
    vector<PatronInfo> patrons;

    // insert a new patron in the reservation heap
    void insert(int patronID, int priorityNumber, long int timeOfReservation){
        PatronInfo node;
        node.patronID = patronID;
        node.priorityNumber = priorityNumber;
        node.timeOfReservation = timeOfReservation;

        patrons.push_back(node);
        int i = patrons.size()-1;

        while (i != 0 && isSwapNeeded(patrons[i], patrons[(i-1)/2])) {
            swapPatrons(i, (i-1)/2);
            i = (i - 1) / 2;
        }
    }

    // this function checks if we need to swap 2 patrons in the heap to ensure that min heap property is maintained
    bool isSwapNeeded(PatronInfo childPatron, PatronInfo parentPatron) {
        if(childPatron.priorityNumber < parentPatron.priorityNumber){
            return true;
        } 
        // in case of a tie in priory of patrons, looking for reservation times to resolve the tie
        else if (childPatron.priorityNumber == parentPatron.priorityNumber){
            if (childPatron.timeOfReservation< parentPatron.timeOfReservation) {
                return true;
            }
        }

        return false;
    }

    // this function swap the position of patrons in the heap
    void swapPatrons(int a, int b){
        PatronInfo temp = this->patrons[a];
        this->patrons[a] = this->patrons[b];
        this->patrons[b] = temp;
    }

    // this function is called we need to fix the heap after a remove min operation is called
    // after the top element is deleted, we replace it with the last node in the heap and then keep swapping the nodes by using heap property
    void heapify(int i) {
        // indexes to access the parents and childs in heap
        int parent = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int heap_size = this->patrons.size();

        if(left < heap_size && right< heap_size){
            int min_child;
            // this means that the left one is min of childs, so we swap with parent
            if (isSwapNeeded(patrons[left], patrons[right])){
                min_child = left;
            } else{
                min_child = right;
            }

            if (isSwapNeeded(patrons[min_child], patrons[parent])){
                // once a swap is done recursively fix the subtree heap
                swapPatrons(parent, min_child);
                heapify(min_child);
            }

        }
        // this means that the right chile is min of childs, so we swap with parent
        else if(left < heap_size && isSwapNeeded(patrons[left], patrons[parent])){
                // once a swap is done recursively fix the subtree heap
                swapPatrons(parent, left);
                heapify(left);
        }

        return;
    }


    // This operation removes the top element of the heap, which is the minimum of th heap, once it is remove, it is replaced with last node in the heap
    void pop(){
        if(patrons.size()==0){
            return;
        }

        if(patrons.size()==1){
            patrons.pop_back();
            return;
        }

        int n = patrons.size();

        patrons[0] = patrons[n-1];

        patrons.pop_back();

        // once the last node is sent to the top of the heap, we call this function to fix the heap property recursively
        heapify(0);
    }

    // get min of the heap, which is the top element
    PatronInfo getMin(){
        if (patrons.size() == 0){
            return PatronInfo{};
        }
        return patrons[0];
    } 
};
