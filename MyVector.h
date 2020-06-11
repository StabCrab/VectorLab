#pragma once
#include <cstdlib>
#include <cmath>
// стратегия изменения capacity
enum class SortedStrategy {
    Ascending,
    Descending
};
enum class ResizeStrategy {
	Additive,
	Multiplicative
};

// тип значений в векторе
// потом будет заменен на шаблон

template<class ValueType>
class MyVector
{
public:
    class Iterator
    {
    public:
        Iterator(ValueType *ptr);
        ~Iterator();
        bool operator==(const Iterator &i);
        bool operator!=(const Iterator &i);
        Iterator &operator++();
        Iterator &operator--();
        ValueType operator*();
        ValueType *operator->();
    protected:
        ValueType* ptr;
        friend class MyVector;
    };
	MyVector(size_t size = 0, ResizeStrategy resizeStrategy = ResizeStrategy::Additive, float coef = 1.5f);

	MyVector(size_t size, ValueType value = ValueType(), ResizeStrategy resizeStrategy = ResizeStrategy::Multiplicative, float coef = 1.5f);

	MyVector(MyVector &&moveVec) noexcept;

	MyVector(const MyVector& copy);

	MyVector& operator=(const MyVector& copy);

	~MyVector();

	// для умненьких — реализовать конструктор и оператор для перемещения

	size_t capacity() const;

	size_t size() const;
	float loadFactor();

	// доступ к элементу, 
	// должен работать за O(1)
	ValueType& operator[](const size_t i);
    const ValueType& operator[](const size_t i) const;

	// добавить в конец,
	// должен работать за amort(O(1))
	void pushBack(const ValueType& value);
	// вставить,
	// должен работать за O(n)
	void insert(const size_t i, const ValueType& value);// версия для одного значения
	void insert(const size_t i, const MyVector& value);

	// удалить с конца,
	// должен работать за amort(O(1))
	void popBack();
	void popFront();
	// удалить
	// должен работать за O(n)
	void erase(const size_t i);
	void erase(const size_t i, const size_t len);// удалить len элементов начиная с i

	// найти элемент,
	// должен работать за O(n)
	// если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
	// если искомого элемента нет, вернуть -1
	long long int find(const ValueType& value, bool isBegin = true) const;

	// зарезервировать память (принудительно задать capacity)
	void reserve(const size_t capacity);

	// изменить размер
	// если новый размер больше текущего, то новые элементы забиваются дефолтными значениями
	// если меньше - обрезаем вектор
	void resize(const size_t size, const ValueType value = ValueType());

	// очистка вектора, без изменения capacity
	void clear();
	Iterator begin();
	Iterator end();
	const Iterator cbegin();
	const Iterator cend();
	ValueType getValue(Iterator i);
	void setValue(Iterator i, ValueType value);
    MyVector<ValueType> sortedSquares(SortedStrategy strategy);

protected:
    ValueType* _data;
    size_t _size;
    size_t _capacity;
    ResizeStrategy _strategy;
    float _coef;
};

template<class ValueType>
MyVector<ValueType>::Iterator::Iterator(ValueType *ptr)
{
    this->ptr = ptr;
}

template<class ValueType>
MyVector<ValueType>::Iterator::~Iterator(){}

template<class ValueType>
bool MyVector<ValueType>::Iterator::operator==(const Iterator &i)
{
    return this->ptr == i.ptr;
}
template<class ValueType>
bool MyVector<ValueType>::Iterator :: operator!=(const Iterator &i)
{
    return this->ptr != i.ptr;
}
template<class ValueType>
class MyVector<ValueType>::Iterator& MyVector<ValueType>::Iterator:: operator++()
{
    this->ptr++;
    return *this;
}
template<class ValueType>
class MyVector<ValueType> ::Iterator& MyVector<ValueType>::Iterator::operator--()
{
    this->ptr--;
    return *this;
}

