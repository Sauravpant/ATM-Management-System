#include<bits/stdc++.h>
using namespace std;

// Transaction class to store details of each transaction
class Transaction {
public:
    string type;   // "Deposit" or "Withdraw"
    double amount;
    double balanceAfterTransaction;

    Transaction(string t, double amt, double bal) {
        type = t;
        amount = amt;
        balanceAfterTransaction = bal;
    } 
};

// Account class to represent bank accounts
class Account {
private:
    string accountNumber, accountHolderName, pin;
    double balance;
    vector<Transaction> transactionHistory;

public:
    Account(string accNumber, string name, string accountPin, double initialBalance) {
        accountNumber = accNumber;
        accountHolderName = name;
        pin = accountPin;
        balance = initialBalance;
    }

    string getAccountNumber() { return accountNumber; }
    string getPIN() { return pin; }
    bool verifyPIN(string enteredPin) { return pin == enteredPin; }
    
    // Changing PIN
    void changePIN(string newPin) {
        pin = newPin;
        cout << "PIN changed successfully." << endl;
    }

    // Deposit operation
    void deposit(double amount) {
        balance += amount;
        transactionHistory.push_back(Transaction("Deposit", amount, balance));
        cout << "Deposit successful. Current balance: $" << balance << endl;
    }

    // Withdraw operation
    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            transactionHistory.push_back(Transaction("Withdrawal", amount, balance));
            cout << "Withdrawal successful. Current balance: $" << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    // Checking balance
    void checkBalance() {
        cout << "Account holder: " << accountHolderName << endl;
        cout << "Current balance: $" << balance << endl;
    }

    // Display the  transaction history
    void showTransactionHistory() {
        if (transactionHistory.empty()) {
            cout << "No transactions yet." << endl;
        } else {
            cout << "Transaction history for " << accountHolderName << ":" << endl;
            for (auto& transaction : transactionHistory) {
                cout << transaction.type << ": $" << transaction.amount 
                 << " | Balance after: $" << transaction.balanceAfterTransaction << endl;
            }
        }
    }

    // Save account details in file
    void saveToFile(ofstream &file) {
        file << accountNumber << "," << accountHolderName << "," << pin << "," << balance << endl;
    }

    // Update balance and PIN of the user in the file
    void updateFile() {
        vector<Account> accounts = loadAccountsFromFile();
        ofstream file("accounts.txt");
        for (Account& acc : accounts) {
            if (acc.getAccountNumber() == this->accountNumber) {
                acc.balance = this->balance;
                acc.pin = this->pin;
            }
            acc.saveToFile(file);
        }
        file.close();
    }

    // Load the accounts stored in  file
    static vector<Account> loadAccountsFromFile() {
        vector<Account> accounts;
        ifstream file("accounts.txt");
        if (!file.is_open()) return accounts;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string accountNumber, name, pin;
            double balance;
            getline(ss, accountNumber, ',');
            getline(ss, name, ',');
            getline(ss, pin, ',');
            ss >> balance;
            accounts.emplace_back(accountNumber, name, pin, balance);
        }
        file.close();
        return accounts;
    }
};

// ATM class to handle multiple accounts
class ATM {
private:
    vector<Account> accounts;

public:
    ATM() {
        // Load all accounts stored in file on ATM initialization
        accounts = Account::loadAccountsFromFile();
    }

    // Create a new account in the bank
    void createNewAccount() {
        string accountNumber, accountHolderName, pin;
        double initialDeposit;
        cout << "Enter account number: "; cin >> accountNumber;
        cout << "Enter account holder name: ";
        cin.ignore();
        getline(cin, accountHolderName);
        cout << "Enter 4-digit PIN: ";
        cin >> pin;
        cout << "Enter initial deposit: ";
        cin >> initialDeposit;

        Account newAccount(accountNumber, accountHolderName, pin, initialDeposit);
        accounts.push_back(newAccount);

        ofstream file("accounts.txt", ios::app);
        if (file.is_open()) {
            newAccount.saveToFile(file);
            file.close();
            cout << "Account created successfully!" << endl;
        }
    }

    // Function to search the account by account number
    Account* findAccount(string accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber()==accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }

    // Function to perform operations 
    void performOperations(Account* account) {
        string pin;
        cout << "Enter PIN: ";
        cin >> pin;

        if (account->verifyPIN(pin)) {
            int choice;
            do {
                cout <<"1. Deposit"<<endl<<"2. Withdraw"<<endl<<"3. Check Balance"<<endl<<"4. Show Transaction History"<<endl<<"5. Change PIN"<<endl<<"6. Exit"<<endl;
                cin >> choice;

                switch (choice) {
                    case 1: {
                        double amount;
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        account->deposit(amount);
                        account->updateFile();
                        break;
                    }
                    case 2: {
                        double amount;
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        account->withdraw(amount);
                        account->updateFile();
                        break;
                    }
                    case 3: account->checkBalance();
                    break;
                    case 4: account->showTransactionHistory();
                    break;
                    case 5: {
                        string newPin;
                        cout << "Enter new PIN: ";
                        cin >> newPin;
                        account->changePIN(newPin);
                        account->updateFile();
                        break;
                    }
                    case 6: cout << "Exiting..." << endl;
                    break;
                    default:
                    cout << "Invalid choice!" << endl;
                }
            } while (choice != 6);
        } else {
            cout << "Incorrect PIN!" << endl;
        }
    }

    // Main menu of ATM
    void mainMenu() {
        int choice;
        do {
            cout << "\n1. Login\n2. Create New Account\n3. Exit\n";
            cin >> choice;
            switch (choice) {
                case 1: {
                    string accountNumber;
                    cout << "Enter account number: "; 
                    cin >> accountNumber;

                    Account* account = findAccount(accountNumber);
                    if (account) {
                        performOperations(account);
                    } else {
                        cout << "Account not found!" << endl;
                    }
                    break;
                }
                case 2: createNewAccount();
                break;
                case 3: cout << "Exiting ATM...\n";
                break;
                default: 
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 3);
    }
};

// Main function 
int main() {
    ATM atm;
    atm.mainMenu();
    return 0;
}
