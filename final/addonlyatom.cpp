class AddOnlySet {
private:
 std::atomic_int end_index;
 std::atomic_int* elements;
public:
    AddOnlySet(int s) {
        end_index = 0;
        elements = new std::atomic_int[s];
        for (int i = 0; i < s; i++) {
            elements[i].store(-1);
        }
    }
    bool contains(int check) {
        for (int i = 0; i < end_index.load(); i++) {
            if (elements[i].load() == check) {
                return true;
            }
        }
        return false;
    }
    void add(int to_add) {
        bool complete = false;
        int check = -1;
        while(!complete){
            if (contains(to_add)) {
                return;
            }
            else {
                check = -1;
                //compare: does elements[end_index] = -1?
                //if so, swap: elements[end_index] = to_add
                //if not, check = elements[end_index]
                complete = std::atomic_compare_exchange_strong(&elements[end_index], &check, to_add);
                //if success, increment end_index
                if(complete)end_index.store(end_index.load()+1);
            }
        }
    }
};