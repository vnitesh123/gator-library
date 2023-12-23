#include<iostream>
#include<string>
#include "minheap.h"
#include <chrono>
#include <ctime>
#include <sstream>

struct Book {
    int bookID;
    string authorName;
    string bookName;
    string availabilityStatus;
    int borrowedBy;
    Heap reservations;
};

// RBTNode represents the node of a red black tree, that has a left and right child, parent, color of node and a book
// Every Book in the Library is stored as a node of Red black tree
struct RBTNode{
    struct RBTNode* left;
    struct RBTNode* right;
    struct RBTNode* parent;
    char color;
    Book book;
    // by default every new node created is going to be red and will later get updated when the tree is fixed
    RBTNode (Book b){
        book = b;
        color = 'r';
        left = NULL;
        right = NULL;
        parent = NULL;
    }
};

// This class implements the Gator Library for us and handles all the operation performed in our Library Management system
class Library {
    public:

    // stores the color flips performed in the RBT
    int colorflips;

    // This is do add a delay in the clock as the values provided by the clock library are having very bad prescision and resulting in same value for few reservation. 
    // To handle this delay counter is addedf
    int counter;

    // root node of RB tree which is the gateway to all the ther books of the library
    struct RBTNode* books;

    Library(){
        // initially no books are present
        books = NULL;
        colorflips = 0;
        counter = 0;
    }

    // This function adds a new book to the library
    void InsertBook(int bookID, string bookName, string authorName, string availabilityStatus){
        // create book details record
        Book b = Book{};
        b.bookID = bookID;
        b.bookName = bookName;
        b.authorName = authorName;
        b.availabilityStatus = availabilityStatus;
        b.borrowedBy = 0;
        b.reservations = Heap();

        // insert the book in the RB tree
        insertNode(b);
    }

    // inserts a book in the RB tree
    void insertNode(Book b){
        RBTNode* target = new RBTNode(b);

        // if no books were present, creating a new node and making it a root node and coloring it black
        if (books == NULL){
            books = target;
            books->color = 'b';
            return;
        }
        RBTNode* temp_parent = NULL;
        RBTNode* temp = this->books;

        // to know whether the node is going to be inserted to the left or right
        // this is used to create links with the new node and its parent
        bool insertToLeft = false;

        // else we look for a place where the node can be inserted in the tree
        while(temp!=NULL){
            if(b.bookID < temp->book.bookID){
                temp_parent = temp;
                temp = temp->left;
                insertToLeft = true;
            } else {
                temp_parent = temp;
                temp = temp->right;
                insertToLeft = false;
            }
        }


        // if the node is to be inserted to left, then the parents left child is linked as the target node
        if(insertToLeft){
            target->parent = temp_parent;
            temp_parent->left = target;
        } 
        // else the paren's right pointer will be linked to the target node
        else{
            target->parent = temp_parent;
            temp_parent->right = target;
        }

        // fix the tree imbalance if any after the inser is performed
        balanceTreeAfterInsert(target);
    }

    // this method searches for a node whose bookID = key and returns it, if nothing found return null
    RBTNode* searchBook(int key){
        if (this->books != NULL){
            RBTNode* temp = books;
            while(temp!=NULL){
                if (temp->book.bookID == key){
                    return temp;
                } else if(temp->book.bookID< key) {
                    temp= temp->right;
                } else{
                    temp = temp->left;
                }
            }
        }

        return NULL;
    }

