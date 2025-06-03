#include "BankAccount.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "BankUtils.h"

const char* RESET   = "\033[0m";
const char* BOLD    = "\033[1m";
const char* RED     = "\033[31m";
const char* GREEN   = "\033[32m";
const char* YELLOW  = "\033[33m";
const char* BLUE    = "\033[34m";

void showUserUsage(const std::string& programName) {
    std::cout << BOLD << BLUE << "=== Banat Bank User Menu ===\n" << RESET;
    std::cout << "Usage:\n";
    std::cout << YELLOW << "View Balance: " << RESET << programName << " <account_id> <password> balance\n";
    std::cout << YELLOW << "Deposit: " << RESET << programName << " <account_id> <password> deposit <amount>\n";
    std::cout << YELLOW << "Withdraw: " << RESET << programName << " <account_id> <password> withdraw <amount>\n";
    std::cout << YELLOW << "Send Money: " << RESET << programName << " <account_id> <password> send <target_id> <amount>\n";
    std::cout << YELLOW << "Delete Account: " << RESET << programName << " <account_id> <password> delete\n";
}

bool loadUserAccount(const std::string& accountID, const std::string& password, BankAccount& account) {
    if (accountID.size() != 20 || accountID.substr(0,2) != "AL") {
        std::cout << RED << "Error: Invalid account ID format.\n" << RESET;
        return false;
    }

    std::ifstream file("accounts.txt");
    if (!file.is_open()) {
        std::cout << RED << "Error: Cannot access account database.\n" << RESET;
        return false;
    }

    std::string line;
    std::string hashedInput = BankUtils::hashPassword(password);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string id, name, fileHash, balanceStr;

        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, fileHash, ',');
        std::getline(ss, balanceStr, ',');

        if (id == accountID && fileHash == hashedInput) {
            double balance = std::stod(balanceStr);
            account = BankAccount(id, name, fileHash, balance);
            file.close();
            return true;
        }
    }

    file.close();
    std::cout << RED << "Error: Invalid credentials or account not found.\n" << RESET;
    return false;
}

void showBalance(const BankAccount& account) {
    std::cout << BOLD << GREEN << "--- Account Information ---\n" << RESET;
    std::cout << "Account Holder: " << account.getAccountHolder() << "\n";
    std::cout << "Account ID: " << account.getID() << "\n";
    std::cout << GREEN << "Current balance: $" << account.getBalance() << RESET << "\n";
}

void depositMoney(BankAccount& account, double amount) {
    if (account.deposit(amount)) {
        std::cout << GREEN << "Deposit successful!\n" << RESET;
    } else {
        std::cout << RED << "Deposit failed.\n" << RESET;
    }
}

void withdrawMoney(BankAccount& account, double amount) {
    if (account.withdraw(amount)) {
        std::cout << GREEN << "Withdrawal successful!\n" << RESET;
    } else {
        std::cout << RED << "Withdrawal failed.\n" << RESET;
    }
}

void sendMoney(BankAccount& account, const std::string& targetID, double amount) {
    if (account.sendMoney(targetID, amount)) {
        std::cout << GREEN << "Money transfer successful!\n" << RESET;
    } else {
        std::cout << RED << "Money transfer failed.\n" << RESET;
    }
}

void deleteAccount(const BankAccount& account) {
    std::cout << YELLOW << "Are you sure you want to delete your account?\n" << RESET;
    std::cout << RED << "This action cannot be undone!\n" << RESET;
    std::cout << "Account holder: " << account.getAccountHolder() << "\n";
    std::cout << "Current balance: $" << account.getBalance() << "\n";
    std::cout << YELLOW << "To confirm deletion, run this command again with 'confirm' at the end.\n" << RESET;
}

void confirmDeleteAccount(const BankAccount& account) {
    account.deleteAccount();
    std::cout << RED << "Account has been permanently deleted.\n" << RESET;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        showUserUsage(argv[0]);
        return 1;
    }

    std::string accountID = argv[1];
    std::string password = argv[2];
    std::string command = argv[3];

    BankAccount userAccount;
    if (!loadUserAccount(accountID, password, userAccount)) {
        return 1;
    }

    std::cout << BOLD << GREEN << "Welcome, " << userAccount.getAccountHolder() << "!\n" << RESET;

    if (command == "balance") {
        showBalance(userAccount);
    }
    else if (command == "deposit") {
        if (argc != 5) {
            std::cout << RED << "Error: Deposit requires amount.\n" << RESET;
            std::cout << "Usage: " << argv[0] << " <account_id> <password> deposit <amount>\n";
            return 1;
        }
        try {
            double amount = std::stod(argv[4]);
            depositMoney(userAccount, amount);
        } catch (const std::exception& e) {
            std::cout << RED << "Error: Invalid amount format.\n" << RESET;
            return 1;
        }
    }
    else if (command == "withdraw") {
        if (argc != 5) {
            std::cout << RED << "Error: Withdraw requires amount.\n" << RESET;
            std::cout << "Usage: " << argv[0] << " <account_id> <password> withdraw <amount>\n";
            return 1;
        }
        try {
            double amount = std::stod(argv[4]);
            withdrawMoney(userAccount, amount);
        } catch (const std::exception& e) {
            std::cout << RED << "Error: Invalid amount format.\n" << RESET;
            return 1;
        }
    }
    else if (command == "send") {
        if (argc != 6) {
            std::cout << RED << "Error: Send requires target account ID and amount.\n" << RESET;
            std::cout << "Usage: " << argv[0] << " <account_id> <password> send <target_id> <amount>\n";
            return 1;
        }
        try {
            std::string targetID = argv[4];
            double amount = std::stod(argv[5]);
            sendMoney(userAccount, targetID, amount);
        } catch (const std::exception& e) {
            std::cout << RED << "Error: Invalid amount format.\n" << RESET;
            return 1;
        }
    }
    else if (command == "delete") {
        if (argc == 5 && std::string(argv[4]) == "confirm") {
            confirmDeleteAccount(userAccount);
        } else {
            deleteAccount(userAccount);
        }
    }
    else {
        std::cout << RED << "Error: Unknown command '" << command << "'\n" << RESET;
        showUserUsage(argv[0]);
        return 1;
    }

    return 0;
}
