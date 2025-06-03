//
// Created by lozog on 5/23/2025.
//

// BankAccount.h

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>

class BankAccount {
private:
    std::string accountID;
    std::string accountHolder;
    std::string password;
    double balance;

public:
    BankAccount(); // Default constructor
    BankAccount(const std::string& name, const std::string& password); // Used for registration
    BankAccount(const std::string& id, const std::string& name, const std::string& hash, double balance); // Used for loading

    [[nodiscard]] std::string getID() const;
    [[nodiscard]] std::string getAccountHolder() const;
    [[nodiscard]] double getBalance() const;

    void saveToFile(bool overwrite = false) const;

    bool deposit(double amount);
    bool withdraw(double amount);
    bool sendMoney(const std::string& targetID, double amount);

    void deleteAccount() const;

    static bool loadAccount(const std::string& id, const std::string& plainPassword, BankAccount& outAccount);
    static bool loadAccountByID(const std::string& id, BankAccount& outAccount); // New method for money transfers
};

#endif