    // This function prints the details of the book
    string PrintBook(int bookID){
        // create a output string buffer and keep on adding the out statements to be printed
        stringstream output_buff;
        string output = "";

        RBTNode* bookNode = searchBook(bookID);

        // if a book is found, printing the details of the book
        if(bookNode != NULL){
            Book b = bookNode->book;
            output_buff<<"BookID = "<< b.bookID<<endl;
            output_buff<<"Title = \""<<b.bookName<<"\""<<endl;
            output_buff<<"Author = \""<<b.authorName<<"\""<<endl;
            output_buff<<"Availability = \""<<b.availabilityStatus<<"\""<<endl;
            if (b.borrowedBy == 0){
                output_buff<<"BorrowedBy = None"<<endl;
            } else{
                output_buff<<"BorrowedBy = "<<b.borrowedBy<<endl;
            }

            output_buff<<"Reservations = [";
            int n = b.reservations.patrons.size();
            // fetches the reservations of the book and prints them in the order of their priority
            Heap temp = b.reservations;
            for(int i=0;i<n;i++){
                PatronInfo pi = temp.getMin();
                temp.pop();
                if(i != n-1){
                    output_buff<<pi.patronID<<", ";
                } else{
                    output_buff<<pi.patronID;
                }
            }
            output_buff<<"]"<<endl;
            output_buff<<endl;

        } 
        // else printing book not found in the library
        else{ 
            output_buff<< "Book "<<bookID<<" not found in the Library"<<endl<<endl;
        }

        output = output_buff.str();
        return output;
    }

    // This function is executed when a partron makes a borrow request for a book in library
    string BorrowBook(int patronID, int bookID, int patronPriority){
        stringstream output_buff;
        auto currentTime = std::chrono::system_clock::now();

        // Recording the time of the request
        auto duration = currentTime.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

        // Convert milliseconds to long int
        long int currentTimeInMilliseconds = static_cast<long int>(millis.count()) + (this->counter);
        // adding delay counter to add more precision
        this->counter = this->counter + 3;

        // fething the book requested by patron
        RBTNode* bookNode = searchBook(bookID);
        if(bookNode == NULL){
            output_buff<< "Book "<<bookID<<" not found in the Library"<<endl<<endl;
            return output_buff.str();
        }

        // if the book is available, assigning the book to the patron and updating the borrower details of the book
        if (bookNode->book.availabilityStatus == "Yes"){
            bookNode->book.availabilityStatus = "No";
            bookNode->book.borrowedBy = patronID;
            output_buff<< "Book "<<bookID<<" Borrowed by Patron "<<patronID<<endl<<endl;

        } 
        // else, adding the patron to the reservation list of the book bease on his priority
        else{
            bookNode->book.reservations.insert(patronID, patronPriority, currentTimeInMilliseconds);
            output_buff<< "Book "<<bookID<<" Reserved by Patron "<<patronID<<endl<<endl;
        }

        return output_buff.str();
    }

    // This function is executed when a partron return a book borrowed by him from the library
    string ReturnBook(int patronID, int bookID){
        stringstream output_buff;
        // Fething the book details
        RBTNode* bookNode = searchBook(bookID);
        output_buff<<"Book "<< bookID<< " Returned by Patron "<<patronID<<endl<<endl;

        // If no more reservation are left for the book, changing the book status to available
        if (bookNode->book.reservations.patrons.size()==0){
            bookNode->book.borrowedBy = 0;
            bookNode->book.availabilityStatus = "Yes";
        } 
        // else assing the book to the patron in the reservation list with top priority
        else{
            int newBorrower = bookNode->book.reservations.getMin().patronID;
            bookNode->book.reservations.pop();
            bookNode->book.borrowedBy = newBorrower;
            output_buff<<"Book "<< bookID<< " Allotted to Patron "<<newBorrower<<endl<<endl;
        }

        return output_buff.str();

    }

    // This function prints all the books details falling in the given range 
    string PrintBooks(int leftRange, int rightRange){
        stringstream output_buff;
        vector<Book> resultBooks;
        RBTNode* temp = this->books;
        getBooksInRange(leftRange, rightRange, resultBooks, temp);

        for(int i=0;i<resultBooks.size(); i++){
            Book b = resultBooks[i];
            output_buff<<"BookID = "<< b.bookID<<endl;
            output_buff<<"Title = \""<<b.bookName<<"\""<<endl;
            output_buff<<"Author = \""<<b.authorName<<"\""<<endl;
            output_buff<<"Availability = \""<<b.availabilityStatus<<"\""<<endl;
            if (b.borrowedBy == 0){
                output_buff<<"BorrowedBy = None"<<endl;
            } else{
                output_buff<<"BorrowedBy = "<<b.borrowedBy<<endl;
            }

            output_buff<<"Reservations = [";
            int n = b.reservations.patrons.size();
            Heap temp = b.reservations;
            for(int i=0;i<n;i++){
                PatronInfo pi = temp.getMin();
                temp.pop();
                if(i != n-1){
                    output_buff<<pi.patronID<<", ";
                } else{
                    output_buff<<pi.patronID;
                }
            }
            output_buff<<"]"<<endl;
            output_buff<<endl;
        }

        return output_buff.str();
    }

