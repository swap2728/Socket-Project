#include <iostream>
#include <bits/stdc++.h>
#include "NodeandRoot.h"
using namespace std;

class Calculator
{
public:
};

class A : public Calculator
{
public:
    virtual void add(double num1, double num2) = 0;
    virtual void subtract(double num1, double num2) = 0;
    virtual void multiply(double num1, double num2) = 0;
    virtual void divide(double num1, double num2) = 0;
};

class B : public Calculator
{
public:
    virtual Node* add(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual Node* subtract(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual Node* multiply(const Root &num1, const Root &num2,int cnt=0) = 0;
    virtual void divide(const Root &num1, const Root &num2) = 0;
};

// BasicCalculator implementation (for simple numeric operations)
class BasicCalculator : public A
{
public:
    void add(double a, double b) override
    {
        cout << "Addition: " << (a + b) << endl;
    }

    void subtract(double a, double b) override
    {
        cout << "Subtraction: " << (a - b) << endl;
    }

    void multiply(double a, double b) override
    {
        cout << "Multiplication: " << (a * b) << endl;
    }

    void divide(double a, double b) override
    {
        if (b != 0)
            cout << "Division: " << (a / b) << endl;
        else
            cout << "Error: Division by zero!" << endl;
    }
};

// SuperCalculator class for linked list-based arithmetic
class SuperCalculator : public B
{
public:
    Node* add(const Root &num1, const Root &num2 , int cnt=0) override
    {
        Root A(num1.head);
        Root B(num2.head);
        // Root* node2 = new Root(num2.head);
        A.reverseList();
        B.reverseList();
        Node *node1 = A.head;
        Node *node2 = B.head;
        // Root C(nullptr) ;
        Node *result = nullptr;
        Node *tail = nullptr;
        int carry = 0;

        // Traverse both lists
        while (node1 || node2 || carry)
        {
            int sum = carry; // Start with carry

            if (node1)
            {
                sum += node1->value; // Add value of first list node if available
                node1 = node1->next; // Move to next node in the first list
            }

            if (node2)
            {
                sum += node2->value; // Add value of second list node if available
                node2 = node2->next; // Move to next node in the second list
            }

            // Calculate new digit and carry
            carry = sum / 10;
            int digit = sum % 10;

            // Create new node with the resulting digit
            Node *newNode = new Node(digit);
            if (!result)
            {
                result = tail = newNode; // Initialize result list with the first node
            }
            else
            {
                tail->next = newNode; // Add node to the end of result list
                tail = newNode;       // Move tail pointer
            }
        }
        A.reverseList();
        B.reverseList();
        Root C(result);
        C.reverseList();


        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"Addition Result : ";
        Node* fileptr = C.head;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();


        cout << "Addition Result: ";
        printList(C.head);
        }

        
        // printList(C.head); // Helper function to print the linked list

        

        return C.head;
    }

    Node *reverse(Node *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }
        Node *curr = head;
        Node *prev = NULL;
        while (curr != NULL)
        {
            Node *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }

    Node* subtract(const Root &num1, const Root &num2,int cnt=0) override
    {
        Node *head1 = num1.head;
        Node *head2 = num2.head;
        // If head2 is NULL, return head1 as is.
        if (head2 == NULL)
        {
            printList(head1);
        }

        // If head1 is NULL, return head2 but with a negation in its value.
        if (head1 == NULL)
        {
            head2->value = -head2->value;
            printList(head2);
        }

        // Reverse both linked lists to start from the least significant digit.
        head1 = reverse(head1);
        head2 = reverse(head2);

        // Dummy node for the result linked list.
        Node *head = new Node(-1);
        Node *itr = head;

        Node *c1 = head1;
        Node *c2 = head2;
        int borrow = 0;

        // Subtract digits one by one.
        while (c1 != NULL)
        {
            int diff = borrow + c1->value - (c2 != NULL ? c2->value : 0);

            // Handle the borrow if the diff is negative.
            if (diff < 0)
            {
                borrow = -1;
                diff += 10;
            }
            else
            {
                borrow = 0;
            }

            // Add the current diff to the result linked list.
            itr->next = new Node(diff);
            itr = itr->next;
            c1 = c1->next;
            if (c2 != NULL)
            {
                c2 = c2->next;
            }
        }
        Node *result = reverse(head->next);
        while (result != NULL && result->value == 0)
        {
            result = result->next;
        }

        if (result == NULL)
        {
            printList(new Node(0));
        }
        
        

        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"subtraction Result : ";
        Node* fileptr = result;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();


        cout << "subtraction Result: ";
        printList(result);
        }

        return result;
    }

