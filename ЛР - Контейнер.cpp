#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <chrono>
#include <time.h>

template <typename T>
class MyArray
{
private:
    T* key;
    int size; // Размер контейнера
public:
    MyArray()
    {
        key = nullptr;
        size = 0;
    }
    MyArray(int size)
    {
        key = new T[size];
        this->size = size;
    }
    MyArray(const MyArray& ma)
    {
        key = new T[ma.size];
        for (int i = 0; i < ma.size; ++i)
            key[i] = ma.key[i];
        size = ma.size;
    }
    ~MyArray()
    {
        delete[] key;
        key = nullptr;
    }

    // Перегрузка оператора присваивания копированием
    MyArray& operator=(const MyArray& ma)
    {
        if (this == &ma)
            return *this;

        delete[] key;
        size = ma.size;
        key = new T[size];
        for (int i = 0; i < size; ++i)
            key[i] = ma.key[i];

        return *this;
    }
    // Возвращает элемент под указанным индексом
    T& operator[](int i)
    {
        return key[i];
    }
    // Возрващает элемент под указанным индексом
    // Проверяет, вышел ли индекс за пределы допустимого диапозона
    T& at(int i)
    {
        assert((i < 0 || i >= size) && "i is out of range");
        return key[i];
    }
    // Возвращает размер контейнера
    int mySize() const
    {
        return size;
    }
    // true - если контейнер пустой
    bool isEmpty() const
    {
        return (size == 0) ? true : false;
    }

    // Изменение размера контейнера
    MyArray& resize(int size)
    {
        if (size < 0)
            size = 0;

        T* tmp = key;
        key = new T[size];
        for (int i = 0; i < size && i < this->size; ++i)
            key[i] = tmp[i];
        this->size = size;
        delete[] tmp;

        return *this;
    }
    // Добавление элемента в конец
    MyArray& pushBack(const T& num)
    {
        resize(size + 1);
        key[size - 1] = num;

        return *this;
    }
    // Добавление элемента в начало
    MyArray& pushFront(const T& num)
    {
        T* tmp = key;
        key = new T[size + 1];
        key[0] = num;
        for (int i = 0; i < size; ++i)
            key[i + 1] = tmp[i];
        ++size;
        delete[] tmp;

        return *this;
    }
    // Добавляет элемент в указанный индекс, возвращает объект
    // Если индекс больше размера, использвутеся pushFront
    // Если индекс меньше 0, используется pushBack
    MyArray& pushIn(const T& num, int index)
    {
        if (index < size && index >= 0)
        {
            T* tmp = key;
            key = new T[size + 1];
            for (int i = 0; i < index; ++i)
                key[i] = tmp[i];
            key[index] = num;
            for (int i = index; i < size; ++i)
                key[i + 1] = tmp[i];
            ++size;
            delete[] tmp;
        }
        else if (index < 0)
            pushBack(num);
        else
            pushFront(num);

        return *this;
    }
    //Удаление объекта по индексу, возвращает объект
    MyArray& remove(int index)
    {
        if (size == 0)
            return *this;
        if (index < 0)
            index = 0;
        else if (index > size)
            index = size - 1;
        for (int i = index + 1; i < size; ++i)
            key[i - 1] = key[i];
        --size;

        return *this;
    }

    // Преегрузака оператора вывода
    friend std::ostream& operator<<(std::ostream& out, const MyArray& ma)
    {
        for (int i = 0; i < ma.size; ++i)
            out << ma.key[i] << " ";

        return out;
    }

    // Функции для итератора
    T* myBegin() { return &key[0]; }
    const T* myBegin() const { return &key[0]; }
    T* myEnd() { return &key[size - 1]; }
    const T* myEnd() const { return &key[size - 1]; }

    // Семантика перемещения
    // Конструктор перемещения
    MyArray(MyArray&& ma)
    {
        key = ma.key;
        size = ma.size;
        ma.key = nullptr;
        ma.size = 0;
    }
    // Оператор присваивания
    MyArray& operator=(MyArray&& ma)
    {
        if (this == &ma)
            return *this;
        delete[] key;
        key = ma.key;
        size = ma.size;
        ma.key = nullptr;
        ma.size = 0;

        return *this;
    }
};

class MyTimer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<float, std::ratio<1>>;

    std::chrono::time_point<clock_t> time;

public:
    MyTimer() : time(clock_t::now()) {};
    void reset()
    {
        time = clock_t::now();
    }
    double duration()
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - time).count();
    }
};

void whiles(int cnt)
{
    MyArray<int> ma(cnt);
    const int cntMethods = 14;
    srand(time(NULL));
    
    MyTimer time;
    for (int i = 0; i < cnt; ++i)
    {
        int num = rand() % 10000;
        switch (num % cntMethods)
        {
        case 0:
            ma.remove(num);
            break;
        case 1:
            ma.pushBack(num);
            break;
        case 2:
            ma.pushFront(num);
            break;
        case 3:
            ma.mySize();
            break;
        case 4:
            ma.resize(num - 500);
            break;
        case 6:
            ma.isEmpty();
            break;
        case 7:
            ma.pushIn(num, num - 5000);
            break;
        case 8:
            ma.myBegin();
            break;
        case 9:
            ma.myEnd();
            break;
        case 10:
        {
            MyArray<int> ma2(ma);
            break;
        }
        case 11:
        {
            MyArray<int> ma3 = ma;
            break;
        }
        case 12:
        {
            MyArray<int> ma4(std::move(ma));
            break;
        }
        case 13:
        {
            MyArray<int> ma5 = std::move(ma);
            break;
        }
        default:
            break;
        }
    }
    std::cout << "Duration: " << time.duration() << std::endl;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Настройка потока вывода
    std::cout.setf(std::ios::fixed); // Не экспонента
    std::cout.precision(8); // Восемь знаков
    std::cout.setf(std::ios::showpoint); // Восемь знаков для всех

    whiles(100);
    whiles(1000);
    whiles(10000);

    return 0;
}