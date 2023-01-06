#include<fstream>
#include <cstring>
#include <iostream>
class count{
public:
    std::fstream counts;
    int index2=0;
    char inform[100];
count(){
    std::ifstream in2("counts");
    if (!in2) {
        std::ofstream out2("counts");
        out2.close();
        counts.open("counts");
        counts.seekp(0);
        counts.write(reinterpret_cast<const char *>(&index2), sizeof(int));
        counts.close();
    }
    in2.close();
}
void writes(const char *inform_){
    strcpy(inform,inform_);
    counts.open("counts");
    counts.seekg(0);
    counts.read(reinterpret_cast<char *>(&index2),sizeof(int));
    index2++;
    counts.seekp(0);
    counts.write(reinterpret_cast<char*>(&index2),sizeof(int));
    counts.seekp(sizeof(int)+(index2-1)*sizeof(char[100]));
    counts.write(reinterpret_cast<const char*>(&inform),sizeof(char[100]));
    counts.close();

}

void log(){
    counts.open("counts");
    counts.seekg(0);
    counts.read(reinterpret_cast<char*>(&index2),sizeof(int));
    for(int i=0;i<index2;++i){
        counts.seekg(sizeof(int)+i*sizeof(char[100]));
        counts.read(reinterpret_cast<char *>(&inform),sizeof(char[100]));
        std::cout<<inform<<std::endl;
    }
}
};