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

    tree_elem(T key) {//конструктор для создания нового узла
        DATA = key;
        M_LEFT = NULL;
        M_RIGHT = NULL;
    }
    tree_elem(T key, tree_elem<T>* left, tree_elem<T>* right) {//конструктор для копирования конкретного узла
        DATA = key;
        M_LEFT = left;
        M_RIGHT = right;
    }
};

template<typename T>
void sqr(tree_elem<T>* curr) {//(для Map)
    curr->DATA = curr->DATA * curr->DATA;
}

template<typename T>
bool is_more_than_25(tree_elem<T>* curr) {//для удаления всех элементов больших 25 (для Where)
    if (curr) {
        if (curr->DATA >= 25)
            return true;
    }
    return false;
}

template<typename T>
T sum(tree_elem<T>* curr, T curr_sum) {//(для Reduce)
    return curr_sum + curr->DATA;
}

template<typename T>
class binary_tree {
private:

    tree_elem<T>* m_root;

    int size;

    void MapRecursivelyFunction(void(*func)(tree_elem<T>*), tree_elem<T>* curr) {//для рекурсивного выполнения Map
        if (curr) {
            MapRecursivelyFunction(func, curr->M_LEFT);
            func(curr);
            MapRecursivelyFunction(func, curr->M_RIGHT);
        }
    }

    void WhereRecursivelyFunction(bool(*func)(tree_elem<T>*), tree_elem<T>* curr) {//для рекурсивного выполнения Where
        if (curr) {
            WhereRecursivelyFunction(func, curr->M_LEFT);
            WhereRecursivelyFunction(func, curr->M_RIGHT);
            if (func(curr)) {
                delete_elem(curr->DATA);
            }
        }
    }

    void ReduceRecursivelyFunction(T(*func)(tree_elem<T>*, T), tree_elem<T>* curr, T& curr_sum) {//для рекурсивного выполнения Reduce
        if (curr) {
            ReduceRecursivelyFunction(func, curr->M_LEFT, curr_sum);
            curr_sum = func(curr, curr_sum);
            ReduceRecursivelyFunction(func, curr->M_RIGHT, curr_sum);
        }
    }

