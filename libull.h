#ifndef BLOCK_LIST
#define BLOCK_LIST


#include <cstring>
#include <utility>
#include <vector>
#include <fstream>
#include <stack>

template<int T>
struct String {
    char str[T];

    bool operator<(const String &a) const {
        return strcmp(str, a.str) < 0;
    }

    bool operator==(const String &a) const {
        return strcmp(str, a.str) == 0;
    }

    bool operator!=(const String &a) const {
        return !((*this) == a);
    }

    bool operator<=(const String &a) const {
        return (*this < a || *this == a);
    }

    bool operator>(const String &a) const {
        return strcmp(str, a.str) > 0;
    }

    bool operator>=(const String &a) const {
        return (*this > a || *this == a);
    }

    friend std::istream &operator>>(std::istream &is, String &tmp) {
        return is >> tmp.str;
    }

    friend std::ostream &operator<<(std::ostream &os, const String &tmp) {
        return os << tmp.str;
    }

    String<T> &operator=(const String<T> &rhs) {
        if (this != &rhs) { memcpy(str, rhs.str, sizeof(str)); }
        return *this;
    }
};


struct cur {
    String<32> UserID;
    int privilege;
    String<22> ISBN;
    bool selected = false;
    int index;
} current{};
std::stack<cur> log;

template<class M, int T>
class BlockList {
    static const int max = 800;

    struct Node {
        std::pair<String<T>, M> unit[max];
        int siz;
        int my_num;
        int next_num;

        Node() : siz(0), my_num(0), next_num(0) {};

        Node(int siz_, int my_num_, int next_num_) { siz = siz_, my_num = my_num_, next_num = next_num_; }
    };

