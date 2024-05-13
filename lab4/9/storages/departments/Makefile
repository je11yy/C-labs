all: main

main: tests.o errors.o BinarySearchTree.o department.o DynamicArray.o application_storage.o application.o BinaryHeap.o BinomialHeap.o FibonacciHeap.o LeftistHeap.o SkewHeap.o Treap.o HashSet.o Trie.o
	gcc tests.o errors.o BinarySearchTree.o department.o DynamicArray.o application_storage.o application.o BinaryHeap.o BinomialHeap.o FibonacciHeap.o LeftistHeap.o SkewHeap.o Treap.o HashSet.o Trie.o -g -o main -lm

tests.o: tests.c
	gcc -c tests.c

errors.o: ../../errors.c
	gcc -c ../../errors.c

application_storage.o: ../application_storage.c
	gcc -c ../application_storage.c

application.o: ../../application.c
	gcc -c ../../application.c

BinarySearchTree.o: BinarySearchTree.c
	gcc -c BinarySearchTree.c

HashSet.o: HashSet.c
	gcc -c HashSet.c

Trie.o: Trie.c
	gcc -c Trie.c

department.o: department.c
	gcc -c -g department.c

DynamicArray.o: DynamicArray.c
	gcc -c DynamicArray.c

BinaryHeap.o: ../applications/BinaryHeap.c
	gcc -c ../applications/BinaryHeap.c

BinomialHeap.o: ../applications/BinomialHeap.c
	gcc -c ../applications/BinomialHeap.c

FibonacciHeap.o: ../applications/FibonacciHeap.c
	gcc -c ../applications/FibonacciHeap.c

LeftistHeap.o: ../applications/LeftistHeap.c
	gcc -c ../applications/LeftistHeap.c

SkewHeap.o: ../applications/SkewHeap.c
	gcc -c ../applications/SkewHeap.c

Treap.o: ../applications/Treap.c
	gcc -c ../applications/Treap.c

clean:
	rm -rf *.o main
