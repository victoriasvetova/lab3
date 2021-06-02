#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

template<typename T>
class tree_elem {
public:
    T DATA;
    tree_elem* M_LEFT;
    tree_elem* M_RIGHT;

    tree_elem(T key) {//����������� ��� �������� ������ ����
        DATA = key;
        M_LEFT = NULL;
        M_RIGHT = NULL;
    }
    tree_elem(T key, tree_elem<T>* left, tree_elem<T>* right) {//����������� ��� ����������� ����������� ����
        DATA = key;
        M_LEFT = left;
        M_RIGHT = right;
    }
};

template<typename T>
void sqr(tree_elem<T>* curr) {//(��� Map)
    curr->DATA = curr->DATA * curr->DATA;
}

template<typename T>
bool is_more_than_25(tree_elem<T>* curr) {//��� �������� ���� ��������� ������� 25 (��� Where)
    if (curr->DATA >= 25)
        return true;
    return false;
}

template<typename T>
T sum(tree_elem<T>* curr, T curr_sum) {//(��� Reduce)
    return curr_sum + curr->DATA;
}

template<typename T>
class binary_tree {
private:

    tree_elem<T>* m_root;

    int size;

    void MapRecursivelyFunction(void(*func)(tree_elem<T>*), tree_elem<T>* curr) {//��� ������������ ���������� Map
        if (curr) {
            MapRecursivelyFunction(func, curr->M_LEFT);
            func(curr);
            MapRecursivelyFunction(func, curr->M_RIGHT);
        }
    }

    void WhereRecursivelyFunction(bool(*func)(tree_elem<T>*), tree_elem<T>* curr) {//��� ������������ ���������� Where
        if (curr) {
            WhereRecursivelyFunction(func, curr->M_LEFT);

            if (func(curr)) {
                delete_elem(curr->DATA);
            }

            WhereRecursivelyFunction(func, curr->M_RIGHT);
        }
    }

    void ReduceRecursivelyFunction(T(*func)(tree_elem<T>*, T), tree_elem<T>* curr, T& curr_sum) {//��� ������������ ���������� Reduce
        if (curr) {
            ReduceRecursivelyFunction(func, curr->M_LEFT, curr_sum);
            curr_sum = func(curr, curr_sum);
            ReduceRecursivelyFunction(func, curr->M_RIGHT, curr_sum);
        }
    }

    void upgrade_sub_tree(binary_tree<T>* result, tree_elem<T>* curr) {//��� ������������ ���������� ������ ����������
        if (curr) {
            upgrade_sub_tree(result, curr->M_LEFT);
            result->insert(curr->DATA);
            upgrade_sub_tree(result, curr->M_RIGHT);
        }
    }

    void tree_to_stringRecursivelyFunction(tree_elem<T>* curr, std::vector<std::string>& curr_str) {
        if (curr) {
            tree_to_stringRecursivelyFunction(curr->M_LEFT, curr_str);
            curr_str.push_back(std::to_string(curr->DATA));
            tree_to_stringRecursivelyFunction(curr->M_RIGHT, curr_str);
        }
    }

    void print_tree(tree_elem<T>* curr) {
        if (curr) {
            print_tree(curr->M_LEFT);
            std::cout << curr->DATA << " ";
            print_tree(curr->M_RIGHT);
        }
    }
public:

    binary_tree() {
        m_root = nullptr;
        size = 0;
    }


    //����������� � ����������� �����
    binary_tree(int key) {
        m_root = new tree_elem<T>(key);
        size = 1;
    }
    ~binary_tree() {
        delete_tree(m_root);
    }
    int get_size() { return size; }

