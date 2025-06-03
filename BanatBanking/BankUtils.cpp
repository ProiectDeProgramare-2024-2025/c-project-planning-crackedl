//
// Created by lozog on 5/24/2025.
//

#include "BankUtils.h"
#include <iostream>
#include <random>

std::string BankUtils::generateAccountID() {

    /*  This function generates the AccountID formated as "AL" + 18 randomly generated digits.  */

    std::string accountID = "AL";
    std::random_device rd;  // Hardware random number source (if available)
    std::mt19937 gen(rd()); //Seed Mersenne Twister generator
    std::uniform_int_distribution<> dis(0, 9);
    for (int i = 0; i < 18; i++)
        accountID += std::to_string(dis(gen));
    return accountID;

}

std::string BankUtils::hashPassword(const std::string& password) {

    /* Simple password hashing  */

    std::hash<std::string> hasher;
    size_t hashed = hasher(password);
    return std::to_string(hashed);

}