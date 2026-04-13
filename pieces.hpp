#pragma once

#include "board.hpp"

class Developer:public Employee {
    public:
        string FavProgrammingLanguage;
        
        /* CHILD CLASS CONSTRUCTOR */
         
        Developer(string name, int age, string company, string favProgrammingLanguage)
            :Employee(name, age, company) 
        {
            FavProgrammingLanguage = favProgrammingLanguage;
        }

        // Cannot use Name in parent class as its private.
        // void fix_bug() {
        //     std::cout << get_name() << " fixed bug using " << FavProgrammingLanguage << std::endl;
        // }

        // void work() {
        //     std::cout << Name << " is writing" << FavProgrammingLanguage << " code!" << std::endl;
        // }
};