/* 
 * File:   Huffman.h
 * Author: chachalaca
 *
 * Created on 21. červen 2014, 16:30
 */

#ifndef HTREE_H
#define	HTREE_H
  
#include <vector>
#include <algorithm>
#include <iostream>
#include "HNode.h"

template <typename T> 
class Huffman {
    public:
        Huffman();
        Huffman(const Huffman<T>& orig);
        Huffman<T> &operator=(Huffman<T> n);
        virtual ~Huffman();        
         
        std::vector<bool> encode(std::vector<T>* text);
        std::vector<T> decode(std::vector<bool>* code) const;
    private:
        std::vector< HNode<T> > createAlphabet(std::vector<T>* text) const;
        HNode<T> buildTree();
        std::vector< HNode<T> > alphabet;
        HNode<T> root;
    
};



template <typename T> 
Huffman<T>::Huffman() {
       
}

template <typename T> 
Huffman<T>::Huffman(const Huffman<T>& orig) {
    this->alphabet = orig.alphabet;
    this->root = orig.root;
}

template <typename T> 
Huffman<T> &Huffman<T>::operator=(Huffman<T> n) {
    std::swap(this->alphabet, n.alphabet);
    std::swap(this->root, n.root);


    return *this;
}

template <typename T> 
Huffman<T>::~Huffman() {
}

template <typename T>
std::vector< HNode<T> > Huffman<T>::createAlphabet(std::vector<T>* text) const{
    std::vector< HNode<T> > alphabet;
    
    typename std::vector<T>::iterator ti;
    ti = text->begin();

    while (ti != text->end()) {
        typename std::vector< HNode<T> >::iterator ai = find_if(alphabet.begin(), alphabet.end(), HNodeSymbolComparator<T>(*ti));    
        if (ai != alphabet.end()) {
            ai->setCount(ai->getCount()+1);
        }
        else {
            HNode<T> newNode;            
            alphabet.push_back(newNode);
            alphabet.back().setCount(1);
            alphabet.back().setSymbol(*ti);
        }
        ti++;
    }
     
    return alphabet;
 
}

template <typename T>
HNode<T> Huffman<T>::buildTree() {
    std::vector< HNode<T>* > temp;
    
    typename std::vector< HNode<T> >::iterator ai = this->alphabet.begin();
    while (ai != this->alphabet.end()) {
        temp.push_back(&(*ai));
        ai++;
    }
    typename std::vector< HNode<T>* >::iterator ti;
    
    while(temp.size() > 1) {
        std::sort(temp.rbegin(), temp.rend(), HNodePointerCountComparator<T>());  
        
        ti = temp.end();
        ti -= 1;

        temp.push_back(new HNode<T>());
        temp.back()->setCount(temp.back()->getCount()+(*ti)->getCount());
        temp.back()->setLeft(*ti);
        
        temp.erase(ti);
                
        ti -= 1;
                
        temp.back()->setCount(temp.back()->getCount()+(*ti)->getCount());
        temp.back()->setRight(*ti);
                
        temp.erase(ti);
        
    }
   
    std::vector<bool> nullcode;
    (*(temp.back())).assignCode(nullcode);
    
    return *(temp.back());
}

template <typename T>
std::vector<bool> Huffman<T>::encode(std::vector<T>* text) {
    
    this->alphabet = this->createAlphabet(text);
    this->root = this->buildTree(); 
    
    std::vector<bool> code;
    typename std::vector<T>::iterator ti = text->begin();    
    while (ti != text->end()) {
        typename std::vector< HNode<T> >::iterator ai = find_if(this->alphabet.begin(), this->alphabet.end(), HNodeSymbolComparator<T>(*ti));    
        if (ai != alphabet.end()) {
            code.insert(code.end(), ai->getCode().begin(), ai->getCode().end());
        }       
        
        ti++;
    }
    return code;
}

template <typename T>
std::vector<T> Huffman<T>::decode(std::vector<bool>* code) const{
    std::vector<T> text;
    while (code->size()>0) {
        text.push_back(this->root.decodeSymbol(code));
    }
    return text;
}




#endif	/* HTREE_H */

