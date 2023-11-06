// Developer: Sunny Dhama
// Topic: Advanced Data Structures - Project 1
// Date: 04/09/2023
// Description: This program is a simulation of a taxi service. It reads in a file of taxi rides and stores them in a red-black tree. It then reads in a file of commands and executes them. The commands are: Insert, Print, GetNextRide, UpdateRide and CancelRide. The program outputs the results of the commands to an output file.
// The program is written in C++ and uses the following data structures: Red-Black Tree, Min Heap, and a custom class called Ride.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

// Ride class
class Ride
{
public:
    int rideNumber = 0;
    int rideCost = 0;
    int tripDuration = 0;

    Ride(int rideNumber, int rideCost, int tripDuration) : rideNumber(rideNumber), rideCost(rideCost), tripDuration(tripDuration) {}

    // Overloaded comparison operators
    int compareTo(const Ride &other) const
    {
        if (rideCost == other.rideCost)
        {
            return tripDuration - other.tripDuration;
        }
        return rideCost - other.rideCost;
    }

    // Overloaded output operator
    std::string toString() const
    {
        std::ostringstream oss;
        oss << "(" << rideNumber << "," << rideCost << "," << tripDuration << ")";
        return oss.str();
    }
};

// Min Heap class for storing the rides in order of cost and duration
class MinHeap
{
private:
    std::vector<Ride> heap;
    int size;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void siftUp(int i);
    void siftDown(int i);

public:
    MinHeap(int capacity) : size(0) { heap.reserve(capacity); }

    bool isEmpty() const { return size == 0; }
    Ride getMin() const { return heap[0]; }
    void insert(const Ride &ride);
    Ride extractMin();
    void remove(const Ride &ride);
};

// siftUp function for the Min Heap
void MinHeap::siftUp(int i)
{
    while (i > 0 && heap[parent(i)].compareTo(heap[i]) > 0)
    {
        std::swap(heap[parent(i)], heap[i]);
        i = parent(i);
    }
}

// siftDown function for the Min Heap
void MinHeap::siftDown(int i)
{
    int minIndex = i;
    int l = left(i);
    if (l < size && heap[l].compareTo(heap[minIndex]) < 0)
    {
        minIndex = l;
    }

    int r = right(i);
    if (r < size && heap[r].compareTo(heap[minIndex]) < 0)
    {
        minIndex = r;
    }

    if (i != minIndex)
    {
        std::swap(heap[i], heap[minIndex]);
        siftDown(minIndex);
    }
}

// insert function for the Min Heap
void MinHeap::insert(const Ride &ride)
{
    if (size == heap.capacity())
    {
        throw std::overflow_error("Max capacity reached");
    }

    heap.push_back(ride);
    siftUp(size++);
}

// extractMin function for the Min Heap
Ride MinHeap::extractMin()
{
    if (isEmpty())
    {
        throw std::underflow_error("No elements in the heap");
    }

    Ride result = heap[0];
    heap[0] = heap[--size];
    heap.pop_back();
    siftDown(0);

    return result;
}

// remove function for the Min Heap
void MinHeap::remove(const Ride &ride)
{
    int i = 0;
    for (; i < size; ++i)
    {
        if (heap[i].rideNumber == ride.rideNumber)
        {
            break;
        }
    }
    if (i >= size)
    {
        return;
    }
    heap[i] = heap[--size];
    heap.pop_back();
    siftDown(i);
    siftUp(i);
}

enum Color
{
    RED,
    BLACK
};

// Red-Black Tree class for storing the rides in order of ride number
class RBTNode
{
public:
    Ride ride;
    RBTNode *left;
    RBTNode *right;
    RBTNode *parent;
    Color color;

    RBTNode(Ride ride, RBTNode *parent, RBTNode *left, RBTNode *right, Color color) : ride(ride), left(left), right(right), parent(parent), color(color) {}
};

class RBTree
{
public:
    RBTNode *root;
    MinHeap *minHeap;

    RBTree()
    {
        root = nullptr;
        minHeap = new MinHeap(100);
    }

    ~RBTree()
    {
        deleteTree(root);
        delete minHeap;
    }

    void insert(Ride ride)
    {
        RBTNode *newNode = new RBTNode(ride, nullptr, nullptr, nullptr, RED);
        insert(newNode);
        minHeap->insert(ride);
    }

