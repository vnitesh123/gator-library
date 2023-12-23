# Advanced Data Structures Project 

## Gator Library

This project implements a Library Management System called GatorLibrary, from where the patrons/users can borrow or reserve the books they want. The system is implemented by using Red Black trees and Min Heap.

### Red Black trees 
- these are used to manage the books and store the details of the book in the library in an efficient way, where every book is a node of the red black tree.

### Binary Min Heap 
- This is used to manage the reservation made by patrons for a book according to their priority. When a book is unavailable to be borrowed, the patron can reserve it. Every book in the library has a separate min heap to manage the reservation of the patrons for the book.

## Execution steps

Run the following command to build the project
```
make
``` 

Execute the program as follows

```
./gatorLibrary <input_file>
```

<input_file> - is the file name for the input commands, a sample commands file can be found on sample.txt

Example command : 
```
./gatorLibrary sample
```

## Operations Supported

1. PrintBook(bookID): given a bookID, all the details of the book are printed
2. PrintBooks(bookID1, bookID2): print details of the all the book within the range [bookID1, bookID2]
3. InsertBook(bookID, bookName, authorName, availabilityStatus): Given the book details, add a new book to the library
4. BorrowBook(patronID, bookID, patronPriority): Given a patron details, and the bookID requested, assign the book to the patron if the book is available, else add the patron’s request to the Reservation Heap.
5. ReturnBook(patronID, bookID): To let patron return the book and update the book status to available if there are no pending reservation requests, else assign the book to the patron of who is at the top of the reservation heap
6. DeleteBook(bookID): To delete the book and if there are any reservations pending in the heap, the patrons are notified about its unavailability.
7. FindClosestBook(targetID) : To find the closest book to targetID. When multiple books are present are equally close, books are returned in the order of their bookID
8. ColorFlipCount(): This returns the number of colour flips performed so far in the Gator Library Red black tree. Acts a a monitoring tool for the Red black tree.


