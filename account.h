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
    String<32> UserID;
    struct detailed {
        String<32> PassWord;
        String<32> UserName;
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
            account_detail.open(filename);
            account_detail.seekp(0);
            account_detail.write(reinterpret_cast<char *>(&index), sizeof(int));
            account_detail.close();
        }
        in.close();
    }

    void read_account(int index_, detailed &detail_) {
        account_detail.seekg((index_ - 1) * sizeof(detailed)+sizeof(int));
        account_detail.read(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void write_account(int index_, const detailed &detail_) {
        account_detail.seekp((index_ - 1) * sizeof(detailed)+sizeof(int));       //存帐户所有信息
        account_detail.write(reinterpret_cast<const char *>(&detail_), sizeof(detailed));
    }

    virtual void Register(const char *userid_, const char *password, const char *username) {
//        if(log.empty()) {
//            std::cout<<"Invalid\n";
//            return;
//        }
        account_detail.open(filename);
        strcpy(UserID.str, userid_);
        if (account_name.find(UserID).empty()) {
            strcpy(detail.PassWord.str, password);
            strcpy(detail.UserName.str, username);
            detail.privilege = 1;
            account_detail.seekg(0);
            account_detail.read(reinterpret_cast<char*>(&index),sizeof(int));
            index++;
            account_detail.seekp(0);
            account_detail.write(reinterpret_cast<const char *>(&index), sizeof(int));
            write_account(index, detail);
            account_name.insert(UserID, index);        //存id到所有信息的映射
        } else std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void su(const char *userid_, const char *password_ = nullptr) {
        account_detail.open(filename);
        strcpy(UserID.str, userid_);
        String<32> password{};
        if(password_!= nullptr)  strcpy(password.str,password_);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp[0], detail);
            if (detail.PassWord == password || (detail.privilege < current.privilege&&!log.empty())) {
                current.privilege = detail.privilege;
                current.UserID = UserID;
                log.push(current);
                account_detail.close();
                return;
            }
        }
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void logout() {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void passwd(const char *userid_, const char *currentPassword = nullptr, const char *NewPassword = nullptr) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void useradd(const char *userid_, const char *password_, int privilege_, const char *username_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void delete_(const char *userid) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void passwdd(const char *userid_, const char *NewPassword) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void show(const char *input_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void buy(const char *isbn_, int quantity_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void select(const char *isbn_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void modify(const char *input_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void import(int quantity_, double totalCost_) {
        account_detail.open(filename);
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void show_finance(int count_){
        std::cout << "Invalid\n";
    }
};

int visitor::index = 0;

class customer : public visitor {
public :
    customer() : visitor() {}

    void logout() {
        account_detail.open(filename);
        if (log.empty()) std::cout << "Invalid\n";
        else {
            log.pop();
            if(!log.empty()) current=log.top();
        }
        account_detail.close();
    }

   virtual void passwd(const char *userid_, const char *currentPassword, const char *NewPassword){
        account_detail.open(filename);
        strcpy(UserID.str, userid_);
        String<32> cp{};
        strcpy(cp.str,currentPassword);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp.back(), detail);
            if (detail.PassWord == cp) {
                strcpy(detail.PassWord.str, NewPassword);
                write_account(tmp.back(), detail);
                account_detail.close();
                return;
            }
        }
        std::cout << "Invalid\n";
        account_detail.close();
    }

    virtual void show(const char *input_) {
        account_detail.open(filename);
        a.show(input_);
        account_detail.close();
    }

    virtual void buy(const char *isbn_, int quantity_) {
        account_detail.open(filename);
        a.buy(isbn_, quantity_);
        account_detail.close();
    }
};

class worker : public customer {
public:
    worker() : customer() {}

    void useradd(const char *userid_, const char *password_, int privilege_, const char *username_) {
        account_detail.open(filename);
        std::ifstream in("judge");
        if (current.privilege > privilege_ || !in) {
            strcpy(UserID.str, userid_);
            std::vector<int> tmp(account_name.find(UserID));
            if (tmp.empty()) {
                strcpy(detail.PassWord.str, password_);
                strcpy(detail.UserName.str, username_);
                detail.privilege = privilege_;
                account_detail.seekg(0);
                account_detail.read(reinterpret_cast<char*>(&index),sizeof(int));
                index++;
                account_detail.seekp(0);
                account_detail.write(reinterpret_cast<const char *>(&index), sizeof(int));
                write_account(index, detail);
                account_name.insert(UserID, index);
                account_detail.close();
                return;
            }
        }
        in.close();
        std::cout << "Invalid\n";
        account_detail.close();
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
        account_detail.open(filename);
        strcpy(UserID.str, userid);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            std::stack<cur> temp(log);
            for (int i = 0; i < log.size(); ++i) {
                if (temp.top().UserID == UserID) {
                    std::cout << "Invalid\n";
                    account_detail.close();
                    return;
                }
                temp.pop();
            }
            account_name.remove(UserID, tmp.back());
        }
        else std::cout << "Invalid\n";
        account_detail.close();
    }

    void passwdd(const char *userid_, const char *NewPassword) {
        account_detail.open(filename);
        strcpy(UserID.str, userid_);
        std::vector<int> tmp(account_name.find(UserID));
        if (!tmp.empty()) {
            read_account(tmp.back(), detail);
            strcpy(detail.PassWord.str, NewPassword);
            write_account(tmp.back(), detail);
            account_detail.close();
            return;
        }
        std::cout << "Invalid\n";
        account_detail.close();
    }
    void show_finance(int count_){
        a.show_finance(count_);
    }
};


#endif