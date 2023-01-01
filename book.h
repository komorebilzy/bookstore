#ifndef BOOK
#define BOOK

#include "libull.h"
//#include "account.h"
#include <stack>
#include <fstream>
#include <cstring>
#include <utility>
#include <stdio.h>

class book {
public:
    String<22> isbn;
    struct detailed {
        String<22> isbn;
        String<64> BookName;
        String<64> author;
        String<64> keyword;
        int quantity=0;
        double price;
        double totalCost;
    } detail{};
    BlockList<int, 22> book_isbn{"book_isbn"};
    BlockList<String<22>, 64> book_name{"book_name"};
    BlockList<String<22>, 64> book_author{"book_author"};
    BlockList<String<22>, 64> book_keyword{"book_keyword"};
    const char *filename = "book_detail";
    std::fstream book_detail;
    int index = 0;
    std::stack<double> count;

    book() {
        std::ifstream in(filename);
        if (!in) {
            std::ofstream out(filename);
            out.close();
            book_detail.open(filename);
            book_detail.seekp(0);
            book_detail.write(reinterpret_cast<char *>(&index), sizeof(int));
            book_detail.close();
        }
        in.close();
        book_detail.open(filename);
    }

    ~book() {
        book_detail.close();
    }

    void show_detail(detailed &a) {  //keyword输出需要改善
        std::cout << a.isbn << "\t" << a.BookName << "\t" << a.author << "\t" << a.keyword << "\t";
        printf("%.2f\t", a.price);
        std::cout << a.quantity << "\n";
    }

