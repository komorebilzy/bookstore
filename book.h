#ifndef BOOK
#define BOOK

#include "libull.h"
//#include "account.h"
#include <stack>
#include <fstream>
#include <cstring>
#include <utility>

class book {
public:
    String<22> isbn;
    struct detailed {
        String<22> isbn;
        String<64> BookName;
        String<64> author;
        String<64> keyword;
        int quantity;
        double price;
        double totalCost;
    } detail;
    BlockList<int, 22> book_isbn{"book_isbn"};
    BlockList<String<22>, 64> book_name{"book_name"};
    BlockList<String<22>, 64> book_author{"book_author"};
    BlockList<String<22>, 64> book_keyword{"book_keyword"};
    const char *filename = "book_detail";
    std::fstream book_detail;
    int index=0;

    book() {
        std::ifstream in(filename);
        if (!in) {
            std::ofstream out(filename);
            out.close();
        }
        in.close();
        book_detail.open(filename);
    }

    void show_detail(detailed &a){  //keyword输出需要改善
        std::cout << a.isbn << "\t" <<a.BookName << "\t" << a.author << "\t" << a.keyword << "\t"
                  << a.price << "\t" << a.quantity << "\n";
    }

    void show(const char *input_) {
        std::string order = "";
        int i = 1;
        while (input_[i] != '=' || input_[i] != '\0') {
            order += input_[i];
            ++i;
        }
        if (order == "ISBN") {
            std::string tmp = "";
            String<22> isbn_{};
            i++;
            while (input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(isbn_.str, tmp.c_str());
            if (!book_isbn.find(isbn_).empty()) {
                read_book(book_isbn.find(isbn_).back(), detail);
                show_detail(detail);
            }
            else std::cout<<"\n";
        } else if (order == "name") {
            std::string tmp="";
            String<64> name_{};
            i+=2;
            while (input_[i] != '"'||input_[i]!='\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(name_.str,tmp.c_str());
            if(!book_name.find(name_).empty()){
                for(int k=0;i<book_name.find(name_).size();++i){
                    read_book(book_isbn.find(book_name.find(name_)[k]).back(),detail);
                    show_detail(detail);
                }
            }
            else  std::cout << "Invalid\n";
        } else if (order == "author") {
            std::string tmp="";
            String<64> author_{};
            i+=2;
            while (input_[i]!='"'||input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(author_.str,tmp.c_str());
            if(!book_author.find(author_).empty()){
                for(int k=0;i<book_author.find(author_).size();++i){
                    read_book(book_isbn.find(book_author.find(author_)[k]).back(),detail);
                    show_detail(detail);
                }
            }
            else   std::cout << "Invalid\n";
        } else if (order == "keyword") {
            std::string tmp="";
            i+=2;
            while (input_[i]!='"'||input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            std::vector<String<22>> isbn_(book_keyword.show(tmp));
            if(!isbn_.empty()){
                String<22> temp;
                for (int k = 0; k < isbn_.size();k++) {                  //共进行9步
                    for (int j = 0; j < 9 - k;j++) {      //在每一步进行10-i次两两比较
                        if (isbn_[j] > isbn_[j + 1]) {
                            temp = isbn_[j];
                            isbn_[j] = isbn_[j + 1];
                            isbn_[j + 1] = temp;
                        }
                    }
                }
                for (int k = 0; k < isbn_.size();k++){
                    read_book(book_isbn.find(isbn_[k]).back(),detail);
                    show_detail(detail);
                }
            }
        } else if (i == 1) {
            std::vector<int> index_(book_isbn.show_all());
            for(int k=0;k<index_.size();++k){
                read_book(index_[k],detail);
                show_detail(detail);
            }

        } else   std::cout << "Invalid\n";
    }

    void buy(const char *isbn_, int quantity_) {
        strcpy(isbn.str, isbn_);
        if (!book_isbn.find(isbn).empty()) {
            if (quantity_ > 0) {
                read_book(book_isbn.find(isbn).back(), detail);
                detail.quantity -= quantity_;
                std::cout << quantity_ * detail.price << '\n';
                return;
            }
        }
        std::cout << "Invalid\n";
    }

    void read_book(int index_, detailed &detail_) {
        book_detail.seekg(index_ * sizeof(detailed));
        book_detail.read(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void write_book(int index_, detailed &detail_) {
        book_detail.seekp(index_ * sizeof(detailed));
        book_detail.write(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void select(const char *isbn_) {
        strcpy(isbn.str, isbn_);
        if (book_isbn.find(isbn).empty()) {
            book_isbn.insert(isbn,index);
            write_book(index,detail);
            index++;
        }
        current.ISBN=isbn;
        current.selected=true;
    }

    void modify(const char* input_) {
        if (current.selected) {
            std::string order = "";
            int i = 1;
            while (input_[i] != '=' || input_[i] != '\0') {
                order += input_[i];
                ++i;
            }
            if (order == "ISBN") {
                std::string tmp = "";
                String<22> isbn_{};
                i++;
                while (input_[i] != '\0') {
                    tmp += input_[i];
                    ++i;
                }
                strcpy(isbn_.str, tmp.c_str());
                if (isbn_ == current.ISBN)   std::cout << "Invalid\n";
                else if (tmp == "")   std::cout << "Invalid\n";
                int index_ = book_isbn.find(current.ISBN).back();
                book_isbn.change(current.ISBN, index_, isbn_, index_);
                read_book(index_, detail);
                book_name.change(detail.BookName, current.ISBN, detail.BookName, isbn_);
                book_author.change(detail.author, current.ISBN, detail.author, isbn_);
                book_keyword.change(detail.keyword, current.ISBN, detail.keyword, isbn_);

            } else if (order == "name") {
                std::string tmp="";
                String<64> name_{};
                i+=2;
                while(input_[i]!='"'||input_[i]!='\0'){
                    tmp+=input_[i];
                    ++i;
                }
                strcpy(name_.str,tmp.c_str());
                if (tmp == "")  std::cout << "Invalid\n";
                int index_ = book_isbn.find(current.ISBN).back();
                read_book(index_,detail);
                book_name.change(detail.BookName,current.ISBN,name_,current.ISBN);
                detail.BookName=name_;
                write_book(index_,detail);
            }
            else if(order=="author"){
                std::string tmp="";
                String<64> author_{};
                i+=2;
                while(input_[i]!='"'||input_[i]!='\0'){
                    tmp+=input_[i];
                    ++i;
                }
                strcpy(author_.str,tmp.c_str());
                if (tmp == "")  std::cout << "Invalid\n";
                int index_ = book_isbn.find(current.ISBN).back();
                read_book(index_,detail);
                book_author.change(detail.author,current.ISBN,author_,current.ISBN);
                detail.author=author_;
                write_book(index_,detail);
            }
            else if(order=="keyword"){
                std::string tmp="";
                String<64> keyword_{};
                i+=2;
                while(input_[i]!='"'||input_[i]!='\0'){
                    tmp+=input_[i];
                    ++i;
                }
                strcpy(keyword_.str,tmp.c_str());
                if (tmp == "")  std::cout << "Invalid\n";
                //关键字拆分，有重复关键字则throw
                int index_ = book_isbn.find(current.ISBN).back();
                read_book(index_,detail);
                book_keyword.change(detail.keyword,current.ISBN,keyword_,current.ISBN);
                detail.keyword=keyword_;
                write_book(index_,detail);
            }
            else if(order=="price"){
                i++;
                std::string tmp;
                double price_;
                while(input_[i]!='\0'){
                    tmp+=input_[i];
                    ++i;
                }
                int index_ = book_isbn.find(current.ISBN).back();
                read_book(index_,detail);
                price_=std::stod(tmp);
                detail.price=price_;
                write_book(index_,detail);
            }
        }
        else   std::cout << "Invalid\n";
    }

    void import(int quantity_,double totalCost_){
        if(current.selected) {
            if(quantity_<=0||totalCost_<=0)   std::cout << "Invalid\n";
            int index_ = book_isbn.find(current.ISBN).back();
            read_book(index_, detail);
            detail.quantity += quantity_;
            detail.totalCost = totalCost_;
            write_book(index_, detail);
        }
        else  std::cout << "Invalid\n";
    }


};

#endif