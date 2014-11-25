/* 
 * File:   sentence.cpp
 * Author: sandeep
 * 
 * Created on November 9, 2014, 11:45 AM
 */

#include "sentence.h"

sentence::sentence() {
    
    this->number_Of_operators=0;
    this->conjuncts_Count = 0;
//    this->has_Variable = false;
}

sentence::sentence(const sentence& orig) {
}

sentence::~sentence() {
}

/*Construction of knowledge base*/

void sentence::update_data(string key, string value){
    static struct predicate* new_Predicate = new (struct predicate);
    if(key == "predicate")
        new_Predicate = new (struct predicate);
    if(key == "predicate"){
        new_Predicate->name = value;
    }
    else if(key == "members"){
        new_Predicate->members[new_Predicate->number_Of_Members] = value;
        new_Predicate->number_Of_Members++;
        
    }
    else if(key == "update"){
        this->conjuncts.push_back(new_Predicate);
        this->conjuncts_Count ++;
    }
    else{
        this->conjuncts.push_back(new_Predicate);
        new_Predicate = new (struct predicate);
        this->conjuncts_Count ++;
        if(value == "and")
            this->oper.push_back(1);
        else
            this->oper.push_back(2);
        this->number_Of_operators++;
    }
}

/*Print a sentence for debugging purpose */

void sentence::print(){
    vector<struct predicate *>::iterator it = this->conjuncts.begin();
    while(it!=this->conjuncts.end()){
        cout << (*it)->name << "\t";
        int i=0;
        while(i < (*it)->number_Of_Members){
            cout << (*it)->members[i] << "\t";
            i++;
        }
        
        i=0;
       
        it++;
        
    }
    vector<int>::iterator it1 = this->oper.begin();
    while(it1 != this->oper.end()){
        cout << (*it1) << "\t";
        it1++;
    }
    
}

bool sentence::check_Entilled(sentence *query,string substitution){
    bool return_Value = false;
    vector <struct predicate *> ::iterator it1 = this->conjuncts.begin();
    vector <struct predicate *> ::iterator it2 = query->conjuncts.begin();
    while(it1 != this->conjuncts.end() && it2 != query->conjuncts.end()){
        if((*it1)->name == (*it2)->name){
            int members_Iterator1=0;
            int members_Iterator2=0;
            while(members_Iterator1 < (*it1)->number_Of_Members && members_Iterator2 < (*it2)->number_Of_Members){
                if((*it1)->members[members_Iterator1] == (*it2)->members[members_Iterator2]){
                    members_Iterator1++;
                    members_Iterator2++;
                }
                else{
                    
                    if((*it1)->members[members_Iterator1] == "x"){
                        if(!((*it2)->members[members_Iterator2] == substitution)){
                            break;
                            //return_Value=true;
                        }
                    }
                    else if((*it2)->members[members_Iterator1] == "x"){
                         if(!((*it1)->members[members_Iterator2] == substitution)){
                            //return_Value=true;
                             break;
                        }
                    }
                    else{
                        return_Value = false;
                        break;
                    }
                    
                    members_Iterator1++;
                    members_Iterator2++;
                }
                    
            }
            if((members_Iterator1 != (*it1)->number_Of_Members) || (members_Iterator2 != (*it2)->number_Of_Members)){
                return_Value = false;
                break;
            }
            if((members_Iterator1 == (*it1)->number_Of_Members) && (members_Iterator2 == (*it2)->number_Of_Members)){
                return_Value = true;
                return return_Value;
            }
        }
        else{
            return_Value = false;
           // break;
        }
        
        it1++;
        //it2++;
        
    }
  /*  if(it1 == this->conjuncts.end() && it2 == query->conjuncts.end() )
        return_Value = true;*/
    return return_Value;
}

bool sentence::is_Implication(){
    int i=0;
    bool return_Value = false;
    vector <int> :: iterator it = this->oper.end();
    if(it==this->oper.begin())
        return return_Value;
    it--;
    if ((*it) == 2){
        return_Value = true;
        
    }
    
    return return_Value;
}

bool sentence::is_Query_Implied(sentence *query){
    bool return_Value = false;
    vector<struct predicate *> ::iterator it = this->conjuncts.end();
    it--;
    vector<struct predicate *>::iterator it1 = query->conjuncts.begin();
    if((*it)->name == (*it1)->name && (*it)->number_Of_Members == (*it1)->number_Of_Members){
        int i=0;
        while(i < (*it)->number_Of_Members){
            if((*it)->members[i]!=(*it1)->members[i]){
                if((*it)->members[i]=="x" || ((*it1)->members[i]=="x"))
                    i++;
                else{
                    break;
                }
            }
            else{
                i++;
            }
                
        }
        if(i == (*it)->number_Of_Members)
            return_Value = true;
        
    }
    return return_Value;
}

bool sentence::has_Variable(){
    vector <struct predicate *>::iterator it1 = this->conjuncts.end();
    it1 --;
    int it = (*it1)->number_Of_Members;
    int i = 0;
    while(i < it){
        if((*it1)->members[i] == "x")
            return true;
        i++;
    }
    return false;
}

string sentence::return_Sub(sentence *query,string substitution){
    vector <struct predicate *>::iterator it1 =query->conjuncts.begin();
    vector <struct predicate *>::iterator it2 =this->conjuncts.end();
    it2--;
    int i=0;
    while(i < (*it1)->number_Of_Members && i < (*it2)->number_Of_Members && (*it1)->members[i] == (*it2)->members[i]){
        i++;
    }
    if(i!=(*it1)->number_Of_Members || i!=(*it2)->number_Of_Members){
        if((*it1)->members[i] == "x"){
            if((*it2)->members[i] != substitution)
                return (*it2)->members[i];
        }
        else{
            if((*it1)->members[i] != substitution)
            return (*it1)->members[i];
        }
    }
   if( i == (*it1)->number_Of_Members && i == (*it2)->number_Of_Members )
       return "";
   else
       return "nosub";
        
        
}


/*
 Get predicate sturcture at nth posistion
 */

struct predicate * sentence::return_Nth_Predicate(int position){
    int i=1;
    vector<struct predicate *>::iterator it = this->conjuncts.begin();
    while(i<position && it!=this->conjuncts.end()){
        it++;
        i++;
    }
    if(it!=this->conjuncts.end() && i!=this->conjuncts_Count)
        return (*it);
    else
        return NULL;
}

void sentence::add_Predicate(struct predicate *input_Predicate){
    this->conjuncts_Count++;
    this->conjuncts.push_back(input_Predicate);
    return;
    
}
