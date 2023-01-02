

template <class T>
class HashTable {
    private:
    int itemCounter;
    int size;
    double loadFactor;
    
    public:
    class Node {
        public:
        T* data;
        Node* next;
        Node(T* data, Node* next){
            this->data = data;
            this->next = next;
        }
    };

    Node** table;

    HashTable(){
        this->itemCounter = 0;
        this->size = 100;
        this->loadFactor = 0;
        this->table = new Node*[this->size];
        for(int i = 0; i < this->size; i++){
            this->table[i] = nullptr;
        }
    }

    ~HashTable(){
        Node* node;
        Node* tempNode;
        for(int i = 0; i < this->size; i++){
            node = this->table[i];
            while(node != nullptr){
                tempNode = node;
                node = node->next;
                delete tempNode->data;
                delete tempNode;
            }
        }
        delete[] this->table;
    }

    void insert(T* item){
        int hashRes = hash(item);
        Node* node = new Node(item, this->table[hashRes]);
        this->table[hashRes] = node;
        this->itemCounter++;
        this->loadFactor = double(this->itemCounter) / this->size;
        if(this->loadFactor > 0.75){
            this->resize();
        }
    }

    int hash(T* item){
        return item->getId() % this->size;
    }

    void resize(){
        int newSize = this->size * 2;
        this->size = newSize;
        Node** newTable = new Node*[newSize];
        for(int i = 0; i < newSize; i++){
            newTable[i] = nullptr;
        }
        int hashRes;
        Node* node;
        Node* tempNode;
        for(int i = 0; i < this->size/2; i++){
            node = this->table[i];
            while(node != nullptr){
                hashRes = hash(node->data);
                Node* newNode = new Node(node->data, newTable[hashRes]);
                newTable[hashRes] = newNode;
                tempNode = node;
                node = node->next;
                delete tempNode;
            }
        }
        delete[] this->table;
        this->table = newTable;
        this->loadFactor = double(this->itemCounter) / this->size;
    }

    void remove(T* item){
        int hashRes = hash(item);
        Node* node = this->table[hashRes];
        Node* prevNode = nullptr;
        while(node != nullptr){
            if(node->data->getId() == item->getId()){
                if(prevNode == nullptr){
                    this->table[hashRes] = node->next;
                } else {
                    prevNode->next = node->next;
                }
                delete node->data;
                delete node;
                this->itemCounter--;
                this->loadFactor = double(this->itemCounter) / this->size;
                return;
            }
            prevNode = node;
            node = node->next;
        }
    }

    T* search(T* item){
        int hashRes = hash(item);
        Node* node = this->table[hashRes];
        while(node != nullptr){
            if(node->data->getId() == item->getId()){
                return node->data;
            }
            node = node->next;
        }
        return nullptr;
    }
};

