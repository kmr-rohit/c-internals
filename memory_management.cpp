// BASIC C types are self alligned , to make access faster because it facilitates generating single instruction. 
// in general struct instance have the allignment of its widest scalar member
// http://www.catb.org/esr/structure-packing/ 

// usecase related to HFT , 
// u have a trade server and you are connected to exchange server and the connection is always like TCP 
// NASDAQ , NSE , HKEQ etc , mandates to use TCP connection rather then REST API's 
// we send bytes on wire which has order information
// 
// for eg:
// 
// #pragma pack(1)
// struct Order{
//     int qty;
//     int prc;
//     char side; // buy or sell 
//     char symbol[6] // ticker symbol
// };
// static_assert(sizeof(Order) == 15);
// int main(){
//     Order myOrder{12 , 100 , 'B' , 'GOOGL'};
//     auto orderStr = reinterpret_cast<char*>(&myOrder); // sending my orders as bytes.
//     // auto order = reinterpret_cast<Order*>(orderStr);// exchanges get my order from bytes 
// }


#include<iostream>
using namespace std;

struct emp{
    char c;
    // char pad[3] - additional padding as int storage address should only start at address divisible by 4 
    int x;
};

struct foo{
    char c;
    // char pad[3]
    int x;
    // char pad[3]
    char d;
};

struct foo_reordered{
    int x; // M
    char c;// M + 4
    char d;// M + 5
    // char pad[2]; this comes out to 8 widest member
};


// C++ introduces prgma parameters to reduce memory usage
#pragma pack(1)
struct foo_packed{
    char c;
    int x;
    char d;
};


struct foo_pragma_packed{
    char c;
    int x;
    char d;
}__attribute__((__packed__));


int main(){
    cout<<"sizeof(int) = "<<sizeof(int)<<endl; // 4 
    cout<<"sizeof(char) = "<<sizeof(char)<<endl; // 1
    cout<<"sizeof(emp) = "<<sizeof(emp)<<endl; // 8
    cout<<"sizeof(foo) = "<<sizeof(foo)<<endl; // 12
    cout<<"sizeof(foo_reordered) = "<<sizeof(foo_reordered)<<endl; // 8 
    cout<<"sizeof(foo_packed) = "<<sizeof(foo_packed)<<endl; // 6 
    cout<<"sizeof(foo_pragma_packed) = "<<sizeof(foo_pragma_packed)<<endl; // 6 used by Hft's 
    
}






