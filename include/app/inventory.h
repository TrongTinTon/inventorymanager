#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    void remove(int index);
    string
    toString() const;

    friend ostream &operator<<(ostream &os, const List1D<T> &list)
    {
        for (int i = 0; i < list.size(); i++)
        {
            os << list.get(i) << " ";
        }
        return os;
    };
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;
    void removeRow(int index);

    friend ostream &operator<<(ostream &os, const List2D<T> &matrix)
    {
        for (int i = 0; i < matrix.rows(); i++)
        {
            os << matrix.getRow(i) << "\n";
        }

        return os;
    };
};

struct InventoryAttribute
{
    string name;
    double value;
    InventoryAttribute() : name(""), value(0.0) {}
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const;
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        pList->add(T());
    }
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        pList->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    pList = new XArrayList<T>();
    for (int i = 0; i < other.size(); i++)
    {
        pList->add(other.get(i));
    }
}

template <typename T>
List1D<T>::~List1D()
{
    delete pList;
}

template <typename T>
int List1D<T>::size() const
{
    return pList->size();
}

template <typename T>
T List1D<T>::get(int index) const
{
    return pList->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    pList->get(index) = value;
}

template <typename T>
void List1D<T>::add(const T &value)
{
    pList->add(value);
}

template <typename T>
string List1D<T>::toString() const
{
    return pList->toString();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    for (int i = 0; i < list.size(); i++)
    {
        os << list.get(i) << " ";
    }
    return os;
}

template <typename T>
void List1D<T>::remove(int index)
{
    pList->removeAt(index);
}
// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    pMatrix = new XArrayList<IList<T> *>();
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    pMatrix = new XArrayList<IList<T> *>();
    for (int i = 0; i < num_rows; i++)
    {
        pMatrix->add(new XArrayList<T>());
        for (int j = 0; j < array[i].size(); j++)
        {
            pMatrix->get(i)->add(array[i].get(j));
        }
    }
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    pMatrix = new XArrayList<IList<T> *>();
    for (int i = 0; i < other.rows(); i++)
    {
        IList<T> *row = new XArrayList<T>();
        for (int j = 0; j < other.getRow(i).size(); j++)
        {
            row->add(other.get(i, j));
        }
        pMatrix->add(row);
    }
}

template <typename T>
List2D<T>::~List2D()
{
    for (int i = 0; i < pMatrix->size(); i++)
    {
        delete pMatrix->get(i);
    }
    delete pMatrix;
}

template <typename T>
int List2D<T>::rows() const
{
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    delete pMatrix->get(rowIndex);
    IList<T> *newRow = new XArrayList<T>();
    for (int i = 0; i < row.size(); i++)
    {
        newRow->add(row.get(i));
    }
    pMatrix->get(rowIndex) = newRow;
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    return pMatrix->get(rowIndex)->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    List1D<T> row;
    for (int i = 0; i < pMatrix->get(rowIndex)->size(); i++)
    {
        row.add(pMatrix->get(rowIndex)->get(i));
    }
    return row;
}

template <typename T>
string List2D<T>::toString() const
{
    stringstream ss;
    for (int i = 0; i < rows(); i++)
    {
        ss << getRow(i).toString() << "\n";
    }
    return ss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    for (int i = 0; i < matrix.rows(); i++)
    {
        os << matrix.getRow(i) << "\n";
    }

    return os;
}

template <typename T>
void List2D<T>::removeRow(int index)
{
    delete pMatrix->get(index); // Giải phóng bộ nhớ hàng
    pMatrix->removeAt(index);   // Xoá hàng khỏi danh sách
}
// -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    attributesMatrix = List2D<InventoryAttribute>();
    productNames = List1D<string>();
    quantities = List1D<int>();
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities) : attributesMatrix(matrix), productNames(names), quantities(quantities) {}

InventoryManager::InventoryManager(const InventoryManager &other) : attributesMatrix(other.attributesMatrix),
                                                                    productNames(other.productNames),
                                                                    quantities(other.quantities) {}

int InventoryManager::size() const
{
    return productNames.size();
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    return attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const
{
    return productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const
{
    return quantities.get(index);
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    attributesMatrix.setRow(attributesMatrix.rows(), attributes);
    productNames.add(name);
    quantities.add(quantity);
}

void InventoryManager::removeProduct(int index)
{
    productNames.remove(index);
    quantities.remove(index);
    attributesMatrix.removeRow(index);
}

List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    List1D<string> result;

    for (int i = 0; i < size(); i++)
    {
        List1D<InventoryAttribute> attributes = getProductAttributes(i);
        bool matched = false;

        for (int j = 0; j < attributes.size(); j++)
        {
            if (attributes.get(j).name == attributeName &&
                attributes.get(j).value >= minValue &&
                attributes.get(j).value <= maxValue &&
                getProductQuantity(i) >= minQuantity)
            {
                matched = true;
                break;
            }
        }

        if (matched)
        {
            result.add(getProductName(i));
        }
    }

    // Sắp xếp kết quả
    if (ascending)
    {
        for (int i = 0; i < result.size() - 1; i++)
        {
            for (int j = i + 1; j < result.size(); j++)
            {
                if (result.get(i) > result.get(j))
                {
                    string temp = result.get(i);
                    result.set(i, result.get(j));
                    result.set(j, temp);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < result.size() - 1; i++)
        {
            for (int j = i + 1; j < result.size(); j++)
            {
                if (result.get(i) < result.get(j))
                {
                    string temp = result.get(i);
                    result.set(i, result.get(j));
                    result.set(j, temp);
                }
            }
        }
    }

    return result;
}

void InventoryManager::removeDuplicates()
{
    for (int i = 0; i < size(); i++)
    {
        for (int j = i + 1; j < size(); j++)
        {
            if (productNames.get(i) == productNames.get(j))
            {
                removeProduct(j);
                j--;
            }
        }
    }
}

InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    InventoryManager result = inv1;
    for (int i = 0; i < inv2.size(); i++)
    {
        result.addProduct(inv2.getProductAttributes(i), inv2.getProductName(i), inv2.getProductQuantity(i));
    }
    return result;
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    int splitIndex = size() * ratio;

    for (int i = 0; i < splitIndex; i++)
    {
        section1.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
    }
    for (int i = splitIndex; i < size(); i++)
    {
        section2.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
    }
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    return attributesMatrix;
}

List1D<string> InventoryManager::getProductNames() const
{
    return productNames;
}

List1D<int> InventoryManager::getQuantities() const
{
    return quantities;
}

string InventoryManager::toString() const
{
    stringstream ss;
    ss << left << setw(20) << "Product Name"
       << setw(10) << "Quantity"
       << "Attributes\n";

    for (int i = 0; i < size(); i++)
    {
        ss << left << setw(20) << getProductName(i)
           << setw(10) << getProductQuantity(i)
           << getProductAttributes(i).toString() << "\n";
    }
    return ss.str();
}

inline ostream &operator<<(ostream &os, const InventoryAttribute &attr)
{
    os << "{" << attr.name << ": " << attr.value << "}";
    return os;
}

inline bool operator==(const InventoryAttribute &lhs, const InventoryAttribute &rhs)
{
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

#endif /* INVENTORY_MANAGER_H */
