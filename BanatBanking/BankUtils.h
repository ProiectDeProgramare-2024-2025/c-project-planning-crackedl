//
// Created by lozog on 5/24/2025.
//

#ifndef BANKUTILS_H
#define BANKUTILS_H
#include <string>


class BankUtils {

    public:
        static std::string generateAccountID();
        static std::string hashPassword(const std::string& password);

};



#endif //BANKUTILS_H