template<class ValueType>
ValueType MyVector<ValueType>::Iterator:: operator*()
{
    return *this->ptr;
}
template<class ValueType>
ValueType* MyVector<ValueType>::Iterator :: operator->()
{
    return this->ptr;
}
template<class ValueType>
MyVector<ValueType>::MyVector(size_t size, ResizeStrategy resizeStrategy, float coef)
{
    if (size == 0)
    {
        this->_size = size;
        this->_capacity = 1;
        this->_data = new ValueType[this->_capacity];
        this->_strategy = resizeStrategy;
        this->_coef = coef;
    }
    else
    {
        if (resizeStrategy == ResizeStrategy::Additive)
        {
            this->_size = size;
            this->_capacity = size + round(coef);
            this->_data = new ValueType[this->_capacity];
            for (int i = 0; i < size; i++)
            {
                this->_data[i] = ValueType();
            }
            this->_strategy = ResizeStrategy::Additive;
            this->_coef = coef;
        }
        else
        {
            this->_size = size;
            this->_capacity = round(size * coef);
            this->_data = new ValueType[this->_capacity];
            for (int i = 0; i < size; i++)
            {
                this->_data[i] = ValueType();
            }
            this->_strategy = ResizeStrategy :: Multiplicative;
            this->_coef = coef;
        }
    }
}

template<class ValueType>
MyVector<ValueType>::MyVector(size_t size, ValueType value, ResizeStrategy resizeStrategy, float coef)
{
    if (size == 0)
    {
        this->_size = size;
        this->_capacity = 1;
        this->_data = new ValueType[_capacity];
        this->_strategy = resizeStrategy;
        this->_coef = coef;
    }
    else
    {
        if (resizeStrategy == ResizeStrategy::Additive)
        {
            this->_size = size;
            this->_capacity = size + round(coef);
            this->_data = new ValueType[_capacity];
            this->_strategy = ResizeStrategy::Additive;
            this->_coef = coef;
            for (int i = 0; i < size; i++)
            {
                this->_data[i] = value;
            }
        }
        else
        {
            this->_size = size;
            this->_capacity = round(size * coef);
            this->_data = new ValueType[_capacity];
            this->_strategy = ResizeStrategy::Multiplicative;
            this->_coef = coef;
            for (int i = 0; i < size; i++)
            {
                this->_data[i] = value;
            }
        }
    }
}
template<class ValueType>
MyVector<ValueType>::MyVector (MyVector &&moveVec) noexcept
{
    _data = moveVec._data;
    _size = moveVec._size;
    _capacity = moveVec._capacity;
    moveVec._data = nullptr;
    moveVec._capacity = 0;
    moveVec._size = 0;
}
template<class ValueType>
MyVector<ValueType>::MyVector(const MyVector& copy)
{
    ValueType* newData = new ValueType[copy._capacity];
    for (int i = 0; i < copy._size; i++)
    {
        newData[i] = copy._data[i];
    }
    this->_data = newData;
    this->_strategy = copy._strategy;
    this->_capacity = copy._capacity;
    this->_size = copy._size;
}
template<class ValueType>
MyVector<ValueType>& MyVector<ValueType> :: operator=(const MyVector& copy)
{
    if (this == &copy)
        return *this;
    ValueType* newData = new ValueType[copy._capacity];
    for (int i = 0; i < copy._size; i++)
    {
        newData[i] = copy._data[i];
    }
    delete[] this->_data;
    this->_data = newData;
    this->_strategy = copy._strategy;
    this->_capacity = copy._capacity;
    this->_size = copy._size;
    this->_coef = copy._coef;
    return *this;
}
template<class ValueType>
MyVector<ValueType>::MyVector::~MyVector()
{
    delete[] _data;
}
template<class ValueType>
size_t MyVector<ValueType>:: capacity() const
{
    return _capacity;
}

template<class ValueType>
size_t MyVector<ValueType>:: size() const
{
    return _size;
}
template<class ValueType>
float MyVector<ValueType>:: loadFactor()
{
    if (this->_capacity == 0)
        return 0;
    else
        return (float)this->_size / (float)this->_capacity;
}
template<class ValueType>
ValueType& MyVector<ValueType>:: operator[](const size_t i)
{
    return this->_data[i];
}

template<class ValueType>
const ValueType& MyVector<ValueType>:: operator[](const size_t i) const
{
    return this->_data[i];
}

