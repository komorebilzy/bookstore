#include <iostream>
#include "libull.h"
#include "account.h"
#include <vector>
#include <cstdio>
#include <fstream>
int main() {
//    std::fstream f;
//    f.open("book_name",std::fstream::trunc);
//    f.open("book_isbn",std::fstream::trunc);
//    f.open("book_detail",std::fstream::trunc);
//    f.open("book_keyword",std::fstream::trunc);
//    f.open("book_author",std::fstream::trunc);
//    f.open("bookstore",std::fstream::trunc);
//    f.open("account_name",std::fstream::trunc);
//    f.open("account_detail",std::fstream::trunc);
//    return 0;
    visitor *m;
    visitor v;customer c;worker w;boss b;
    m=&b;
    m->useradd("root","sjtu",7,"root");
    std::string order;
    while(order != "quit" && order!= "exit") {
        std::cin>>order;
        std::string tmp1,tmp2,tmp3;
        if (order == "su") {
            if(current.privilege==0) m=&v;
            else if(current.privilege==1) m=&c;
            else if (current.privilege==3) m=&w;
            else if(current.privilege==7) m=&b;
            std::cin>>tmp1;
            std::string password_;
            char tmp=std::cin.get();
            if (tmp== '\n') {
                m->su(tmp1.c_str());
            }
            else {
                std::cin>>password_;
                m->su(tmp1.c_str(),password_.c_str());
            }
        } else if (order == "logout") {
            if(current.privilege==0) m=&v;
            else if(current.privilege==1) m=&c;
            else if (current.privilege==3) m=&w;
            else if(current.privilege==7) m=&b;
            m->logout();
        }else if(order=="register"){
            if(current.privilege==0) m=&v;
            else if(current.privilege==1) m=&c;
            else if (current.privilege==3) m=&w;
            else if(current.privilege==7) m=&b;
            std::cin>>tmp1>>tmp2>>tmp3;
            m->Register(tmp1.c_str(), tmp2.c_str(), tmp3.c_str());
        }else if(order=="passwd"){
            std::cin>>tmp1>>tmp2;
            char tmp=std::cin.get();
            if(tmp=='\n'){
                m->passwdd(tmp1.c_str(),tmp2.c_str());
            }
            else {
                std::cin>>tmp3;
                m->passwd(tmp1.c_str(),tmp2.c_str(),tmp3.c_str());
            }
        }else if(order=="useradd"){
            int tmp4;
            std::cin>>tmp1>>tmp2>>tmp4>>tmp3;
            m->useradd(tmp1.c_str(),tmp2.c_str(),tmp4,tmp3.c_str());
        }else if(order=="delete"){
            std::cin>>tmp1;
            m->delete_(tmp1.c_str());
        }else if(order=="show"){
            std::cin>>tmp1;
            m->show(tmp1.c_str());
        }else if(order=="buy"){
            int tmp4;
            std::cin>>tmp1>>tmp4;
            m->buy(tmp1.c_str(),tmp4);
        }else if(order=="select"){
            std::cin>>tmp1;
            m->select(tmp1.c_str());
        }else if(order=="modify"){
            char a=std::cin.get();
            while(a!='\n'){
                if(a==' '){m->modify(tmp1.c_str());tmp1="";}
                else tmp1+=a;
                a=std::cin.get();
            }
        }else if(order=="import"){
            int tmp4;double tmp5;
            std::cin>>tmp4>>tmp1;
            tmp5= std::atoi(tmp1.c_str());
            m->import(tmp4,tmp5);
        }
    }
    return 0;
}
