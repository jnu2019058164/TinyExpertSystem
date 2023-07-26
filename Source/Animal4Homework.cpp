#include <iostream>
#include "KnowledgeInput.h"
#include "KnowledgeChaining.h"
using namespace std;
// Author :	Chan Vivon(CV)_陈辉煌
// date:	15/12/2022
// 知识库录入程序
int main() {
	int ret = -1;	//返回号
	string msg;

	cout << "1.输入规则 2.推理（确保先输入规则）" << endl; cin >> msg;
	if(msg=="1")
		KnowledgeInput::Input();	//输入
	KnowledgeChaining::ForwordChaining();	//推理
	system("pause");
	ret = 0;	return ret;		//结束程序
}
