#include <iostream>

using namespace std;

namespace T{
	enum Type {General,Light, Medium, Heavy};
}
//匿名namespace 所以所有的枚举成员名都默认进入全局名字空间
namespace {
	enum Category{ General = 1,Pistol,MachineGun,Cannon};
}

enum Rp{General=2,Medium};
enum Repeat{General,Medium};
int main()
{
	T::Type t = T::Light;
	if(t == T::General)//忘记使用namespace
		cout<<"General Weapon" <<endl;
	return 0;
}