    //================================================================================
    //��������� ��������� �� ����
    binary_tree<T>* get_sub_tree(T key) {

        tree_elem<T>* curr = m_root;
        while (curr && curr->DATA != key)
            if (curr->DATA < key)
                curr = curr->M_RIGHT;
            else
                curr = curr->M_LEFT;
        binary_tree<T>* result = new binary_tree<T>(curr->DATA);
        upgrade_sub_tree(result, curr);

        return result;
    }
    //================================================================================
    int height(tree_elem<T>* curr)//������ ��������� ����������� ���� curr
    {
        int h = 0;

        if (curr != NULL)
        {
            int l_height = height(curr->M_LEFT);
            int r_height = height(curr->M_RIGHT);
            int max_height = std::max(l_height, r_height);
            h = max_height + 1;
        }
        return h;
    }
    //================================================================================
    int diff(tree_elem<T>* curr)//������� ����� ����������� ���� curr
    {
        int l_height = height(curr->M_LEFT);
        int r_height = height(curr->M_RIGHT);
        int b_factor = l_height - r_height;
        return b_factor;
    }
    //================================================================================
    tree_elem<T>* ll_rotation(tree_elem<T>* curr)
    {
        tree_elem<T>* temp;
        temp = curr->M_LEFT;
        curr->M_LEFT = temp->M_RIGHT;
        temp->M_RIGHT = curr;
        return temp;
    }
    //================================================================================
    tree_elem<T>* rr_rotation(tree_elem<T>* curr)
    {
        tree_elem<T>* temp;
        temp = curr->M_RIGHT;
        curr->M_RIGHT = temp->M_LEFT;
        temp->M_LEFT = curr;
        return temp;
    }
    //================================================================================
    tree_elem<T>* lr_rotation(tree_elem<T>* curr)
    {
        tree_elem<T>* temp;
        temp = curr->M_LEFT;
        curr->M_LEFT = rr_rotation(temp);
        return ll_rotation(curr);
    }
    //================================================================================
    tree_elem<T>* rl_rotation(tree_elem<T>* curr)
    {
        tree_elem<T>* temp;
        temp = curr->M_RIGHT;
        curr->M_RIGHT = ll_rotation(temp);
        return rr_rotation(curr);
    }
    //================================================================================
    tree_elem<T>* balance(tree_elem<T>* temp)
    {
        int bal_factor = diff(temp);
        if (bal_factor > 1)
        {
            if (diff(temp->M_LEFT) > 0)
                temp = ll_rotation(temp);
            else
                temp = lr_rotation(temp);
        }
        else if (bal_factor < -1)
        {
            if (diff(temp->M_RIGHT) > 0)
                temp = rl_rotation(temp);
            else
                temp = rr_rotation(temp);
        }
        return temp;
    }

    int get_diff() {
        return diff(m_root);
    }
    //================================================================================
    //������ ������
    void print() {
        print_tree(m_root);
        std::cout << std::endl;
    }
    //================================================================================
    //�������� ������
    void delete_tree(tree_elem<T>* curr) {
        if (curr) {
            delete_tree(curr->M_LEFT);
            delete[] curr;
            delete_tree(curr->M_RIGHT);
        }
    }


    //����� �������� � ������
    bool find(int key) {//����� ��������

        tree_elem<T>* curr = m_root;
        while (curr && curr->DATA != key) {
            if (curr->DATA > key)
                curr = curr->M_LEFT;
            else
                curr = curr->M_RIGHT;
        }
        return (bool)curr;
    }

    //���������� ��������
    void insert(int key) {//���������� ������ �������� � ������
        if (m_root == nullptr) {
            m_root = new tree_elem<T>(key);
            size = 1;
        }
        else {
            tree_elem<T>* curr = m_root;
            while (curr && curr->DATA != key) {
                if (curr->DATA > key) {//������� � ����� ���������
                    if (curr->M_LEFT)//���� ���� �����������,�� ��� ��� ����������� �������� ������ ����,
                        //��� � ���� curr,
                        //�� �� ���������� � ����� ��������� ��� ���(������� ��� ��������� ��������)
                        curr = curr->M_LEFT;
                    else {//���� �� ����������� ���������� ��� ���� ���,�� �� ��������� ����� ������� � ������
                        //(��� ��� �� ��� ��������� �� ��� �����,���� ����� �������� ����� �������)
                        curr->M_LEFT = new tree_elem<T>(key);

                        m_root = balance(m_root);

                        size++;
                        return;
                    }
                }//��� ������� ��������� ����������
                else {//������� � ������ ���������
                    if (curr->M_RIGHT)
                        curr = curr->M_RIGHT;
                    else {
                        curr->M_RIGHT = new tree_elem<T>(key);

                        m_root = balance(m_root);//��������� ������ ����� ����� ������������
                        //----------------------------------------------------------------------------------------------
                        //(����� ������������ ������� � ���,����� ��� ����������� ���� ������,��� ������� ���� ��������
                        //���� �� ����������� ��������� - ������� ����� ����������� ���� ���� �� ����� 1)
                        //----------------------------------------------------------------------------------------------
                        //� ������,��� ���������� ������ �������� �������,����� �� ����������� ������ ����� ����� ������
                        //�� ����� ������ �����,��� ���������������� ������ ��� ������� �����
                        //������ ll_rotation,rl_rotation,lr_rotation,rr_rotation - ������ �� ������� ������������ ��� �������
                        //"��������" ������ ��������� ������ ���������� �������� �����,
                        //"��������" ������� ��������� ������ ���������� �������� �����,
                        //"��������" ������ ��������� ������� ���������� �������� �����,
                        //"��������" ������� ��������� ������� ���������� �������� �����,

                        size++;
                        return;
                    }
                }
            }
        }
    }

