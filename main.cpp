#include <iostream>
#include "libull.h"
#include "account.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <cstring>
int main() {
//    int timer=0;
    visitor *m;
    visitor v;customer c;worker w;boss b;
    m=&b;
    std::ifstream in("judge");
    if (!in) {
        m->useradd("root","sjtu",7,"root");
        std::ofstream out("judge");
        out.close();
    }
    in.close();
    std::string order;
    while(std::cin>>order) {
        if(std::cin.eof()) return 0;
        std::string tmp1,tmp2,tmp3;
        if (order == "su") {
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
            if(!log.empty())  current=log.top();
            if(current.privilege==0||log.empty()) m=&v;
            else if(current.privilege==1) m=&c;
            else if (current.privilege==3) m=&w;
            else if(current.privilege==7) m=&b;
        } else if (order == "logout") {
            m->logout();
            if(current.privilege==0||log.empty()) m=&v;
            else if(current.privilege==1) m=&c;
            else if (current.privilege==3) m=&w;
            else if(current.privilege==7) m=&b;

        }else if(order=="register"){
            std::string tmp[5];
            int i=0;
            while(std::cin.get()!='\n'){
                std::cin>>tmp[i];
                ++i;
            }
            if(i!=3) {
                std::cout<<"Invalid\n";
                continue;
            }
//            if(i>3){
//                for(int j=3;j<i;++j){
//                    tmp[2]+=" ";
//                    tmp[2]+=tmp[j];
//                }
//            }
            m->Register(tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
        }else if(order=="passwd"){
            std::cin>>tmp1>>tmp2;
            char tmp=std::cin.get();
            if(tmp=='\n'){
                if(current.privilege==7&&!log.empty()) {
                    m->passwdd(tmp1.c_str(), tmp2.c_str());
                }
                else std::cout<<"Invalid\n";
            }
            else {
                std::cin>>tmp3;
                m->passwd(tmp1.c_str(),tmp2.c_str(),tmp3.c_str());
            }
        }else if(order=="useradd"){
            std::string tmper[4]={"","","",""};
            int i=0;
            while(std::cin.get()!='\n'){
                std::cin>>tmper[i];
                ++i;
            }
            if(tmper[3]==""||i!=4) {
                std::cout<<"Invalid\n";
                continue;
            }
            else {
                try {
                    int tmp = atoi(tmper[2].c_str());
                    m->useradd(tmper[0].c_str(), tmper[1].c_str(), tmp, tmper[3].c_str());
                }catch(...){
                    std::cout<<"Invalid\n";
                }
            }
        }else if(order=="delete"){
            std::cin>>tmp1;
            m->delete_(tmp1.c_str());
        }else if(order=="show"){
            char a=std::cin.get();
            if(a=='\n') m->show("all");
            else {
                std::cin >> tmp1;
                if(tmp1=="finance") {
                    char b=std::cin.get();
                    if(b=='\n') m->show_finance(0);
                    else {
                        int tmp4;
                        std::cin>>tmp4;
                        m->show_finance(tmp4);
                    }
                }
                else m->show(tmp1.c_str());
            }
        }else if(order=="buy"){
            int tmp4;
            try {
                std::cin >> tmp1 >> tmp4;
                m->buy(tmp1.c_str(), tmp4);
            }
            catch(...){
                std::cout<<"Invalid\n";
                continue;
            }
        }else if(order=="select"){
            std::cin>>tmp1;
            m->select(tmp1.c_str());
        }else if(order=="modify"){
            if(!current.selected||current.privilege<=1||log.empty()) {
                while(std::cin.get()!='\n'){
                    std::cin>>tmp1;
                }
                std::cout<<"Invalid\n";
                continue;
            }
            else {
                bool flag=0;
                char a=std::cin.get();
                while (a != '\n') {
                    if (a == ' ' && !flag) flag = 1;
                    else if (a == ' ') {
                        m->modify(tmp1.c_str());
                        tmp1 = "";
                    }
                    else tmp1 += a;
                    a = std::cin.get();
                }
                m->modify(tmp1.c_str());
            }
        }else if(order=="import"){
            int tmp4;double tmp5;
            std::cin>>tmp1>>tmp2;
            try {
                tmp4 = atoi(tmp1.c_str());
                tmp5 = std::stod(tmp2.c_str());
            }catch (...){
                std::cout<<"Invalid\n";
                continue;
            }
            m->import(tmp4,tmp5);
        }
        else if(order=="exit"||order=="quit"){
            return 0;
        }
        else {
            while(std::cin.get()!='\n'){
                std::cin>>tmp1;
            }
            std::cout<<"Invalid\n";
        }
    }
    return 0;
}