    // fetches the books in the given ranges
    void getBooksInRange(int leftRange, int rightRange, vector<Book> &resultBooks, RBTNode* currentNode){
        if(currentNode == NULL){
            return;
        }

        if(currentNode->book.bookID < leftRange){
            // if left range is grater that bookID of the root, only look ar the right subtree 
            getBooksInRange(leftRange, rightRange, resultBooks, currentNode->right);
            return;
        }

        if(currentNode->book.bookID > rightRange){
            // if right range is lesser that bookID of the root, only look ar the left subtree 
            getBooksInRange(leftRange, rightRange, resultBooks, currentNode-> left);
        } else{
            // else look at both subtrees
            getBooksInRange(leftRange, rightRange, resultBooks, currentNode-> left);
            resultBooks.push_back(currentNode->book);
            getBooksInRange(leftRange, rightRange, resultBooks, currentNode->right);
        }

        
    }

    // this method balance the RB tree during a Delete Book operation
    void balanceTreeAfterDelete(RBTNode* y){
        RBTNode* py = y->parent;

        // if null return 
        if (py == NULL){
            return;
        }

        RBTNode* v;

        bool isLeftChild;

        // checking if parent of deficient node is left or right child
        if(py->left == y){
            v = py->right; 
            isLeftChild = true;
        } else{
            v = py->left;
            isLeftChild = false;
        }


        // if sibling's colour is black do colour flips and rotation about parent in the directection of deficiency node
        if(v != NULL && v->color == 'r'){
            if(isLeftChild){
                performRRRotate(py);
                v->color = 'b';
                py->color = 'r';

                this->colorflips = this->colorflips+2;
                balanceTreeAfterDelete(y);
            } else {
                performLLRotate(py);
                v->color = 'b';
                py->color = 'r';
                this->colorflips = this->colorflips+2;
                balanceTreeAfterDelete(y);
            }
        } else if (v != NULL && v->color == 'b'){ // if sibling is black
            if(isLeftChild){ 
                // if sibling children are null or black
                // doing colour flips and repeat the process
                if((v->left == NULL || v->left->color == 'b') && (v->right == NULL || v->right->color == 'b')) {
                    if (v->color == 'b'){
                        v->color = 'r';
                        this->colorflips = this->colorflips+1;
                    } 
                    
                    if(py->color == 'r') {
                        py->color = 'b';
                        this->colorflips = this->colorflips+1;
                    } else {
                        balanceTreeAfterDelete(py);
                    }
                } else {
                    // if a far child is black, perform rotation away from the deficiency about sibling node and performing colour flips
                    if(v->right == NULL || v->right->color == 'b') {
                        performLLRotate(v);
                        if(v->color == 'b'){
                            v->color = 'r';
                            this->colorflips = this->colorflips+1;
                        }
                        if (v->parent->color =='r'){
                            v->parent->color = 'b';
                            this->colorflips = this->colorflips+1;
                        }
                        v = v->parent;
                    }// perform a rotation towards defeciency node about deficiency node's parent and doing colour flips
                    performRRRotate(py);
                    if (v->color != py->color){
                        v->color = py->color;
                        this->colorflips = this->colorflips+1;
                    }
                    if (py->color != 'b'){
                        py->color = 'b';
                        this->colorflips = this->colorflips+1;
                    }

                    if(v->right->color !='b'){
                        v->right->color = 'b';
                        this->colorflips = this->colorflips+1;
                    }
                    
                    if(v->parent == NULL){
                        this->books = v;
                    }
                }

            } else{ // symmetically as the above we do the same steps
                if((v->left == NULL || v->left->color == 'b') && (v->right == NULL || v->right->color == 'b')) {
                    if (v->color == 'b'){
                        v->color = 'r';
                        this->colorflips = this->colorflips+1;
                    } 
                
                    if(py->color == 'r') {
                        py->color = 'b';
                        this->colorflips = this->colorflips+1;
                    } else {
                        balanceTreeAfterDelete(py);
                    }
                } else {
                    if(v->left == NULL || v->left->color == 'b') {
                        performRRRotate(v);
                        if(v->color == 'b'){
                            v->color = 'r';
                            this->colorflips = this->colorflips+1;
                        }
                        if (v->parent->color =='r'){
                            v->parent->color = 'b';
                            this->colorflips = this->colorflips+1;
                        }
                        
                        v = v->parent;

                        if(v->parent == NULL){
                            this->books = v;
                        }
                    }
                    performLLRotate(py);
                    if (v->color != py->color){
                        v->color = py->color;
                        this->colorflips = this->colorflips+1;
                    }
                    if (py->color!='b'){
                        py->color = 'b';
                        this->colorflips = this->colorflips+1;
                    }
                    
                    if (v->left->color != 'b'){
                        v->left->color = 'b';
                        this->colorflips = this->colorflips+1;
                    }
                    
                    if(v->parent == NULL){
                        this->books = v;
                    }
                }
            }
        }


    }

