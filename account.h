#ifndef ACCOUNT
#define ACCOUNT

#include "libull.h"
#include "book.h"
#include <stack>
#include <fstream>
#include <cstring>
#include <utility>

static book a;
class visitor {
protected:

public :
    std::stack<cur> log;
    String<32> UserID;
    struct detailed {
        char PassWord[32];
        char UserName[32];
        int privilege;
    };
    detailed detail;
    static int index;
    BlockList<int, 32> account_name{"account_name"};
    const char *filename = "account_detail";
    std::fstream account_detail;

    visitor() {
        std::ifstream in(filename);
        if (!in) {
            std::ofstream out(filename);
            out.close();
        }
        in.close();
        account_detail.open(filename);
    }

    void read_account(int index_, detailed &detail_) {
        account_detail.seekg(index_ * sizeof(detailed));
        account_detail.read(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void write_account(int index_, detailed &detail_) {
        account_detail.seekp(index_ * sizeof(detailed));       //存帐户所有信息
        account_detail.write(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void Register(const char *userid_, const char *password, const char *username) {
        strcpy(UserID.str, userid_);
        if (account_name.find(UserID).empty()) {
            strcpy(detail.PassWord, password);
            strcpy(detail.UserName, username);
            detail.privilege = 1;
            write_account(index, detail);
            index++;
            account_name.insert(UserID, index);        //存id到所有信息的映射
        } else std::cout << "Invalid\n";
    }

    void su(const char *userid_, const char *password = nullptr) {
        strcpy(UserID.str, userid_);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp.back(), detail);
            if (detail.PassWord == password || detail.privilege > current.privilege) {
                current.privilege = detail.privilege;
                current.UserID = UserID;
                log.push(current);
                return;
            }
        }
        std::cout << "Invalid\n";
    }

    virtual void logout() {
        std::cout << "Invalid\n";
    }

    virtual void passwd(const char *userid_, const char *currentPassword = nullptr, const char *NewPassword = nullptr) {
        std::cout << "Invalid\n";
    }

    virtual void useradd(const char *userid_, const char *password_, int privilege_, const char *username_) {
        std::cout << "Invalid\n";
    }

    virtual void delete_(const char *userid) {
        std::cout << "Invalid\n";
    }

    virtual void passwdd(const char *userid_, const char *NewPassword){
        std::cout << "Invalid\n";
    }

    virtual void show(const char *input_) {
        std::cout << "Invalid\n";
    }

    virtual void buy(const char *isbn_, int quantity_) {
        std::cout << "Invalid\n";
    }

    virtual void select(const char *isbn_) {
        std::cout << "Invalid\n";
    }

    virtual void modify(const char *input_) {
        std::cout << "Invalid\n";
    }

    virtual void import(int quantity_, double totalCost_) {
        std::cout << "Invalid\n";
    }

};

int visitor::index = 0;

class customer : public visitor {
public :
    customer() : visitor() {}

    void logout() {
        if (log.empty()) std::cout << "Invalid\n";
        else {
            log.pop();
            current = log.top();
        }
    }

    void passwd(const char *userid_, const char *currentPassword , const char *NewPassword ) {
        strcpy(UserID.str, userid_);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp.back(), detail);
            if (detail.PassWord == currentPassword) {
                strcpy(detail.PassWord, NewPassword);
                write_account(tmp.back(), detail);
                return;
            }
        }
        std::cout << "Invalid\n";
    }

    virtual void show(const char *input_) {
        a.show(input_);
    }

    virtual void buy(const char *isbn_, int quantity_) {
        a.buy(isbn_, quantity_);
    }
};

class worker : public customer {
public:
    worker() : customer() {}

    void useradd(const char *userid_, const char *password_, int privilege_, const char *username_) {
        if (current.privilege > privilege_) {
            strcpy(UserID.str, userid_);
            std::vector<int> tmp(account_name.find(UserID));
            if (tmp.empty()) {
                strcpy(detail.PassWord, password_);
                strcpy(detail.UserName, username_);
                detail.privilege = privilege_;
                write_account(index, detail);
                index++;
                account_name.insert(UserID, index);
                return;
            }
        }
        std::cout << "Invalid\n";
    }

    virtual void select(const char *isbn_) {
        a.select(isbn_);
    }

    virtual void modify(const char *input_) {
        a.modify(input_);
    }

    virtual void import(int quantity_, double totalCost_) {
        a.import(quantity_, totalCost_);
    }
};

class boss : public worker {
public:
    boss() : worker() {}

    void delete_(const char *userid) {
        strcpy(UserID.str, userid);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            std::stack<cur> temp(log);
            for (int i = 0; i < temp.size(); ++i) {
                if (temp.top().UserID == UserID) {
                    account_name.remove(UserID, tmp.back());
                    return;
                }
                temp.pop();
            }
        }
        std::cout << "Invalid\n";
    }

    void passwdd(const char *userid_, const char *NewPassword) {
        strcpy(UserID.str, userid_);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp.back(), detail);
            strcpy(detail.PassWord, NewPassword);
            write_account(tmp.back(), detail);
            return;
        }
        std::cout << "Invalid\n";
    }
};


#endif