template<class ValueType>
void MyVector<ValueType>::pushBack(const ValueType& value)
{
    if(this->_capacity < this->_size + 1)
    {
        if (this->_strategy == ResizeStrategy::Multiplicative)
            reserve(_capacity * _coef);
        if (this->_strategy == ResizeStrategy::Additive)
            reserve(_capacity + _coef);
    }
    this->_data[this->_size] = value;
    this->_size++;
}

template<class ValueType>
void MyVector<ValueType>::insert(const size_t i, const ValueType& value)
{
    if (_size + 1 < _capacity)
    {
        this->reserve(_capacity + 1);
    }
    for (int j = this->_size - 1; j > i; j--)
    {
        this->_data[j] =  this->_data[j - 1];
    }
    _size++;
    this->_data[i] = value;
}
template<class ValueType>
void MyVector<ValueType>::insert(const size_t i, const MyVector<ValueType>& value)
{
    if (i < 0 || i > this->_size)
        throw std::out_of_range("i < 0 || i > this->_capacity");
    if (_size + value._size > _capacity)
    {
        if (_strategy == ResizeStrategy::Additive)
        {
            reserve(_size + value._size+ std::ceil(_coef));
        }
        else
        {
            reserve(std::ceil((float)(_size + value._size)) * _coef);
        }
    }
    for (int j = _size - 1; j >= i; j--)
    {
        _data[j + value._size] = _data[j];
    }
    for (int j = i; j <value._size + i; j ++)
    {
        _data[j] = value[j - i];
    }
    this->_size+=value._size;
}

template<class ValueType>
void MyVector<ValueType> :: popBack()
{
    this->_size--;
}
template<class ValueType>
void MyVector<ValueType>::popFront()
{
    this->_size--;
    ValueType* newData = new ValueType[this->_capacity];
    for(int i = 1; i < this->_size + 1; i++)
    {
        newData[i - 1] = this->_data[i];
    }
    delete[] this->_data;
    this->_data = newData;
}

template<class ValueType>
void MyVector<ValueType>::erase(const size_t i)
{
    ValueType* newData = new ValueType[this->_capacity];
    for (int j = 0; j < i; j++)
    {
        newData[j] = this->_data[j];
    }
    for (int j = i; j < this->_size - 1; j++)
    {
        newData[j] = this->_data[j + 1];
    }
    newData[this->_size - 1] = ValueType();
    delete[] this->_data;
    this->_data = newData;
    this->_size--;
}
template<class ValueType>
void MyVector<ValueType>::erase(const size_t i, const size_t len)
{
    if (len > _size)
        throw std::out_of_range("len > _size");
    if (len < 0)
        throw std::out_of_range("len < 0");
    size_t newCap;
    if (this->_strategy == ResizeStrategy::Additive)
    {
        newCap = (this->_size - len) + std::ceil(_coef);
    }
    else
    {
        newCap = std::ceil((float)(this->_size - len) * _coef);
    }
    if (newCap == 0)
        newCap++;
    ValueType* newData = new ValueType[newCap];
    for (int j = 0; j < i; j++)
    {
        newData[j] = this->_data[j];
    }
    for (int j = i; j < this->_size - len; j++)
    {
        newData[j] = this->_data[j + len];
    }
    delete[] this->_data;
    this->_data = newData;
    this->_size -= len;
    this->_capacity = newCap;
}
template<class ValueType>
long long int MyVector<ValueType>::find(const ValueType& value, bool isBegin) const
{
    if (isBegin)
    {
        for (size_t i = 0; i < this->_size; i++)
        {
            if (this->_data[i] == value)
                return i;
        }
        return -1;
    }
    else
    {
        for (size_t i = this->_size - 1; i > -1; i--)
        {
            if (this->_data[i] == value)
                return i;
        }
        return -1;
    }
}
template<class ValueType>
void MyVector<ValueType>::reserve(const size_t capacity)
{
    if (capacity < this->_capacity)
    {
        this->_size = capacity;
        this->_capacity = capacity;
        ValueType* newData = new ValueType[capacity];
        for (int i = 0; i < this->_size; i++)
        {
            newData[i] = this->_data[i];
        }
        delete[] this->_data;
        this->_data = newData;
    }
    else{
        this->_capacity = capacity;
        ValueType* newData = new ValueType[capacity];
        for (int i = 0; i < this->_size; i++)
        {
            newData[i] = this->_data[i];
        }
        for (int i = this->_size; i < this->_capacity; i++)
        {
            newData[i] = ValueType();
        }
        delete[] this->_data;
        this->_data = newData;
    }
}
template<class ValueType>
void MyVector<ValueType>::resize(const size_t size, const ValueType value)
{
    if (this->_capacity == size)
        return;
    if (this->_capacity < size)
    {
        if (this->_strategy == ResizeStrategy::Additive)
            this->reserve(size + std::ceil(_coef));
        else
            this->reserve (std::ceil(size * _coef));
       for (int i = this->_size; i < size; i++)
       {
           this->_data[i] = value;
       }
       this->_size = size;
    }
    else if (this->_capacity > size)
    {
        if (this->_strategy == ResizeStrategy::Additive)
            this->reserve(size + std::ceil(_coef));
        else
            this->reserve(std::ceil(size * _coef));
        this->_size = size;
    }
}
template<class ValueType>
void MyVector<ValueType>::clear()
{
    this->_size = 0;
    delete[] this->_data;
    this->_data = new ValueType [this->_capacity];
}
template<class ValueType>
class MyVector<ValueType>::Iterator MyVector<ValueType>::begin()
{
    return Iterator(&this->_data[0]);
}