    void upgrade_sub_tree(binary_tree<T>* result, tree_elem<T>* curr) {//для рекурсивного заполнения нового экземпляра
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


    //конструктор с добавлением корня
    binary_tree(int key) {
        m_root = new tree_elem<T>(key);
        size = 1;
    }
    ~binary_tree() {
        delete_tree(m_root);
    }
    int get_size() { return size; }

    //================================================================================
    //получение поддерева по узлу
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
    int height(tree_elem<T>* curr)//высота поддерева конкретного узла curr
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
    int diff(tree_elem<T>* curr)//разница высот поддеревьев узла curr
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
    //печать дерева
    void print() {
        print_tree(m_root);
        std::cout << std::endl;
    }
    //================================================================================
    //удаление дерева
    void delete_tree(tree_elem<T>* curr) {
        if (curr) {
            delete_tree(curr->M_LEFT);
            //delete[] curr;
            delete_tree(curr->M_RIGHT);
        }
    }


    //поиск элемента в дереве
    bool find(int key) {//поиск значения

        tree_elem<T>* curr = m_root;
        while (curr && curr->DATA != key) {
            if (curr->DATA > key)
                curr = curr->M_LEFT;
            else
                curr = curr->M_RIGHT;
        }
        return (bool)curr;
    }

    //добавление элемента
    void insert(int key) {//добавление нового элемента в дерево
        if (m_root == nullptr) {
            m_root = new tree_elem<T>(key);
            size++;
        }
        else {
            tree_elem<T>* curr = m_root;
            while (curr && curr->DATA != key) {
                if (curr->DATA > key) {//переход в левое поддерево
                    if (curr->M_LEFT)//если есть возможность,то так как добавляемое значение меньше того,
                        //что в узле curr,
                        //то мы спускаемся в левое поддерево ещё раз(переход для следующей итерации)
                        curr = curr->M_LEFT;
                    else {//если же возможность спуститься ещё ниже нет,то мы добавляем новый элемент в дерево
                        //(так как мы уже находимся на том месте,куда нужно добавить новый элемент)
                        curr->M_LEFT = new tree_elem<T>(key);

                        m_root = balance(m_root);

                        size++;
                        return;
                    }
                }//для правого поддерева аналогично
                else {//переход в правое поддерево
                    if (curr->M_RIGHT)
                        curr = curr->M_RIGHT;
                    else {
                        curr->M_RIGHT = new tree_elem<T>(key);

                        m_root = balance(m_root);//обновляем корень через метод балансировки
                        //----------------------------------------------------------------------------------------------
                        //(метод балансировки состоит в том,чтобы его результатом было дерево,для каждого узла которого
                        //было бы справедливо следующее - разница высот поддеревьев узла была не более 1)
                        //----------------------------------------------------------------------------------------------
                        //а именно,при добавлении нового элемента смотрим,какое из поддеревьев нашего корня стало больше
                        //из этого делаем вывод,как перераспределить дерево для баланса через
                        //методы ll_rotation,rl_rotation,lr_rotation,rr_rotation - каждый из методов предназначен для случаев
                        //"перевеса" левого поддерева левого наследника текущего корня,
                        //"перевеса" правого поддерева левого наследника текущего корня,
                        //"перевеса" левого поддерева правого наследника текущего корня,
                        //"перевеса" правого поддерева правого наследника текущего корня,

                        size++;
                        return;
                    }
                }
            }
        }
    }

    //удаление элемента
    void delete_elem(T key) {
        tree_elem<T>* curr = m_root;
        tree_elem<T>* prev = nullptr;
        //поиск удаляемого элемента
        while (curr && curr->DATA != key) {
            prev = curr;//запоминаем значение curr,которое изменится на следующем действии
            if (curr->DATA > key) {
                curr = curr->M_LEFT;
            }
            else
                curr = curr->M_RIGHT;
        }
        if (!curr)//проверка на то,существует ли тот элемент,который мы хотим удалить
            return;
        if (prev != nullptr) {
            //некорректная работа в случае prev == nullptr
            if (prev->M_RIGHT == curr) {//если удаляемый элемент является правым наследником
                if (curr->M_RIGHT == nullptr && curr->M_LEFT == nullptr) {//если удаляемый элемент вообще не имеет поддеревьев
                    prev->M_RIGHT = nullptr;
                    delete[] curr;
                    size--;
                    return;
                }
                if (curr->M_LEFT == nullptr) {//если у удаляемого элемента нет левого поддерева
                    prev->M_RIGHT = curr->M_RIGHT;
                    delete[] curr;
                    size--;
                    return;
                }
                if (curr->M_RIGHT == nullptr) {//если у удаляемого элемента нет правого поддерева
                    prev->M_RIGHT = curr->M_LEFT;
                    delete[] curr;
                    size--;
                    return;
                }
            }

            if (prev->M_LEFT == curr) {//если удаляемый элемент является левым наследником
                if (curr->M_RIGHT == nullptr && curr->M_LEFT == nullptr) {//если удаляемый элемент вообще не имеет поддеревьев
                    prev->M_LEFT = nullptr;
                    delete[] curr;
                    size--;
                    return;
                }
                if (curr->M_LEFT == nullptr) {//если у удаляемого элемента нет левого поддерева
                    prev->M_LEFT = curr->M_RIGHT;
                    delete[] curr;
                    size--;
                    return;
                }
                if (curr->M_RIGHT == nullptr) {//если у удаляемого элемента нет правого поддерева
                    prev->M_LEFT = curr->M_LEFT;
                    delete[] curr;
                    size--;
                    return;
                }
            }

            //если у удаляемого элемента есть оба поддерева,то на место удаляемого
            //мы должны поставить минимальный элемент из его правого поддерева

            tree_elem<T>* tmp = curr->M_RIGHT;//временная переменная для поиска минимума в правом поддереве
            while (tmp->M_LEFT)
                tmp = tmp->M_LEFT;

            T minimum = tmp->DATA;//сохранение значения переменной tmp для последующего удаления



            if (prev->M_LEFT == curr) {//если удаляемый элемент является левым наследником
                delete_elem(tmp->DATA);//удаление минимального элемента в правом поддереве
                prev->M_LEFT = new tree_elem<T>(minimum, curr->M_LEFT, curr->M_RIGHT);
                delete[] curr;
                size--;
                return;
            }
            if (prev->M_RIGHT == curr) {//если удаляемый элемент является правым наследником
                delete_elem(tmp->DATA);//удаление минимального элемента в правом поддереве
                prev->M_RIGHT = new tree_elem<T>(minimum, curr->M_LEFT, curr->M_RIGHT);
                delete[] curr;
                size--;
                return;
            }
        }
        else {//это условие выполняется,если мы хотим удалить корень
            if (m_root->M_RIGHT != nullptr && m_root->M_LEFT != nullptr) {//если у корня есть оба поддерева
                curr = curr->M_RIGHT;//переходим в правое поддерева корня
                tree_elem<T>* tmp = curr;

                while (curr->M_LEFT) {//ищем минимальный элемент в правом поддереве
                    curr = curr->M_LEFT;
                }

                curr->M_LEFT = m_root->M_LEFT;//присваиваем левому поддереву минимального элемента левую ветку корня
                delete[] m_root;
                m_root = new tree_elem<T>(tmp->DATA, tmp->M_LEFT, tmp->M_RIGHT);
            }
            if (m_root->M_RIGHT != nullptr) {//если у корня только правое поддерево
                tree_elem<T>* tmp = m_root;
                m_root = m_root->M_RIGHT;
                delete[] tmp;
            }
            if (m_root->M_LEFT != nullptr) {//если у корня только левое поддерево
                tree_elem<T>* tmp = m_root;
                m_root = m_root->M_LEFT;
                delete[] tmp;
            }
        }

    }

    //map,where,reduce
    void Map(void(*func)(tree_elem<T>*)) {//необходимо задать curr,поэтому создана вспомогательная функция
        tree_elem<T>* curr = m_root;
        MapRecursivelyFunction(func, curr);
    }

    void Where(bool(*func)(tree_elem<T>*)) {//необходимо задать curr,поэтому создана вспомогательная функция
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

    std::string is_balanced_str() {
        std::vector<std::string> h;
        std::string res = "";
        curr_balance(m_root, h);
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

    //представление дерева в виде строки
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


int main() {
    int flag;
    std::cout << "Stress-tests or normal work? Please choose 1 or 2 : \n";
    std::cin >> flag;
    if (flag == 1) {
        std::cout << "For the range 10,000 - 100,000, press 1\n"
            "For the range 10,000,000 - 100,000,000 press 2\n";
        std::cin >> flag;
        if (flag == 1) {
            std::cout << "||============================================ 10 000 - 100 000 elements ===========================================||\n";
            int size;

            size = 10000;
            unsigned int start_time = clock(); // начальное время
            binary_tree<int> first1;
            for (int i = 0; i < size; ++i)
            {
                first1.insert(rand() % 10);
            }
            first1.Map(sqr);
            unsigned int end_time = clock(); // конечное время
            unsigned int search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 20000;
            start_time = clock(); // начальное время
            binary_tree<int> first2;
            for (int i = 0; i < size; ++i)
            {
                first2.insert(rand() % 10);
            }
            first2.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 30000;
            start_time = clock(); // начальное время
            binary_tree<int> first3;
            for (int i = 0; i < size; ++i)
            {
                first3.insert(rand() % 10);
            }
            first3.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 40000;
            start_time = clock(); // начальное время
            binary_tree<int> first4;
            for (int i = 0; i < size; ++i)
            {
                first4.insert(rand() % 10);
            }
            first4.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 50000;
            start_time = clock(); // начальное время
            binary_tree<int> first5;
            for (int i = 0; i < size; ++i)
            {
                first5.insert(rand() % 10);
            }
            first5.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 60000;
            start_time = clock(); // начальное время
            binary_tree<int> first6;
            for (int i = 0; i < size; ++i)
            {
                first6.insert(rand() % 10);
            }
            first6.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 70000;
            start_time = clock(); // начальное время
            binary_tree<int> first7;
            for (int i = 0; i < size; ++i)
            {
                first7.insert(rand() % 10);
            }
            first7.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 80000;
            start_time = clock(); // начальное время
            binary_tree<int> first8;
            for (int i = 0; i < size; ++i)
            {
                first8.insert(rand() % 10);
            }
            first8.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 90000;
            start_time = clock(); // начальное время
            binary_tree<int> first9;
            for (int i = 0; i < size; ++i)
            {
                first9.insert(rand() % 10);
            }
            first9.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            size = 100000;
            start_time = clock(); // начальное время
            binary_tree<int> first10;
            for (int i = 0; i < size; ++i)
            {
                first10.insert(rand() % 10);
            }
            first10.Map(sqr);
            end_time = clock(); // конечное время
            search_time = end_time - start_time; // искомое время
            std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

            std::cout << "||==================================================================================================================||\n\n";
        }
        else {
            if (flag == 2) {
                std::cout << "||==================================== 10 000 000 - 100 000 000 elements =============================================||\n";
                int size;

                size = 10000000;
                unsigned int start_time = clock(); // начальное время
                binary_tree<int> first1;
                for (int i = 0; i < size; ++i)
                {
                    first1.insert(rand() % 10);
                }
                first1.Map(sqr);
                unsigned int end_time = clock(); // конечное время
                unsigned int search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 20000000;
                start_time = clock(); // начальное время
                binary_tree<int> first2;
                for (int i = 0; i < size; ++i)
                {
                    first2.insert(rand() % 10);
                }
                first2.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 30000000;
                start_time = clock(); // начальное время
                binary_tree<int> first3;
                for (int i = 0; i < size; ++i)
                {
                    first3.insert(rand() % 10);
                }
                first3.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 40000000;
                start_time = clock(); // начальное время
                binary_tree<int> first4;
                for (int i = 0; i < size; ++i)
                {
                    first4.insert(rand() % 10);
                }
                first4.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 50000000;
                start_time = clock(); // начальное время
                binary_tree<int> first5;
                for (int i = 0; i < size; ++i)
                {
                    first5.insert(rand() % 10);
                }
                first5.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 60000000;
                start_time = clock(); // начальное время
                binary_tree<int> first6;
                for (int i = 0; i < size; ++i)
                {
                    first6.insert(rand() % 10);
                }
                first6.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 70000000;
                start_time = clock(); // начальное время
                binary_tree<int> first7;
                for (int i = 0; i < size; ++i)
                {
                    first7.insert(rand() % 10);
                }
                first7.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 80000000;
                start_time = clock(); // начальное время
                binary_tree<int> first8;
                for (int i = 0; i < size; ++i)
                {
                    first8.insert(rand() % 10);
                }
                first8.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 90000000;
                start_time = clock(); // начальное время
                binary_tree<int> first9;
                for (int i = 0; i < size; ++i)
                {
                    first9.insert(rand() % 10);
                }
                first9.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                size = 100000000;
                start_time = clock(); // начальное время
                binary_tree<int> first10;
                for (int i = 0; i < size; ++i)
                {
                    first10.insert(rand() % 10);
                }
                first10.Map(sqr);
                end_time = clock(); // конечное время
                search_time = end_time - start_time; // искомое время
                std::cout << "Execution time for " << size << " elements is " << search_time << " mls" << std::endl;

                std::cout << "||==================================================================================================================||\n\n";
            }
        }

    }
    else {
        if (flag == 2) {
            //Балансировка 
            int n;
            std::cout << "Enter count of first_values in your first tree :\n";

            if (!(std::cin >> n)) {
                std::cout << "Invalid input\n";
                return -1;
            }

            int* first_values = new int[n];
            int value;
            std::cout << "Enter " << n << " elements for your first tree :\n";
            
            for (int i = 0; i < n; ++i)
            {
                if (std::cin >> value)
                    first_values[i] = value;
                else {
                    delete[] first_values;
                    std::cout << "Invalid input\n";
                    return -1;
                }
            }
            binary_tree<int>first;
            for (int i = 0; i < n; ++i)
            {
                first.insert(first_values[i]);
            }

            std::cout << "First tree :\n";
            first.print();


            //===========================================================//
            std::cout << "Enter count of second_values in your second tree :\n";

            if (!(std::cin >> n)) {
                delete[] first_values;
                std::cout << "Invalid input\n";
                return -1;
            }

            int* second_values = new int[n];
            std::cout << "Enter " << n << " elements for your second tree :\n";
            for (int i = 0; i < n; ++i)
            {
                if (std::cin >> value)
                    second_values[i] = value;
                else {
                    delete[] first_values;
                    delete[] second_values;
                    std::cout << "Invalid input\n";
                    return -1;
                }
            }
            binary_tree<int> second;
            for (int i = 0; i < n; ++i)
            {
                second.insert(second_values[i]);
            }

            std::cout << "About balancing: below are all the differences in the heights of all nodes of the first and second trees.\n";
            std::cout << "First :\n";
            first.is_balanced();//так как деревья сбалансированы в строке будут только числа -1,0,1
            std::cout << "Second :\n";
            second.is_balanced();

            first.Map(sqr);
            std::cout << "MAP(First) :\n";// first tree after squaring each element
            first.print();

            second.Where(is_more_than_25);
            std::cout << "Where(Second) :\n";//All items less than 25 that belong to the second tree
            second.print();

            std::cout << "Reduce(first) = " << first.Reduce(sum) << std::endl;//The sum of the elements of the modified first tree


            delete[] first_values;
            delete[] second_values;
        }
    }
    return 0;
}