    RBTNode *search(int rideNumber)
    {
        RBTNode *current = root;
        while (current && current->ride.rideNumber != rideNumber)
        {
            if (rideNumber < current->ride.rideNumber)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return current;
    }

    void printRange(int rideNumber, std::ofstream &outputFile)
    {
        RBTNode *result = search(rideNumber);
        if (result)
        {
            outputFile << result->ride.toString() << std::endl;
        }
        else
        {
            outputFile << "(0,0,0)" << std::endl;
        }
    }

    void printRange(int rideNumber1, int rideNumber2, std::ofstream &outputFile)
    {
        bool printed = false;
        if (!printRangeHelper(root, rideNumber1, rideNumber2, outputFile, printed))
        {
            outputFile << "(0,0,0)";
        }
    }

    void remove(Ride ride)
    {
        RBTNode *node = search(ride.rideNumber);
        if (node)
        {
            minHeap->remove(ride);
            removeNode(node);
        }
    }

    void removeNode(RBTNode *node)
    {
        RBTNode *y = node;
        Color yOriginalColor = y->color;
        RBTNode *x = nullptr; // Initialize x to nullptr to avoid uninitialized usage later

        // If node has no left child, replace node with its right child
        if (!node->left)
        {
            x = node->right;
            transplant(node, node->right);
        } // If node has no right child, replace node with its left child
        else if (!node->right)
        {
            x = node->left;
            transplant(node, node->left);
        } // If node has both left and right children, replace node with its successor
        else
        {
            y = minimum(node->right);
            yOriginalColor = y->color;
            x = y->right;
            // If successor is a direct child of node, replace successor with its right child
            if (y->parent == node)
            {
                if (x)
                    x->parent = y;
            }
            // If successor is not a direct child of node, replace successor with its right child and replace successor's parent with successor's right child
            else
            {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            // Replace node with successor
            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }

        // If successor is black, fix the tree
        if (yOriginalColor == BLACK)
        {
            removeFixup(x);
        }
        delete node;
    }

    void updateTrip(int rideNumber, int newTripDuration)
    {
        RBTNode *node = search(rideNumber);
        if (!node)
            return;

        int old_tripDuration = node->ride.tripDuration;

        // If new trip duration is less than or equal to old trip duration, update the trip duration and heap
        if (newTripDuration <= old_tripDuration)
        {
            minHeap->remove(node->ride);
            node->ride.tripDuration = newTripDuration;
            minHeap->insert(node->ride);
            return;
        } // If new trip duration is less than or equal to twice the old trip duration, update the trip duration, cost and heap
        else if (newTripDuration <= 2 * old_tripDuration)
        {
            minHeap->remove(node->ride);
            node->ride.tripDuration = newTripDuration;
            node->ride.rideCost += 10;
            minHeap->insert(node->ride);
        } // If new trip duration is greater than twice the old trip duration, cancel the ride
        else
        {
            minHeap->remove(node->ride);
            remove(node->ride);
        }
    }

    void cancelRide(int rideNumber)
    {
        RBTNode *node = search(rideNumber);
        if (!node)
            return;

        minHeap->remove(node->ride);
        remove(node->ride);
    }

private:
    void insert(RBTNode *newNode)
    {
        // Insert node in the tree
        RBTNode *current = root;
        RBTNode *parent = nullptr;

        while (current)
        {
            parent = current;
            if (newNode->ride.rideNumber < current->ride.rideNumber)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (!parent)
        {
            root = newNode;
        }
        else if (newNode->ride.rideNumber < parent->ride.rideNumber)
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }

        // Fix Red-Black Tree properties
        newNode->color = RED;

        insertFixup(newNode);
    }

    void insertFixup(RBTNode *node)
    {
        // If node is root, color it black and return
        while (node->parent && node->parent->color == RED)
        {
            // If parent of node is left child of grandparent of node
            if (node->parent == node->parent->parent->left)
            {
                RBTNode *uncle = node->parent->parent->right;

                // If uncle of node is red, recolor parent, uncle and grandparent of node
                if (uncle && uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } // If uncle of node is black, rotate the tree
                else
                {
                    // If node is right child of parent, rotate left at parent
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        leftRotate(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rightRotate(node->parent->parent);
                }
            } // If parent of node is right child of grandparent of node
            else
            {
                RBTNode *uncle = node->parent->parent->left;

                // If uncle of node is red, recolor parent, uncle and grandparent of node
                if (uncle && uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else // If uncle of node is black, rotate the tree
                {
                    // If node is left child of parent, rotate right at parent
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    leftRotate(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void leftRotate(RBTNode *x)
    {
        RBTNode *y = x->right;
        x->right = y->left;

        if (y->left)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (!x->parent)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBTNode *y)
    {
        RBTNode *x = y->left;
        y->left = x->right;

        if (x->right)
        {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (!y->parent)
        {
            root = x;
        }
        else if (y == y->parent->right)
        {
            y->parent->right = x;
        }
        else
        {
            y->parent->left = x;
        }

        x->right = y;
        y->parent = x;
    }

    void transplant(RBTNode *oldNode, RBTNode *newNode)
    {
        if (!oldNode->parent)
        {
            root = newNode;
        }
        else if (oldNode == oldNode->parent->left)
        {
            oldNode->parent->left = newNode;
        }
        else
        {
            oldNode->parent->right = newNode;
        }

        if (newNode)
        {
            newNode->parent = oldNode->parent;
        }
    }

    RBTNode *minimum(RBTNode *node)
    {
        while (node->left)
        {
            node = node->left;
        }
        return node;
    }

    void removeFixup(RBTNode *node)
    {
        while (node && node != root && node->color == BLACK)
        {
            if (node == node->parent->left)
            {
                RBTNode *sibling = node->parent->right;

                // If sibling is red, recolor sibling and parent of node
                if (sibling && sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }

                // If sibling is black and both children of sibling are black, recolor sibling
                if ((!sibling->left || sibling->left->color == BLACK) && (!sibling->right || sibling->right->color == BLACK))
                {
                    sibling->color = RED;
                    node = node->parent;
                } // If sibling is black and left child of sibling is red and right child of sibling is black, recolor sibling and left child of sibling
                else
                {
                    // If right child of sibling is black, recolor sibling and left child of sibling
                    if (!sibling->right || sibling->right->color == BLACK)
                    {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rightRotate(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;

                    // If right child of sibling is red, recolor right child of sibling
                    if (sibling->right)
                        sibling->right->color = BLACK;

                    // Rotate left at parent of node
                    leftRotate(node->parent);
                    node = root;
                }
            }
            else
            {
                RBTNode *sibling = node->parent->left;
                if (sibling && sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rightRotate(node->parent);
                    sibling = node->parent->left;
                }
                if ((!sibling->right || sibling->right->color == BLACK) && (!sibling->left || sibling->left->color == BLACK))
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (!sibling->left || sibling->left->color == BLACK)
                    {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        leftRotate(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    rightRotate(node->parent);
                    node = root;
                }
            }
        }
        if (node)
            node->color = BLACK;
    }

    void deleteTree(RBTNode *node)
    {
        if (!node)
        {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    bool printRangeHelper(RBTNode *node, int rideNumber1, int rideNumber2, std::ofstream &outputFile, bool &printed)
    {
        if (!node)
        {
            return false;
        }

        bool foundInRange = false;
        if (rideNumber1 < node->ride.rideNumber)
        {
            foundInRange |= printRangeHelper(node->left, rideNumber1, rideNumber2, outputFile, printed);
        }
        if (rideNumber1 <= node->ride.rideNumber && node->ride.rideNumber <= rideNumber2)
        {
            if (printed)
            {
                outputFile << ",";
            }
            printed = true;
            foundInRange = true;
            outputFile << node->ride.toString();
        }
        if (node->ride.rideNumber < rideNumber2)
        {
            foundInRange |= printRangeHelper(node->right, rideNumber1, rideNumber2, outputFile, printed);
        }
        return foundInRange;
    }
};

// Main function
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " file_name" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream inputFile(fileName);
    std::ofstream outputFile("output_file.txt");

    if (!inputFile.is_open())
    {
        std::cout << "Error opening file: " << fileName << std::endl;
        return 1;
    }

    RBTree rbt;

    std::string line;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string operation;
        std::getline(iss, operation, '(');

        if (operation == "Insert")
        {
            int rideNumber, rideCost, tripDuration;
            char discard;
            iss >> rideNumber >> discard >> rideCost >> discard >> tripDuration >> discard;
            Ride ride(rideNumber, rideCost, tripDuration);

            if (rbt.search(rideNumber))
            {
                outputFile << "Duplicate RideNumber";
                break;
            }

            rbt.insert(ride);
        }
        else if (operation == "Print")
        {
            int rideNumber1, rideNumber2;
            char discard2;
            std::string restOfLine;
            iss >> rideNumber1 >> restOfLine;
            std::istringstream restOfLineStream(restOfLine);
            if (restOfLineStream >> discard2 && discard2 == ',')
            {
                restOfLineStream >> rideNumber2;
                rbt.printRange(rideNumber1, rideNumber2, outputFile);
                outputFile << std::endl;
            }
            else
            {
                rbt.printRange(rideNumber1, outputFile);
            }
        }
        else if (operation == "UpdateTrip")
        {
            int rideNumber, newTripDuration;
            char discard;
            iss >> rideNumber >> discard >> newTripDuration >> discard;
            rbt.updateTrip(rideNumber, newTripDuration);
        }
        else if (operation == "GetNextRide")
        {
            if (rbt.minHeap->isEmpty())
            {
                outputFile << "No active ride requests" << std::endl;
            }
            else
            {
                Ride nextRide = rbt.minHeap->extractMin();
                rbt.remove(nextRide);
                outputFile << "(" << nextRide.rideNumber << "," << nextRide.rideCost << "," << nextRide.tripDuration << ")" << std::endl;
            }
        }
        else if (operation == "CancelRide")
        {
            int rideNumber;
            char discard;
            iss >> rideNumber >> discard;

            RBTNode *node = rbt.search(rideNumber);
            if (node)
            {

                rbt.remove(node->ride);
            }
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
