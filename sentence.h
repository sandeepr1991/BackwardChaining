/* 
 * File:   sentence.h
 * Author: sandeep
 *
 * Created on November 9, 2014, 11:45 AM
 */

#ifndef SENTENCE_H
#define SENTENCE_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;
enum operator_t{
    AND_OPERATOR=1,
    IMPLICATION_OPERATOR=2
};   // 1:& 2:<< 

struct predicate{
    string name;
    int number_Of_Members ;
    string members[10];
    predicate() : number_Of_Members(0){}
};

class sentence {
public:
    sentence();
    sentence(const sentence& orig);
    void update_data(string,string);
    bool check_Entilled(sentence *,string);
    bool is_Implication();
    bool is_Query_Implied(sentence *);
    bool has_Variable();
    string return_Sub(sentence *,string);
    struct predicate * return_Nth_Predicate(int); 
    void add_Predicate(struct predicate *);
    void print();
    virtual ~sentence();
    
private:
    
    vector<struct predicate *> conjuncts;
    vector<int> oper;
    int number_Of_operators;
    int conjuncts_Count;
    
    
};

#endif  /* SENTENCE_H */