template<class ValueType>
class MyVector<ValueType>::Iterator MyVector<ValueType> :: end()
{
    return Iterator(&this->_data[this->_size - 1]);
}

template<class ValueType>
const class MyVector<ValueType> :: Iterator MyVector<ValueType> :: cbegin()
{
    return Iterator(&this->_data[0]);
}
template<class ValueType>
const class MyVector<ValueType> :: Iterator MyVector<ValueType> :: cend()
{
    return Iterator(&this->_data[_size - 1]);
}
template<class ValueType>
ValueType MyVector<ValueType> :: getValue(Iterator i)
{
    return *i.ptr;
}
template<class ValueType>
void MyVector<ValueType> :: setValue(Iterator i, ValueType value)
{
    *i.ptr = value;
}

template<class ValueType>
MyVector<ValueType> MyVector<ValueType>::sortedSquares(SortedStrategy strategy)
{
    auto* vec  = new MyVector<ValueType>[this->_size];
    if (strategy == SortedStrategy::Descending)
    {
        int i = 0;
        int j = this->size() - 1;
        int k = 0;
        while(this->_data[i] < 0 && this->_data[j] >= 0)
        {
            if (abs(this->_data[i]) >= abs(this->_data[j]))
            {
                vec[k] = this->_data[i] * this->_data[i];
                i++;
                k++;
            }
            else
            {
                vec[k] = this->_data[j] * this->_data[j];
                j--;
                k++;
            }
        }
        while(this->_data[i] < 0)
        {
            vec[k] = this->_data[i] * this->_data[i];
            i++;
            k++;
        }
        while(this->_data[j] >= 0)
        {
            vec[k] = this->_data[j] * this->_data[j];
            j--;
            k++;
        }
    }
    else
    {
        int i = 0;
        int j = this->size() - 1;
        int k = this->size() - 1;
        while(this->_data[i] < 0 && this->_data[j] >= 0)
        {
            if (abs(this->_data[i]) >= abs(this->_data[j]))
            {
                vec[k] = this->_data[i] * this->_data[i];
                i++;
                k--;
            }
            else
            {
                vec[k] = this->_data[j] * this->_data[j];
                j--;
                k--;
            }
        }
        while(this->_data[i] < 0)
        {
            vec[k] = this->_data[i] * this->_data[i];
            i++;
            k--;
        }
        while(this->_data[j] >= 0)
        {
            vec[k] = this->_data[j] * this->_data[j];
            j--;
            k--;
        }
    }
    return vec;
}
