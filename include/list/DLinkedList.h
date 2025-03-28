/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    // Backward Iterator
    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool last = true)
        {
            if (last)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->head;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->next; // MUST next, so iterator-- will go to head
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix -- overload
        BWDIterator &operator--()
        {
            pNode = pNode->prev;
            return *this;
        }
        // Postfix -- overload
        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    this->count = 0;
    this->setDeleteUserDataPtr(deleteUserData);
    this->itemEqual = itemEqual;
    this->head = new Node(); // Dummy head
    this->tail = new Node(); // Dummy tail
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // Khởi tạo head và tail
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
    copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    if (this != &list)
    {
        removeInternalData();
        copyFrom(list);
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    clear();
    delete head;
    delete tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    Node *newNode = new Node(e, this->tail, this->tail->prev);
    this->tail->prev->next = newNode;
    this->tail->prev = newNode;
    count++;
}

template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > count)
    {
        throw out_of_range("Index is out of range!");
    }

    // Trường hợp thêm vào đầu danh sách
    if (index == 0)
    {
        Node *newNode = new Node(e, head->next, head);
        head->next->prev = newNode;
        head->next = newNode;
    }
    // Trường hợp thêm vào cuối danh sách
    // ✅ Trường hợp thêm vào cuối danh sách
    else if (index == count)
    {
        add(e);
        return;
    }
    // Trường hợp thêm vào giữa danh sách
    else
    {
        Node *prevNode = getPreviousNodeOf(index);
        Node *newNode = new Node(e, prevNode->next, prevNode);
        prevNode->next->prev = newNode;
        prevNode->next = newNode;
    }

    // Tăng số lượng phần tử
    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    if (index < 0 || index > count)
    {
        throw out_of_range("Index is out of range!");
    }

    // Trường hợp index = 0 → Trả về head vì không có phần tử đứng trước
    if (index == 0)
    {
        return head;
    }

    // Nếu index ở nửa đầu, duyệt từ head
    if (index <= count / 2)
    {
        Node *current = head->next; // Bắt đầu từ phần tử đầu tiên
        for (int i = 0; i < index - 1; i++)
        { // Duyệt đến phần tử trước index
            current = current->next;
        }
        return current;
    }
    // Nếu index ở nửa cuối, duyệt từ tail
    else
    {
        Node *current = tail->prev; // Bắt đầu từ phần tử cuối cùng
        for (int i = count - 1; i > index; i--)
        {
            current = current->prev;
        }
        return current->prev; // Trả về phần tử đứng trước index
    }
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index > count)
        throw out_of_range("Index is out of range!");
    Node *prevNode = getPreviousNodeOf(index);
    Node *currentNode = prevNode->next;

    prevNode->next = currentNode->next;
    currentNode->next->prev = prevNode;

    T removedData = currentNode->data;
    delete currentNode;
    count--;

    return removedData;
}

template <class T>
bool DLinkedList<T>::empty()
{
    return count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    removeInternalData();
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    if (index < 0 || index >= count)
        throw out_of_range("Index is out of range!");
    Node *p = getPreviousNodeOf(index)->next;
    return p->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    int index = 0;
    for (Node *node = head->next; node != tail; node = node->next)
    {
        if (equals(node->data, item, itemEqual))
        {
            return index;
        }
        index++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int index = indexOf(item);
    if (index == -1)
    {
        return false;
    }
    removeAt(index);
    return true;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    stringstream oss;
    oss << "[";
    Node *p = head->next;
    while (p != tail)
    {
        if (item2str != 0)
        {
            oss << item2str(p->data);
        }
        else
        {
            oss << p->data;
        }
        p = p->next;
        if (p != tail)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    // Kiểm tra danh sách nguồn có rỗng không
    if (list.count == 0)
        return;

    // Xóa dữ liệu cũ nếu có
    if (count > 0)
    {
        clear();
    }

    // Duyệt qua danh sách bằng con trỏ
    for (Node *node = list.head->next; node != list.tail; node = node->next)
    {
        cout << "Copying value: " << node->data << endl;

        if constexpr (std::is_pointer<T>::value)
        {
            // Nếu là con trỏ → cấp phát bộ nhớ mới (deep copy)
            add(new typename std::remove_pointer<T>::type(*node->data));
        }
        else
        {
            // Nếu không phải là con trỏ → sao chép trực tiếp
            add(node->data);
        }
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    if (deleteUserData != nullptr)
    {
        deleteUserData(this); // hàm free sẽ duyệt và delete từng node->data
    }

    // Duyệt để xóa từng node (nhưng KHÔNG xóa node->data nữa!)
    Node *node = head->next;
    while (node != tail)
    {
        Node *nextNode = node->next;
        delete node;
        node = nextNode;
    }

    head->next = tail;
    tail->prev = head;
    count = 0;
}

#endif /* DLINKEDLIST_H */
