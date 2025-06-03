#include "BankAccount.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "BankUtils.h"

const char* RESET   = "\033[0m";
const char* BOLD    = "\033[1m";
const char* RED     = "\033[31m";
const char* GREEN   = "\033[32m";
const char* YELLOW  = "\033[33m";
const char* BLUE    = "\033[34m";

void showUsage(const std::string& programName) {
    std::cout << BOLD << BLUE << "=== Welcome to the Banat Bank Authentication ===\n" << RESET;
    std::cout << "Usage:\n";
    std::cout << YELLOW << "Register: " << RESET << programName << " register <name> <password>\n";
    std::cout << YELLOW << "Login: " << RESET << programName << " login <account_id> <password>\n";
    std::cout << YELLOW << "Example: " << RESET << programName << " register \"John Doe\" mypassword123\n";
    std::cout << YELLOW << "Example: " << RESET << programName << " login AL1234567890123456789 mypassword123\n";
}

void registerUser(const std::string& name, const std::string& password) {
    if (name.empty() || password.empty()) {
        std::cout << RED << "Error: Name and password cannot be empty.\n" << RESET;
        return;
    }

    BankAccount newAcc(name, password);
    newAcc.saveToFile();

    std::cout << GREEN << "Account created successfully!\n" << RESET;
    std::cout << BOLD << "Your Account ID: " << newAcc.getID() << RESET << std::endl;
    std::cout << YELLOW << "Please save this ID - you'll need it to login.\n" << RESET;

    // Save login info for the user menu
    std::cout << "\nTo access your account, use:\n";
    std::cout << BLUE << "./user " << newAcc.getID() << " " << password << RESET << std::endl;
}

bool loginUser(const std::string& accountID, const std::string& password) {
    if (accountID.size() != 20 || accountID.substr(0,2) != "AL") {
        std::cout << RED << "Error: Account ID must be 20 characters, starting with \"AL\".\n" << RESET;
        return false;
    }

    if (password.empty()) {
        std::cout << RED << "Error: Password cannot be empty.\n" << RESET;
        return false;
    }

    std::ifstream file("accounts.txt");
    if (!file.is_open()) {
        std::cout << RED << "Error: No accounts found. Please register first.\n" << RESET;
        return false;
    }

    std::string line;
    std::string hashedInput = BankUtils::hashPassword(password);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string id, fileName, fileHash, balanceStr;

        std::getline(ss, id, ',');
        std::getline(ss, fileName, ',');
        std::getline(ss, fileHash, ',');
        std::getline(ss, balanceStr, ',');

        if (id == accountID && fileHash == hashedInput) {
            file.close();
            std::cout << GREEN << "Login successful!\n" << RESET;
            std::cout << YELLOW << "Starting user session...\n" << RESET;

            // Launch the user menu executable
            std::string command = "./user " + accountID + " " + password;
            int result = system(command.c_str());

            if (result != 0) {
                std::cout << RED << "Error: Could not start user session. Make sure 'user' executable exists.\n" << RESET;
                std::cout << YELLOW << "You can manually run: " << command << RESET << std::endl;
            }

            return true;
        }
    }

    file.close();
    std::cout << RED << "Login failed. Invalid credentials.\n" << RESET;
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "register") {
        if (argc != 4) {
            std::cout << RED << "Error: Register requires name and password.\n" << RESET;
            std::cout << "Usage: " << argv[0] << " register <name> <password>\n";
            return 1;
        }
        registerUser(argv[2], argv[3]);
    }
    else if (command == "login") {
        if (argc != 4) {
            std::cout << RED << "Error: Login requires account ID and password.\n" << RESET;
            std::cout << "Usage: " << argv[0] << " login <account_id> <password>\n";
            return 1;
        }
        loginUser(argv[2], argv[3]);
    }
    else {
        std::cout << RED << "Error: Unknown command '" << command << "'\n" << RESET;
        showUsage(argv[0]);
        return 1;
    }

    return 0;
}