    int num = 1;
    int first_index = 1;
    Node head = Node(0, 1, 0);
    std::fstream file;
    const char *name = new char;
public:
    explicit BlockList(const char *filename) : name(filename) {
        std::ifstream in(filename);      //检查文件是否存在
        if (!in) {
            std::ofstream out(filename);    //文件不存在则创建一个文件并写入初值
            out.close();
            file.open(filename);
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&num), sizeof(int));   //块状链表数量num
            file.write(reinterpret_cast<char *>(&first_index), sizeof(int));    //头块的编号first_index
            writeNode(head, head.my_num);
            file.close();
        }
        in.close();
    }

    ~BlockList() = default;

    void writeNode(const Node &a, int index) {
        file.seekp((index - 1) * sizeof(Node) + 2 * sizeof(int));
        file.write(reinterpret_cast<const char *>(&a), sizeof(Node));
    }

    void readNode(Node &a, int index) {
        file.seekg((index - 1) * sizeof(Node) + 2 * sizeof(int));
        file.read(reinterpret_cast<char *>(&a), sizeof(Node));
    }

    void read_basic() {
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&num), sizeof(int));
        file.read(reinterpret_cast<char *>(&first_index), sizeof(int));
    }

    void write_num() {
        file.seekp(0);
        file.write(reinterpret_cast<const char *>(&num), sizeof(int));
    }

    void write_first_index() {
        file.seekp(sizeof(int));
        file.write(reinterpret_cast<const char *>(&first_index), sizeof(int));
    }


    void insert(const String<T> &key, const M &value) {
        file.open(name);
        read_basic();
        readNode(head, first_index);
        Node *p = &head;
        Node temp;
        Node *q = &temp;
        while (p->next_num != 0) {
            readNode(*q, p->next_num);
            if (key < head.unit[0].first) {     //比最小的块还要小
                break;
            }
            if (p->unit[0].first <= key) {     //找块
                if (key < q->unit[0].first ||
                    (key == p->unit[p->siz - 1].first && value <= p->unit[p->siz - 1].second)) {
                    break;
                }
            }
            *p = *q;
        }
        if (p->unit[p->siz - 1].first < key ||                                    //最大的比尾部还大
            (key == p->unit[p->siz - 1].first && value >= p->unit[p->siz - 1].second)) {
            p->unit[p->siz] = std::make_pair(key, value);
            p->siz++;
        } else {
            for (int i = 0; i < p->siz; ++i) {
                if (key < p->unit[i].first || (key == p->unit[i].first && value <= p->unit[i].second)) {
                    for (int j = p->siz - 1; j >= i; j--) {
                        p->unit[j + 1] = p->unit[j];
                    }
                    p->unit[i] = std::make_pair(key, value);
                    p->siz++;
                    break;
                }
            }
        }
        if (p->siz >= max) {                       //裂块
            Node tmp;
            for (int i = 0; i < (max / 2); ++i) {
                tmp.unit[i] = p->unit[i + (max / 2)];
                tmp.siz++;
                p->siz--;
            }
            num++;
            write_num();
            tmp.my_num = num;
            tmp.next_num = p->next_num;
            p->next_num = tmp.my_num;
            writeNode(tmp, tmp.my_num);
        }
        writeNode(*p, p->my_num);
        file.close();
    }

    void remove(const String<T> &key, const M &value) {
        file.open(name);
        read_basic();
        readNode(head, first_index);
        Node *p = &head;
        Node temp;
        Node *q = &temp;
        Node before;
        int flag = 0;      //标记最后一个元素
        while (true) {                      //找块
            if (p->next_num == 0) flag = 1;
            else readNode(*q, p->next_num);
            if ((p->unit[0].first <= key && key <= q->unit[0].first) || flag == 1) {
                for (int i = 0; i < p->siz; ++i) {                  //块内寻找
                    if (p->unit[i].first == key && p->unit[i].second == value) {   //找到了
                        for (int j = i; j < p->siz - 1; ++j) {
                            p->unit[j] = p->unit[j + 1];
                        }
                        p->siz--;
                        if (p->siz == 0) {         //删除空白块
                            if (p->my_num == first_index) {         //head块
                                if (flag == 1) {
                                    writeNode(*p, p->my_num);      //只剩一个块
                                } else {
                                    first_index = p->next_num;
                                    write_first_index();
                                }
                            } else {
                                before.next_num = p->next_num;
                                writeNode(before, before.my_num);
                            }
                        } else writeNode(*p, p->my_num);
                        file.close();
                        return;
                    }
                }
            }
            if (key < p->unit[0].first || (key == p->unit[0].first && value < p->unit[0].second)) break;
            if (flag == 1) break;
            before = *p;           //记录之前的p
            *p = *q;
        }
        file.close();
    }

    std::vector<M> find(const String<T> &key) {
        file.open(name);
        std::vector<M> ans;
        ans.clear();
        read_basic();
        readNode(head, first_index);
        Node *p = &head;
        Node temp;
        Node *q = &temp;
        int flag = 0;      //标记最后一个元素
        while (true) {                      //找块
            if (p->next_num == 0) flag = 1;
            else {
                readNode(*q, p->next_num);
            }
            if ((p->unit[0].first <= key && key <= q->unit[0].first) || flag == 1) {
                for (int i = 0; i < p->siz; ++i) {                  //块内寻找
                    if (p->unit[i].first == key) {   //找到了
                        ans.push_back(p->unit[i].second);
                    }
                }
            }
            if (flag == 1) break;
            if (key < p->unit[0].first) break;
            *p = *q;
        }
        file.close();
        return ans;
    }

    void change(const String<T> &key, M value, const String<T> &new_key, M new_value) {
        remove(key, value);
        insert(new_key, new_value);
//        file.open(name);
//        read_basic();
//        readNode(head, first_index);
//        Node *p = &head;
//        Node temp;
//        Node *q = &temp;
//        int flag = 0;      //标记最后一个元素
//        while (true) {                      //找块
//            if (p->next_num == 0) flag = 1;
//            else readNode(*q, p->next_num);
//            if ((p->unit[0].first <= key && key <= q->unit[0].first) || flag == 1) {
//                for (int i = 0; i < p->siz; ++i) {                  //块内寻找
//                    if (p->unit[i].first == key && p->unit[i].second == value) {   //找到了
//                        p->unit[i].first=new_key;
//                        p->unit[i].second=new_value;
//                        writeNode(*p,p->my_num);
//                    }
//                }
//            }
//            if (key < p->unit[0].first || (key == p->unit[0].first && value < p->unit[0].second)) break;
//            if (flag == 1) break;
//            *p = *q;
//        }
//        file.close();
    }

    std::vector<M> show(const std::string &key) {
        file.open(name);
        std::vector<M> ans;
        ans.clear();
        read_basic();
        readNode(head, first_index);
        Node *p = &head;
        Node temp;
        Node *q = &temp;
        int flag = 0;      //标记最后一个元素
        while (true) {                      //找块
            if (p->next_num == 0) flag = 1;
            else {
                readNode(*q, p->next_num);
            }
            for (int i = 0; i < p->siz; ++i) {                  //块内寻找
                std::string tmp,keyword="";
               tmp=std::string (p->unit[i].first.str);
                for(int k=0;k<tmp.size()+1;++k){
                    if(tmp[k]!='|'&&tmp[k]!='\0') keyword+=tmp[k];
                    else {
                        if(keyword==key) {
                            ans.push_back(p->unit[i].second);
                            break;
                        }
                        keyword="";
                        continue;
                    }
                }
            }
            if (flag == 1) break;
            *p = *q;
        }
        file.close();
        return ans;
    }

    std::vector<M> show_all(){
        file.open(name);
        std::vector<M> ans;
        ans.clear();
        read_basic();
        readNode(head, first_index);
        Node *p = &head;
        Node temp;
        Node *q = &temp;
        int flag = 0;      //标记最后一个元素
        while (true) {                      //找块
            if (p->next_num == 0) flag = 1;
            else {
                readNode(*q, p->next_num);
            }
            for (int i = 0; i < p->siz; ++i) {                  //块内寻找
                ans.push_back(p->unit[i].second);
            }
            if (flag == 1) break;
            *p = *q;
        }
        file.close();
        return ans;
    }
};

#endif