    // Deletes the details of the deleted book and notifies the patraons in the reservation heap about book going unavailable 
    string PrintDeletedBook(Book b){
        stringstream output_buff;

        output_buff<<"Book "<< b.bookID<<" is no longer available.";
        if (b.reservations.patrons.size() == 1){
            PatronInfo patron = b.reservations.getMin();
            output_buff<<" Reservation made by Patron " << patron.patronID <<" has been cancelled!\n\n";
        } else if (b.reservations.patrons.size() > 1){
            int n = b.reservations.patrons.size();
            output_buff<<" Reservations made by Patrons ";
            for(int i=0; i<n;i++){
                PatronInfo patron = b.reservations.getMin();
                b.reservations.pop();
                if(i==n-1){
                    output_buff<<patron.patronID;
                }else{
                    output_buff<<patron.patronID<<", ";
                }
            }

            output_buff<<" have been cancelled!\n\n";

        } else{
            output_buff<<"\n\n";
        }

        return output_buff.str();

    }



    // Deletes a book from the library
    string DeleteBook(int bookID){
        RBTNode* deleted_node;
        RBTNode* bookNode = searchBook(bookID);

        // if null return
        if(bookNode == NULL){
            return "";
        }

        // if a degree 2 node is deleted, searching for the node that replaces it, and the target will be deleted
        if(bookNode->left!=NULL && bookNode->right != NULL){
            RBTNode* target = bookNode->left;
            while(target->right != NULL){
                target = target->right;
            }

            Book temp_book = bookNode->book;
            bookNode->book = target->book;

            // if target is leaf
            if (target->left == NULL && target->right == NULL){
                // if target is black we will it and fix the tree imbalance
                deleted_node = target;
                if(deleted_node->color == 'b'){
                    // calling rebalancing function in case of black leaf
                    balanceTreeAfterDelete(target);
                }
                
                // remove links of target with its parent
                RBTNode* temp = deleted_node->parent;
                if(temp->right == deleted_node){
                    temp->right = NULL;
                    deleted_node->parent = NULL;
                    deleted_node->book = temp_book;
                } else{
                    temp->left = NULL;
                    deleted_node->parent = NULL;
                    deleted_node->book = temp_book;
                }
            } 
            // if the target node to be deleted is a degree one node, handling differently
            else if (target->left != NULL || target->right != NULL){ 
                RBTNode* newReplacableNode;
                if(target->left == NULL){
                    newReplacableNode = target->right;
                } else{
                    newReplacableNode = target->left;
                }

                // if the child of the deleted node is red, make the child of the deleted node's parent
                if(newReplacableNode->color == 'r'){
                    newReplacableNode->parent = target->parent;
                    RBTNode* temp_parent = target->parent;
                    // checking for root condition
                    if (temp_parent != NULL){
                        if (temp_parent->right == target){
                            temp_parent->right = newReplacableNode;
                            target->parent = NULL;
                            target->book = temp_book;
                        } else{
                            temp_parent-> left = newReplacableNode;
                            target->parent = NULL;
                            target->book = temp_book;
                        }
                    } else {
                        this->books = newReplacableNode;
                    }

                    newReplacableNode->color = 'b';
                    this->colorflips = this->colorflips + 1;
                    deleted_node = target;
                }

            }
            
            // delted the  1 degree node
        } else if (bookNode->left!=NULL || bookNode->right != NULL){
            RBTNode* target;
            if(bookNode->left == NULL){
                target = bookNode->right;
            } else{
                target = bookNode->left;
            }

            // if the child of the deleted node is red, make the child of the deleted node's parent
            if(target->color == 'r'){
                target->parent = bookNode->parent;
                RBTNode* temp_parent = bookNode->parent;
                // checking for root condition
                if (temp_parent != NULL){
                    if (temp_parent->right == bookNode){
                        temp_parent->right = target;
                        bookNode->parent = NULL;
                    } else{
                        temp_parent-> left = target;
                        bookNode->parent = NULL;
                    }
                } else {
                    this->books = target;
                }

                target->color = 'b';
                this->colorflips = this->colorflips + 1;
                deleted_node = bookNode;
            }
        } else {
            deleted_node = bookNode;
            // if the root is the only node in tree and is to be deleted
            if (bookNode->parent == NULL){
                this->books = NULL;
            } else{

                if(bookNode->color == 'b'){
                    balanceTreeAfterDelete(bookNode);
                }
                //deleted_node = bookNode;
                
                RBTNode* temp = deleted_node->parent;
                // if (bookNode->color == 'r'){
                if(temp->right == deleted_node){
                    temp->right = NULL;
                    deleted_node->parent = NULL;
                } else{
                    temp->left = NULL;
                    deleted_node->parent = NULL;
                }
            }
            
        }
        // returning details of the deleted book
        return PrintDeletedBook(deleted_node->book);
    }

