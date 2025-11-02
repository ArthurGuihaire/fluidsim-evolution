#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>

template<typename T>
class dynamicArray {
    public:
        dynamicArray()
         : m_capacity(1)
        {
            m_heapArray = (T*)malloc(sizeof(T));
        }

        dynamicArray(const uint32_t startingNumElements)
         : m_capacity(startingNumElements)
        {
            m_heapArray = (T*)malloc(startingNumElements * sizeof(T));
        }

        ~dynamicArray() {
            free(m_heapArray);
        }

        dynamicArray(const dynamicArray<T>& other)
         : m_capacity(other.m_capacity), m_size(other.m_size)
        {
            m_heapArray = (T*)malloc(m_capacity * sizeof(T));
            std::memcpy(m_heapArray, other.m_heapArray, m_size * sizeof(T));
        }

        dynamicArray(dynamicArray<T>&& other) noexcept
         : m_capacity(other.m_capacity), m_size(other.m_size), m_heapArray(other.m_heapArray)
        {
            other.m_heapArray = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        void reserve(const uint32_t newNumElements) {
            if (newNumElements > m_capacity) {
                m_capacity = newNumElements;
                m_heapArray = (T*)realloc(m_heapArray, m_capacity * sizeof(T));
            }
        }

        void reserveOrDouble(const uint32_t newNumElements) {
            if (newNumElements > m_capacity) {
                m_capacity = (2*m_capacity > newNumElements) ? 2*m_capacity : newNumElements;
                m_heapArray = (T*)realloc(m_heapArray, m_capacity * sizeof(T));
            }
        }

        void add(const T element) {
            if (m_size == m_capacity) {
                m_capacity *= 2;
                m_heapArray = (T*)realloc(m_heapArray, m_capacity * sizeof(T));
            }
            m_heapArray[m_size++] = element;
        }

        void insert(const T element, const uint32_t index) {
            if (m_size == m_capacity) {
                m_capacity *= 2;
                m_heapArray = (T*)realloc(m_heapArray, m_capacity * sizeof(T));
            }
            //Start at size since we're expanding the array
            std::memmove(&m_heapArray[index+1], &m_heapArray[index], (m_size - index) * sizeof(T));
            m_heapArray[index] = element;
            m_size++;
        }

        void addMultiple(const T* elements, const uint32_t numElements) {
            m_capacity = (2*m_capacity > m_capacity + numElements) ? 2*m_capacity : m_capacity + numElements;
            m_heapArray = (T*) realloc(m_heapArray, m_capacity * sizeof(T));
            std::memcpy(&m_heapArray[m_size], elements, numElements * sizeof(T));
            m_size += numElements;
        }

        T pop()  {
            return m_heapArray[--m_size];
        }

        void remove(const uint32_t index) {
            m_size--;
            std::memmove(&m_heapArray[index], &m_heapArray[index+1], (m_size - index) * sizeof(T));
        }

        bool empty() {
            return m_size == 0;
        }

        uint32_t size() const {
            return m_size;
        }

        uint32_t capacity() const {
            return m_capacity;
        }

        T& operator[](const uint32_t index) {
            return m_heapArray[index];
        }

        const T& operator[](const uint32_t index) const {
            return m_heapArray[index];
        }
        
    private:
        uint32_t m_size = 0;
        uint32_t m_capacity;
        T* m_heapArray;
};