    //�������� ��������
    void delete_elem(T key) {
        tree_elem<T>* curr = m_root;
        tree_elem<T>* prev = nullptr;
        //����� ���������� ��������
        while (curr && curr->DATA != key) {
            prev = curr;//���������� �������� curr,������� ��������� �� ��������� ��������
            if (curr->DATA > key) {
                curr = curr->M_LEFT;
            }
            else
                curr = curr->M_RIGHT;
        }
        if (!curr)//�������� �� ��,���������� �� ��� �������,������� �� ����� �������
            return;


        if (prev->M_RIGHT == curr) {//���� ��������� ������� �������� ������ �����������
            if (curr->M_RIGHT == nullptr && curr->M_LEFT == nullptr) {//���� ��������� ������� ������ �� ����� �����������
                prev->M_RIGHT = nullptr;
                delete[] curr;
                size--;
                return;
            }
            if (curr->M_LEFT == nullptr) {//���� � ���������� �������� ��� ������ ���������
                prev->M_RIGHT = curr->M_RIGHT;
                delete[] curr;
                size--;
                return;
            }
            if (curr->M_RIGHT == nullptr) {//���� � ���������� �������� ��� ������� ���������
                prev->M_RIGHT = curr->M_LEFT;
                delete[] curr;
                size--;
                return;
            }
        }

        if (prev->M_LEFT == curr) {//���� ��������� ������� �������� ����� �����������
            if (curr->M_RIGHT == nullptr && curr->M_LEFT == nullptr) {//���� ��������� ������� ������ �� ����� �����������
                prev->M_LEFT = nullptr;
                delete[] curr;
                size--;
                return;
            }
            if (curr->M_LEFT == nullptr) {//���� � ���������� �������� ��� ������ ���������
                prev->M_LEFT = curr->M_RIGHT;
                delete[] curr;
                size--;
                return;
            }
            if (curr->M_RIGHT == nullptr) {//���� � ���������� �������� ��� ������� ���������
                prev->M_LEFT = curr->M_LEFT;
                delete[] curr;
                size--;
                return;
            }
        }

        //���� � ���������� �������� ���� ��� ���������,�� �� ����� ����������
        //�� ������ ��������� ����������� ������� �� ��� ������� ���������

        tree_elem<T>* tmp = curr->M_RIGHT;//��������� ���������� ��� ������ �������� � ������ ���������
        while (tmp->M_LEFT)
            tmp = tmp->M_LEFT;

        T minimum = tmp->DATA;//���������� �������� ���������� tmp ��� ������������ ��������



        if (prev->M_LEFT == curr) {//���� ��������� ������� �������� ����� �����������
            delete_elem(tmp->DATA);//�������� ������������ �������� � ������ ���������
            prev->M_LEFT = new tree_elem<T>(minimum, curr->M_LEFT, curr->M_RIGHT);
            delete[] curr;
            size--;
            return;
        }
        if (prev->M_RIGHT == curr) {//���� ��������� ������� �������� ������ �����������
            delete_elem(tmp->DATA);//�������� ������������ �������� � ������ ���������
            prev->M_RIGHT = new tree_elem<T>(minimum, curr->M_LEFT, curr->M_RIGHT);
            delete[] curr;
            size--;
            return;
        }
    }

    //map,where,reduce
    void Map(void(*func)(tree_elem<T>*)) {//���������� ������ curr,������� ������� ��������������� �������
        tree_elem<T>* curr = m_root;
        MapRecursivelyFunction(func, curr);
    }

    void Where(bool(*func)(tree_elem<T>*)) {//���������� ������ curr,������� ������� ��������������� �������
        tree_elem<T>* curr = m_root;
        WhereRecursivelyFunction(func, curr);
    }

    T Reduce(T(*func)(tree_elem<T>*, T)) {
        tree_elem<T>* curr = m_root;
        T curr_sum = 0;
        ReduceRecursivelyFunction(func, curr, curr_sum);
        return curr_sum;
    }
    void is_balanced() {
        std::vector<int> h;
        curr_balance(m_root, h);
        for (int i = 0; i < size; ++i) {
            std::cout << h[i] << " ";
        }
        std::cout << std::endl;
    }
    void curr_balance(tree_elem<T>* curr, std::vector<int>& h) {
        if (curr) {
            curr_balance(curr->M_LEFT, h);
            h.push_back(diff(curr));
            curr_balance(curr->M_RIGHT, h);
        }

    }
    //==============================================
    //�������������� ��� ������� �������� ������������ ��� ������������
    std::string is_balanced_str() {
        std::vector<std::string> h;
        std::string res = "";
        curr_balance_str(m_root, h);
        for (int i = 0; i < size; ++i) {
            res += h[i] + " ";
        }
        return res;
    }
    void curr_balance_str(tree_elem<T>* curr, std::vector<std::string>& h) {
        if (curr) {
            curr_balance_str(curr->M_LEFT, h);
            h.push_back(std::to_string(diff(curr)));
            curr_balance_str(curr->M_RIGHT, h);
        }

    }
    //==============================================
    //������������� ������ � ���� ������
    std::string tree_to_string() {

        tree_elem<T>* curr = m_root;
        std::vector<std::string> result;
        tree_to_stringRecursivelyFunction(curr, result);

        std::string result_str = "";
        for (int i = 0; i < result.size(); ++i) {
            result_str += result[i] + " ";
        }
        return result_str;
    }
};