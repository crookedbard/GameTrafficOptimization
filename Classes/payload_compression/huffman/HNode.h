/* 
 * File:   HNode.h
 * Author: chachalaca
 *
 * Created on 21. červen 2014, 16:44
 */

#ifndef HNODE_H
#define	HNODE_H

#include <vector>
#include <iostream>
#include <stddef.h>
#include <exception>
#include <algorithm>

template <typename T> 
class HNode { 
    public: 
        HNode();
        
        HNode(const HNode<T>& orig);
        
        HNode<T> &operator=(HNode<T> n);
        
        virtual ~HNode();
        
        void assignCode(std::vector<bool> code);
        
        T decodeSymbol(std::vector<bool>* code) const;
        
        const int& getCount() const;
        void setCount(const int& newCount);    
         
        const T& getSymbol() const;
        void setSymbol(const T& newSymbol); 
        
        const std::vector<bool>& getCode() const;
        void setCode(const std::vector<bool>& newCode); 
         
        const HNode<T>* getLeft() const;
        void setLeft(HNode<T>* newLeft); 
        
        const HNode<T>* getRight() const;
        void setRight(HNode<T>* newRight);          
        
    private:
        int count;
        T symbol;        
        std::vector<bool> code;        
        HNode<T>* left;        
        HNode<T>* right;
};


template <typename T> 
struct HNodeSymbolComparator {
    HNodeSymbolComparator(T const& s) : _s(s) { }  
    
    T _s;
    
    bool operator () (HNode<T> const& node) {
        return (node.getSymbol() == _s);
    }    
};

template <typename T> 
struct HNodePointerCountComparator {
      bool operator ()(const HNode<T>* l, const HNode<T>* r) {
//          std::cout<<"porovnani "<<(l)->count<<" "<<(r)->count<<std::endl;
        return l->getCount() < r->getCount();
      }
};

template <typename T> 
HNode<T>::HNode() {
    this->left = NULL;
    this->right = NULL;
    this->count = 0;
    this->symbol = NULL;
}

template <typename T> 
HNode<T>::HNode(const HNode& orig) {
    this->count = orig.count;
    this->symbol = orig.symbol;
    this->code = orig.code;
    this->left = orig.left;
    this->right = orig.right;
}

template <typename T> 
HNode<T> &HNode<T>::operator=(HNode<T> n) {
    std::swap(this->count, n.count);
    std::swap(this->symbol, n.symbol);
    std::swap(this->code, n.code);
    std::swap(this->left, n.left);
    std::swap(this->right, n.right);

    return *this;
}

template <typename T> 
HNode<T>::~HNode() {
}

template <typename T> 
const int& HNode<T>::getCount() const {
    return this->count;
}
template <typename T> 
void HNode<T>::setCount(const int& newCount) {
    this->count = newCount;
    
}

template <typename T> 
const T& HNode<T>::getSymbol() const {
    return this->symbol;
}
template <typename T> 
void HNode<T>::setSymbol(const T& newSymbol) {
    this->symbol = newSymbol;
    
}

template <typename T> 
const std::vector<bool>& HNode<T>::getCode() const {
    return this->code;
}
template <typename T> 
void HNode<T>::setCode(const std::vector<bool>& newCode) {
    this->code = newCode;
    
}

template <typename T> 
const HNode<T>* HNode<T>::getLeft() const {
    return this->left;
}
template <typename T> 
void HNode<T>::setLeft(HNode<T>* newLeft) {
    this->left = newLeft;
    
}

template <typename T> 
const HNode<T>* HNode<T>::getRight() const {
    return this->right;
}
template <typename T> 
void HNode<T>::setRight(HNode<T>* newRight) {
    this->right = newRight;
    
}


template <typename T>
void HNode<T>::assignCode(std::vector<bool> code) {
    this->code = code;
       
    if(this->left != NULL) {
        code.push_back(false);
        this->left->assignCode(code);        
        code.pop_back();                
    }
    if(this->right != NULL) {
        code.push_back(true);
        this->right->assignCode(code);
    }    
}

template <typename T>
T HNode<T>::decodeSymbol(std::vector<bool>* code) const {
    if(this->right == NULL && this->left == NULL) {
        return this->symbol;
    }        
    if(this->right != NULL && code->front()==true) {
        code->erase(code->begin());
        return this->right->decodeSymbol(code);
    }
    if(this->left != NULL && code->front()==false) {
        code->erase(code->begin());
        return this->left->decodeSymbol(code);
    }    
    std::cerr << "ERROR: to neni huffmanuv kod!\n";
    std::terminate();    
}


#endif	/* HNODE_H */

