#ifndef RANKAVLTREE
#define RANKAVLTREE

#include <math.h>
#include <iostream>

using namespace std;

template <class T>
class RankAVLTree {
    public:
    class Node{
        public:
            T* value;
            int height;
            int rank;
            Node* left;
            Node* right;

            Node(T* value){
                this->value = value;
                this->height = 1;
                this->rank = 1;
                this->left = nullptr;
                this->right = nullptr;
            }

            ~Node() = default;
    };

    Node* root;

    Node* getRoot(){
        return root;
    }

    void delete_hollow_tree(){
        delete_hollow_tree(this->root);
    }

    void delete_hollow_tree(Node* head){
        if(head == nullptr){
            return;
        }
        delete_hollow_tree(head->left);
        delete_hollow_tree(head->right);
        delete head;
    }

    void delete_values()
    {
        delete_values(root);
    }

    void insertValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        this->root = insert(this->root, value, cmp_func);
    }

    void removeValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        this->root = remove(this->root, value, cmp_func);
    }

    T* searchValue(int rank){
        return search(this->root, rank + 1);
    }

    Node* findValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        return find(root, value, cmp_func);
    }

    Node* findDad(T* value, int(*cmp_func)(T* t1, T* t2)){
        return findDad(root, value, cmp_func);
    }

    void doOnTreeInOrder(void(*func)(T* t)){
        doOnTreeInOrder(root, func);
    }

    void intoArray(T** array){
        int index = 0;
        if(this->root != 0){
            intoArray(root, array, &index);
        }
    }

    int countInRange(T* minValue, T* maxValue, int(*cmp_func)(T* t1, T* t2)){
        return countInRange(this->root, minValue, maxValue, cmp_func);
    }

    void rangedIntoArray(T** array, T* minValue, T* maxValue, int(*cmp_func)(T* t1, T* t2)){
        int index = 0;
        if(this->root != 0){
            rangedIntoArray(root, array, &index, minValue, maxValue, cmp_func);
        }
    }

    void arrayToAVLTree(T** array, int size){
        this->root = arrayToAVLTree(array, 0, size - 1);
        setHeight(this->root);
    }

    private: 

    Node* arrayToAVLTree(T** array, int start, int end){
        if(start > end) return nullptr;

        int mid = (start + end)/2;
        Node* root = new Node(array[mid]);

        root->left = arrayToAVLTree(array, start, mid - 1);
        root->right = arrayToAVLTree(array, mid + 1, end);

        return root;
    }

    int setHeight(Node* head){
        if(head == nullptr) return 0;

        int leftHeight = setHeight(head->left);
        int rightHeight = setHeight(head->right);
        head->height = max(leftHeight, rightHeight) + 1;
        return head->height;
    }

    int countInRange(Node* head, T* minValue, T* maxValue, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return 0;

        int sumLeft = 0;
        int sumRight = 0;
        int isInRange = 0;

        int minComare = cmp_func(head->value, minValue);
        int maxCompare = cmp_func(head->value, maxValue);

        if(minComare > 0) sumLeft = countInRange(head->left, minValue, maxValue, cmp_func);
        if(minComare >= 0 && maxCompare <= 0) isInRange = 1;
        if(maxCompare < 0) sumRight = countInRange(head->right, minValue, maxValue, cmp_func);

        return sumLeft + isInRange + sumRight;
    }

    void rangedIntoArray(Node* head, T** array, int* index, T* minValue, T* maxValue, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return;

        int minComare = cmp_func(head->value, minValue);
        int maxCompare = cmp_func(head->value, maxValue);

        if(minComare > 0) rangedIntoArray(head->left, array, index, minValue, maxValue, cmp_func);

        if(minComare >= 0 && maxCompare <= 0)
        {
            array[*index] = head->value;
            (*index)++;
        }

        if(maxCompare < 0) rangedIntoArray(head->right, array, index, minValue, maxValue, cmp_func);
    }

    int height(Node* head){
        if(head == nullptr) return 0;
        return head->height;
    }

    void updateRank(Node* head){
        if(head == nullptr) return;
        head->rank = 1;
        if(head->left != nullptr) head->rank += head->left->rank;
        if(head->right != nullptr) head->rank += head->right->rank;
    }

    Node* rotateRight(Node* head){
        Node* newHead = head->left;
        head->left = newHead->right;
        newHead->right = head;

        head->height = max(height(head->left), height(head->right)) + 1;
        newHead->height = max(height(newHead->left), height(newHead->right)) + 1;

        head->rank = 1;
        if(head->left != nullptr) head->rank += head->left->rank;
        if(head->right != nullptr) head->rank += head->right->rank;

        updateRank(head);
        updateRank(newHead);

        return newHead;
    }

    Node* rotateLeft(Node* head){
        Node* newHead = head->right;
        head->right = newHead->left;
        newHead->left = head;

        head->height = max(height(head->left), height(head->right)) + 1;
        newHead->height = max(height(newHead->left), height(newHead->right)) + 1;

        updateRank(head);
        updateRank(newHead);

        return newHead;
    }
    

    Node * insert(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr){
            Node* temp = new Node(value);
            return temp;
        }
    
        int compare_result = cmp_func(value, head->value);

        if(compare_result < 0){
            head->left = insert(head->left, value, cmp_func);
        }
        else if(compare_result > 0){
            head->right = insert(head->right, value, cmp_func);
        }

        head->height = max(height(head->left), height(head->right)) + 1;

        int balance = height(head->left) - height(head->right);
    
        if(balance > 1){
            if(cmp_func(value, head->left->value) < 0){
                return rotateRight(head);
            }
            else{
                head->left = rotateLeft(head->left);
                return rotateRight(head);
            }
        }
        else if(balance < -1){
            if(cmp_func(value, head->right->value) > 0){
                return rotateLeft(head);
            }
            else{
                head->right = rotateRight(head->right);
                return rotateLeft(head);
            }
        }

        updateRank(head);
        return head;
    }

    Node* remove(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;
    
        int compare_result = cmp_func(value, head->value);
        
        if(compare_result < 0){
            head->left = remove(head->left, value, cmp_func);
        }
        else if(compare_result > 0){
            head->right = remove(head->right, value, cmp_func);
        }
        else{
            Node * temp_left = head->left;
            Node * temp_right = head->right;

            if(temp_left == nullptr){
                delete head;
                head = temp_right;
            }
            else if(temp_right == nullptr){
                delete head;
                head = temp_left;
            }
            else{
                while(temp_left->right != nullptr){
                    temp_left = temp_left->right;
                }
                head->value = temp_left->value;
                head->left = remove(head->left, temp_left->value, cmp_func);
            }
        }

        if(head == nullptr) return nullptr;

        head->height = max(height(head->left), height(head->right)) + 1;

        int balance = height(head->left) - height(head->right);

        if(balance > 1){
            if(height(head->left) >= height(head->right)){
                return rotateRight(head);
            }
            else{
                head->left = rotateLeft(head->left);
                return rotateRight(head);
            }
        }
        else if(balance < -1){
            if(height(head->right) >= height(head->left)){
                return rotateLeft(head);
            }
            else{
                head->right = rotateRight(head->right);
                return rotateLeft(head);
            }
        }

        updateRank(head);
        return head;
    }

    Node* find(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;

        int compare_result = cmp_func(value, head->value);
    
        if(compare_result == 0){
            return head;
        }
        else if(compare_result < 0){
            return find(head->left, value, cmp_func);
        }
        else{
            return find(head->right, value, cmp_func);
        }
    }

    Node* findDad(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;

        int compare_result = cmp_func(value, head->value);
        Node* temp;
    
        if(compare_result == 0){
            return nullptr;
        }

        else if(compare_result < 0){
            temp = findDad(head->left, value, cmp_func);
            if(temp == nullptr) return head;
            return temp;
        }
        else{
            temp = findDad(head->right, value, cmp_func);
            if(temp == nullptr) return head;
            return temp;
        }
    }

    void doOnTreeInOrder(Node* head, void(*func)(T* t)){
        if(head == nullptr) return;

        doOnTreeInOrder(head->left, func);
        func(head->value);
        doOnTreeInOrder(head->right, func);
    }

    void intoArray(Node* head, T** array, int *index){
        if(head == nullptr) return;
        
        intoArray(head->left, array, index);

        array[*index] = head->value;
        (*index)++;

        intoArray(head->right, array, index);
    }
 
    void delete_values(Node* head){
        if(head == nullptr){
            return;
        }
        delete_values(head->left);
        delete_values(head->right);
        delete head->value;
        head->value = nullptr;
    }

    T* search(Node* head, int rank){
        if(head == nullptr) return nullptr;

        int left_rank = 0;
        if(head->left != nullptr) left_rank = head->left->rank;

        if(rank == left_rank + 1){
            return head->value;
        }
        else if(rank <= left_rank){
            return search(head->left, rank);
        }
        else{
            return search(head->right, rank - left_rank - 1);
        }
    }
};



#endif //RANKAVLTREE