    Node* multiply(const Root &num1, const Root &num2 , int cnt=0) override
    {
        struct Node *result(0) ;
        printList(num1.head);
        Node* first = num1.head;
        Node* second = num2.head;
        first=reverse(first);
        second=reverse(second);
        
        struct Node *second_ptr = second,
                    *result_ptr1 = result, *first_ptr;
        int q = 0;
        while (second_ptr)
        {

            int carry = 0;

            Node* temp=new Node(0);
            Node* result_ptr2=temp;
            for(int i=0;i<q;i++){
                Node* p=new Node(0);
                result_ptr2->next=p;
                // if(i==cnt-1)break;
                result_ptr2=result_ptr2->next;
            }
            // Node* p=new Node(0);
            // result_ptr2->next=p;
            first_ptr = first;

            while (first_ptr)
            {    
                // cout<<first_ptr->value<<" "<<second_ptr->value<<endl;

                int mul = (first_ptr->value * second_ptr->value) + carry;
                Node* p = new Node(mul%10);
                result_ptr2->next=p;
                carry = (mul / 10);
                first_ptr = first_ptr->next;
                result_ptr2 = result_ptr2->next;
                // cnt++;
                
            }
            if (carry > 0)
            {   
                Node* p=new Node(carry);
                result_ptr2->next = p;
                result_ptr2 = result_ptr2->next;
            }
            temp=reverse(temp->next);
            // printList(temp);
            second_ptr=second_ptr->next;
            q++;
            result = add(result,temp);
        }

        if(cnt){
             ofstream obj;

        obj.open(file,ios::app);

        obj<<"multiply Result : ";
        Node* fileptr = result;
        while(fileptr){
            obj<<fileptr->value<<" ";
            fileptr=fileptr->next;
        }
        obj<<endl;
        obj.close();
        cout<<"multiply Result : ";
        printList(result);
        }

        
        return result;
    }

    void divide(const Root &num1, const Root &num2) override
    {
        int num1Value = listToNumber(num1.head);
        int num2Value = listToNumber(num2.head);

        if (num2Value == 0)
        {
            cout << "Error: Division by zero!" << endl;
            return;
        }

        int quotient = num1Value / num2Value;
        cout << "Division Result: ";
        printNumberAsList(quotient);
    }

private:
    void printList(Node *head) const
    {
        while (head)
        {
            cout << head->value;
            head = head->next;
        }
        cout << endl;
    }

    int listToNumber(Node *head) const
    {
        int num = 0;
        while (head)
        {
            num = num * 10 + head->value;
            head = head->next;
        }
        return num;
    }

    void printNumberAsList(int num) const
    {
        if (num == 0)
        {
            cout << "0" << endl;
            return;
        }

        Node *result = nullptr;
        Node *tail = nullptr;

        while (num)
        {
            int digit = num % 10;
            Node *newNode = new Node(digit);
            if (!result)
            {
                result = tail = newNode;
            }
            else
            {
                newNode->next = result;
                result = newNode;
            }
            num /= 10;
        }

        printList(result);
    }
};

void showHistory(){
    fstream obj;
    obj.open(file);
    while(obj){
        string p;
        getline(obj,p);
        cout<<p<<endl;
    }
    obj.close();
}