    // finds the closest book to bookID
    string FindClosestBook(int bookID){
        stringstream output_buff;
        vector<Book> closestBooks;
        int closestDiff = INT_MAX;
        RBTNode* root = this->books;

        while (root != NULL) {
            int diff = bookID - root->book.bookID;
            if (diff == 0) {
                closestBooks.clear();
                closestBooks.push_back(root->book);
                break;
            }

            if (abs(diff) < closestDiff) {
                closestDiff = abs(diff);
                closestBooks.clear();
                closestBooks.push_back(root->book);
            } else if (abs(diff) == closestDiff) {
                if (closestBooks.size()==0){
                    closestBooks.push_back(root->book);
                } else if (closestBooks[0].bookID > root->book.bookID) { // in case of multiple close books
                    Book temp = closestBooks[0];
                    closestBooks[0] = root->book;
                    closestBooks.push_back(temp);
                }
                
            }

            if (diff < 0) {
                root = root->left;
            } else {
                root = root->right;
            }
        }

        // printing the details of all the closest books
        for (int i=0; i< closestBooks.size(); i++){
            Book b = closestBooks[i];
            output_buff<<"BookID = "<< b.bookID<<endl;
            output_buff<<"Title = \""<<b.bookName<<"\""<<endl;
            output_buff<<"Author = \""<<b.authorName<<"\""<<endl;
            output_buff<<"Availability = \""<<b.availabilityStatus<<"\""<<endl;
            if (b.borrowedBy == 0){
                output_buff<<"BorrowedBy = None"<<endl;
            } else{
                output_buff<<"BorrowedBy = "<<b.borrowedBy<<endl;
            }

            output_buff<<"Reservations = [";
            int n = b.reservations.patrons.size();
            Heap temp = b.reservations;
            for(int i=0;i<n;i++){
                PatronInfo pi = temp.getMin();
                temp.pop();
                if(i != n-1){
                    output_buff<<pi.patronID<<", ";
                } else{
                    output_buff<<pi.patronID;
                }
            }
            output_buff<<"]"<<endl;
            output_buff<<endl;
        }

        return output_buff.str();
    }
    
