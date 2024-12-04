#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Node for the linked list

string file = "history.txt";

struct Node
{
    int value;
    Node *next;
    Node(int val) : value(val), next(nullptr) {}
};

// Root class to handle input and linked list creation
class Root
{
public:
    Node *head;

    Root(Node *root)
    {
        head = root;
    }

    // Constructor to create linked list from string
    Root(const string &num) : head(nullptr)
    {
        Node *tail = nullptr;

        for (char c : num)
        {
            Node *newNode = new Node(c - '0');
            if (!head)
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    // Print the linked list
    void printList() const
    {
        Node *current = head;
        while (current)
        {
            cout << current->value;
            current = current->next;
        }
        cout << endl;
    }

    void reverseList()
    {
        // Node*& head
        Node *prev = nullptr; // To keep track of the previous node
        Node *curr = head;    // The current node being processed
        Node *next = nullptr; // To keep track of the next node

        // Traverse the linked list and reverse the direction of the next pointers
        while (curr != nullptr)
        {
            next = curr->next; // Save the next node
            curr->next = prev; // Reverse the direction of the current node
            prev = curr;       // Move the previous node to the current node
            curr = next;       // Move to the next node in the original list
        }

        head = prev; // After the loop, prev will point to the new head of the reversed list
    }
};