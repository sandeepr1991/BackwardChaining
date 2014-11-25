/* 
 * File:   main.cpp
 * Author: sandeep
 *
 * Created on November 9, 2014, 11:44 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <queue>
#include "sentence.h"
using namespace std;
bool check = false;

/*
 * 
 */

string backward_Chaining(sentence *,vector<sentence *>, string);
sentence create_Sentence(struct predicate *);

int main(int argc, char** argv) {
    ofstream out;
    ifstream in;
    char buf[255];
    char *sub_String;
    bool allow_Predicate = false;
    sentence *query = new sentence;
    sentence *new_Sentence = new sentence;
    int line_Number = 1;
    int total_KB=999;
    int read_Sentences = 0;
    in.open("input.txt",ios::in);
    
    vector<sentence *> know_Base;
    while(in.eof() == false && read_Sentences < total_KB){
        in >> buf;
        if(line_Number == 1){
            sub_String = strtok(buf, "(,)" );
            
            query->update_data("predicate", sub_String);
            sub_String = strtok ( NULL, "(,)");
            while(sub_String != NULL){
                
                string s_String(sub_String);
                if(s_String.find_first_of("&") != -1 ){
                    query->update_data("operator","and");
                }
                else if(s_String.find_first_of("=>") != -1){
                    query->update_data("operator","imply");
                }
                else{
                    query->update_data("members",sub_String);
                }
                sub_String = strtok ( NULL, "(,)");
            }
            line_Number++;
            continue;
        }
        else if(line_Number == 2){
            total_KB = atoi(buf);
            line_Number++;
            query->update_data("update", "update");
            continue;
        }
        else{
            sub_String = strtok(buf, "(,)");
            new_Sentence->update_data("predicate", sub_String);
            sub_String = strtok ( NULL, "(,)");
            while(sub_String != NULL){
                
                string s_String(sub_String);
                if(s_String.compare("x") == 0){
                    new_Sentence->update_data("members","x");
                }
                else if(s_String.find("&") != -1){
                    
                    new_Sentence->update_data("operator", "and");
                    s_String = s_String.substr(1,s_String.length()-1);
                    new_Sentence->update_data("predicate", s_String);
                    
                    
                }
                else if(s_String.find_first_of("=>") !=-1){
                    
                    new_Sentence->update_data("operator", "imply");
                    s_String = s_String.substr(2,s_String.length()-2);
                    new_Sentence->update_data("predicate", s_String);
                }
                else{
                    new_Sentence->update_data("members",sub_String);
                }
                 
                sub_String = strtok ( NULL, "(,)");
            }
            read_Sentences++;
        }
        new_Sentence->update_data("update", "update");
        know_Base.push_back(new_Sentence);
        new_Sentence = new sentence;
        line_Number++;
    }

    vector<sentence *>::iterator it = know_Base.begin();
/*    while(it != know_Base.end()){
        (*it)->print();
        it++;
        cout << "\n";
    }
    query->print();*/
    if(backward_Chaining(query, know_Base,"")!="false"){
        out.open("output.txt",ios::out);
        out << "TRUE";
    }
    else{
        out.open("output.txt",ios::out);
        out << "FALSE";
    }
    return 0;
}

/* Perform backward chaining to check whether the query is entilled*/

string backward_Chaining(sentence *query, vector<sentence *> KB, string substitution){
    string return_Value = "false";
    string new_substitution;
    ofstream out;
    //string sub;
    vector<sentence *>::iterator it = KB.begin();
    bool all_Entiled = true;
    while(it != KB.end()){
        if(((*it)->is_Implication())==false && ((*it)->check_Entilled(query,substitution))){
            return_Value = substitution;
            return return_Value;
        }
        it++;
    }
    if(return_Value == "false" ){
        it = KB.begin();
        while(it != KB.end()){
       //     if((*it)->is_Implication()){
                if((*it)->is_Query_Implied(query)){
                    if((*it)->has_Variable()){
                        if(substitution == "" || check==true){
                            new_substitution = (*it)->return_Sub(query,substitution);
                        }
                        }
                    else if((query)->has_Variable()){
                        if(substitution == "" || check==true){
                           new_substitution = (*it)->return_Sub(query,substitution);
                        }
                    }
                    
                    if(new_substitution == "nosub"){
                        out.open("output.txt",ios::out);
                        out << "FALSE";
                        out.close();
                        exit(0);
                    }
                    if(!(new_substitution=="false" || new_substitution=="")){
                        check = false;
                        substitution = new_substitution;
                    }
                        queue<struct predicate *> predicates_To_Prove;
                        struct predicate * predicate_To_Prove;
                        int i=1;
                        while(i && all_Entiled == true){
                            predicate_To_Prove = (*it)->return_Nth_Predicate(i);
                            if(predicate_To_Prove!=NULL){
                                sentence sentence_To_Prove = create_Sentence(predicate_To_Prove);
                                
                                
                                if((new_substitution=backward_Chaining(&sentence_To_Prove,KB,substitution)) == "false"){
                                    all_Entiled = false;
                                    it = KB.begin();
                                    check = true;
                                    //continue;
                                   // substitution ="";
                                    //return false;
                                   // break;
                                }
                                else
                                    substitution = new_substitution;
                            }
                            else{
                                substitution = new_substitution;
                                return_Value = substitution;
                                return return_Value;
                            }
                            i++;
                            
                        }
                    
                    
                }
         //   }
            if(all_Entiled == true)
                it++;
                all_Entiled = true;
        }
    }
    out.open("output.txt",ios::out);
    out << "FALSE";
    out.close();
    exit(0);
    return return_Value;
    
}
/*
 prepare a sentence, given a predicate
 */

sentence create_Sentence(struct predicate * input_Predicate){
    sentence query;
    query.add_Predicate(input_Predicate);
    return query;
}