    // performs LL rotation about root passed
    void performLLRotate(RBTNode* root){
        RBTNode* l = root->left;
        RBTNode* c = l->right;

        RBTNode* parent = root->parent;

        l->parent = parent;

        l->right = root;
        root->parent = l;
        root->left = c;

        if(c!=NULL){
            c->parent = root;
        }
        if (parent!=NULL){
            if(parent->right == root){
                parent->right = l;
            } else{
                parent->left = l;
            }
        }
    }

    // performs RR rotation about root passed
    void performRRRotate(RBTNode* root){
        RBTNode* r = root->right;
        RBTNode* c = r->left;

        RBTNode* parent = root->parent;

        r->parent = parent;

        r->left = root;
        root->parent = r;
        root->right = c;

        if(c!=NULL){
            c->parent = root;
        }
        if (parent!=NULL){
            if(parent->right == root){
                parent->right = r;
            } else{
                parent->left = r;
            }
        }
    }

    // responsiple for balaning the tree after insert
    void balanceTreeAfterInsert(RBTNode* root){
        if (root == books){ 
            root->color = 'b';
            return; 
        }

        RBTNode* parent = root->parent;
        
        RBTNode* gp = root->parent->parent;
       
        if(parent->color == 'b'){
            return;
        }

        // done until the colour of parent is black or root is reached
        while(parent!= NULL && parent->color != 'b' && gp != NULL){
            if(gp->left == parent){
                RBTNode* d = gp->right;
                // if sibling is red performing colour flips
                if(d != NULL && d->color == 'r'){
                    d->color = 'b';
                    parent->color = 'b';
                    gp->color ='r';
                    root = gp;
                    parent = root->parent;
                    if(parent != NULL){
                        gp = root->parent->parent;
                    }
                    this->colorflips = this->colorflips + 3; 
                } else {
                    // else performing rotations
                    if (parent->right == root){
                        performRRRotate(parent);
                        root = parent;
                        parent = root->parent;
                    }
                    performLLRotate(gp);
                    gp->color = 'r';
                    parent-> color ='b';
                    if(parent->parent == NULL){
                        books = parent;
                    }
                    this->colorflips = this->colorflips + 2;
                }
            } else {
                RBTNode* d = gp->left;
                // if sibling is red performing colour flips
                if (d!=NULL && d->color == 'r'){
                    d->color = 'b';
                    parent->color = 'b';
                    gp->color ='r';
                    root = gp;
                    parent = root->parent;
                    if(parent != NULL){
                        gp = root->parent->parent;
                    }
                    this->colorflips = this->colorflips + 3; 
                } else {
                    // else performing rotations
                    if (parent->left == root){
                        performLLRotate(parent);
                        root = parent;
                        parent = root->parent;
                    }
                    performRRRotate(gp);
                    gp->color = 'r';
                    parent-> color ='b';
                    if(parent->parent == NULL){
                        books = parent;
                    }
                    this->colorflips = this->colorflips + 2; 
                }
            }
        }

        if (this->books->color == 'r'){
            this->books->color = 'b';
            this->colorflips = this->colorflips+1;
        }

    }

    // returns the colour flips done so far
    string ColorFlipCount(){
        stringstream output_buff;
        output_buff<<"Colour Flip Count: "<< this->colorflips <<endl<<endl;
        return output_buff.str();
    }


};