    void show(const char *input_) {
        if (input_ == "all") {
            std::vector<int> index_(book_isbn.show_all());
            if(index_.empty()) std::cout<<'\n';
            for (int k = 0; k < index_.size(); ++k) {
                read_book(index_[k], detail);
                show_detail(detail);
            }
            return;
        }
        std::string order = "";
        int i = 1;
        while (input_[i] != '=' && input_[i] != '\0') {
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
            } else std::cout << "\n";
        } else if (order == "name") {
            std::string tmp = "";
            String<64> name_{};
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(name_.str, tmp.c_str());
            std::vector<String<22>> isbn_(book_name.find(name_));
            if (!isbn_.empty()) {
                for (int k = 0; k < isbn_.size(); ++k) {
                    read_book(book_isbn.find(isbn_[k]).back(), detail);
                    show_detail(detail);
                }
            } else std::cout << "\n";
        } else if (order == "author") {
            std::string tmp = "";
            String<64> author_{};
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(author_.str, tmp.c_str());
            std::vector<String<22>> isbn_(book_author.find(author_));
            if (!isbn_.empty()) {
                for (int k = 0; k < isbn_.size(); ++k) {
                    read_book(book_isbn.find(isbn_[k]).back(), detail);
                    show_detail(detail);
                }
            } else std::cout << "\n";
        } else if (order == "keyword") {
            std::string tmp = "";
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                if(input_[i]=='|'){
                    std::cout<<"Invalid\n";
                    return;
                }
                tmp += input_[i];
                ++i;
            }
            std::vector<String<22>> isbn_(book_keyword.show(tmp));
            if (!isbn_.empty()) {
                String<22> temp{};
                for (int k = 0; k < isbn_.size() - 1; k++) {
                    for (int j = 0; j < isbn_.size() - k - 1; j++) {
                        if (isbn_[j] > isbn_[j + 1]) {
                            temp = isbn_[j];
                            isbn_[j] = isbn_[j + 1];
                            isbn_[j + 1] = temp;
                        }
                    }
                }
                for (int k = 0; k < isbn_.size(); k++) {
                    read_book(book_isbn.find(isbn_[k]).back(), detail);
                    show_detail(detail);
                }
            }
            else std::cout<<"\n";
        } else std::cout << "Invalid\n";
    }


    void read_book(int index_, detailed &detail_) {
        book_detail.seekg((index_-1) * sizeof(detailed)+sizeof(int));
        book_detail.read(reinterpret_cast<char *>(&detail_), sizeof(detailed));
    }

    void write_book(int index_, const detailed &detail_) {
        book_detail.seekp((index_-1) * sizeof(detailed)+sizeof(int));
        book_detail.write(reinterpret_cast<const char *>(&detail_), sizeof(detailed));
    }

    void select(const char *isbn_) {
        strcpy(isbn.str, isbn_);
        if (book_isbn.find(isbn).empty()) {
            book_detail.seekg(0);
            book_detail.read(reinterpret_cast<char*>(&index),sizeof(int));
            index++;
            book_detail.seekp(0);
            book_detail.write(reinterpret_cast<const char*>(&index),sizeof(int));
            book_isbn.insert(isbn, index);
            detailed tmp{};
            tmp.isbn = isbn;
            write_book(index, tmp);
        }
        current.ISBN = isbn;
        current.selected = true;
        current.index = book_isbn.find(isbn).back();
        log.pop();
        log.push(current);
    }

    void modify(const char *input_) {

        std::string order = "";
        int i = 0;
        if(input_[i]=='-') ++i;
        while (input_[i] != '=' && input_[i] != '\0') {
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
            if (!book_isbn.find(isbn_).empty()) std::cout << "Invalid\n";
            else if (tmp == "") std::cout << "Invalid\n";
            else {
                int index_ = current.index;
                read_book(index_, detail);
                book_isbn.change(detail.isbn, index_, isbn_, index_);
                book_name.change(detail.BookName, detail.isbn, detail.BookName, isbn_);
                book_author.change(detail.author, detail.isbn, detail.author, isbn_);
                book_keyword.change(detail.keyword, detail.isbn, detail.keyword, isbn_);
                detail.isbn = isbn_;
                write_book(index_, detail);
                current.ISBN = isbn_;
                log.pop();
                log.push(current);
            }
        } else if (order == "name") {
            std::string tmp = "";
            String<64> name_{};
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(name_.str, tmp.c_str());
            if (tmp == "") std::cout << "Invalid\n";
            else {
                int index_ = current.index;
                read_book(index_, detail);
                book_name.change(detail.BookName, detail.isbn, name_, detail.isbn);
                detail.BookName = name_;
                write_book(index_, detail);
            }
        } else if (order == "author") {
            std::string tmp = "";
            String<64> author_{};
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(author_.str, tmp.c_str());
            if (tmp == "") std::cout << "Invalid\n";
            int index_ = current.index;
            read_book(index_, detail);
            book_author.change(detail.author, detail.isbn, author_, detail.isbn);
            detail.author = author_;
            write_book(index_, detail);
        } else if (order == "keyword") {
            std::string tmp = "";
            String<64> keyword_{};
            i += 2;
            while (input_[i] != '"' && input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            strcpy(keyword_.str, tmp.c_str());
            if (tmp == "") {
                std::cout << "Invalid\n";
                return;
            }
            std::string keyword;
            std::vector<std::string> key;
            for(int k=0;k<tmp.size()+1;++k){
                if(tmp[k]!='|'&&tmp[k]!='\0') keyword+=tmp[k];
                else {
                    for(int j=0;j<key.size();++j){
                        if(keyword==key[j]){
                            std::cout<<"Invalid\n";
                            return;
                        }
                    }
                    key.push_back(keyword);
                    keyword="";
                }
            }
            int index_ = current.index;
            read_book(index_, detail);
            book_keyword.change(detail.keyword, detail.isbn, keyword_, detail.isbn);
            detail.keyword = keyword_;
            write_book(index_, detail);
        } else if (order == "price") {
            i++;
            std::string tmp;
            double price_;
            while (input_[i] != '\0') {
                tmp += input_[i];
                ++i;
            }
            int index_ = current.index;
            read_book(index_, detail);
            price_ = atof(tmp.c_str());
            detail.price = price_;
            write_book(index_, detail);
        }
    }

    void buy(const char *isbn_, int quantity_) {
        strcpy(isbn.str, isbn_);
        std::vector<int> isbnn(book_isbn.find(isbn));
        if (!isbnn.empty()) {
            if (quantity_ > 0) {
                read_book(isbnn.back(), detail);
                if (quantity_ <= detail.quantity) {
                    detail.quantity -= quantity_;
                    write_book(isbnn.back(), detail);
                    count.push(quantity_ * detail.price);
                    printf("%.2f\n", quantity_ * detail.price);
                    return;
                }
            }
        }
        std::cout << "Invalid\n";
    }

    void import(int quantity_, double totalCost_) {
        if (current.selected) {
            if (quantity_ <= 0 || totalCost_ <= 0) std::cout << "Invalid\n";
            else {
                int index_ = current.index;
                read_book(index_, detail);
                detail.quantity += quantity_;
                detail.totalCost += totalCost_;
                write_book(index_, detail);
                count.push(-totalCost_);
            }
        } else std::cout << "Invalid\n";
    }

    void show_finance(int &count_) {
        double count_in = 0, count_out = 0;
        std::stack<double> tmp_count(count);
        if (count_ == 0) {
            for (int i = 0; i < count.size(); ++i) {
                if (tmp_count.top() > 0) count_in += tmp_count.top();
                else count_out -= tmp_count.top();
                tmp_count.pop();
            }
        } else {
            for (int i = 0; i < count_; ++i) {
                if (tmp_count.top() > 0) count_in += tmp_count.top();
                else count_out -= tmp_count.top();
                tmp_count.pop();
            }
        }
        printf("+ %.2f ", count_in);
        printf("- %.2f\n", count_out);
    }


};

#endif