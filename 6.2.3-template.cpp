#include <iostream>
using namespace std;

//变长模板的声明
template<long... nums> struct Multiply;
//递归的偏特化定义
template<long first, long... last>
struct Multiply<first,last...>{
	static const long val = first*Multiply<last...>::val;
};
//边界条件
template<>
struct Multiply<>{
	static const long val = 1;
};


int main(){

	cout << Multiply<2,3,4,5>::val <<endl;
	cout << Multiply<22,44,66,88,9>::val<<endl;

	return 0;
}
