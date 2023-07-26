//Author: CV
//Date: 16/12/2022
//KnowledgeChaining Implementation
//书写位置打了断点，看右边红色点

#include "KnowledgeChaining.h"
#include "Connection.h"
#include<iostream>
#include<bitset>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace  rename ("EOF", "adoEOF")

//返回号
int KnowledgeChaining::success = true;
int KnowledgeChaining::error = false;

//待定数据
bitset<EleNum> CodeReg[3];
bitset<EleNum> GetAnimalCode = -1;
vector<bitset<EleNum>> Rules_Pre;
vector<bitset<EleNum>> Rules_Visited;
vector<bitset<EleNum>> Facts;
vector<string> Result;
map<string, bitset<EleNum>> Character;
map<string, bitset<EleNum>> Animal;
string Name4X;  string ChInfo;

//连接Access前的准备
 _ConnectionPtr m_pConnection; _RecordsetPtr Rules_Recordset; _RecordsetPtr Character_Recordset; _RecordsetPtr Animal_Recordset; HRESULT hr; _variant_t RecordsSelect_Rules; _variant_t RecordsSelect_Animal; _variant_t RecordsSelect_Character;


 bool bit_Greater(bitset<EleNum> a, bitset<EleNum> b) {
     int count = 32;
     while (count--) {
         if (b.test(count))
             return false;
         else if (a.test(count))
             return true;
     }
     throw "Bitset compare exception!\n";
 }

int KnowledgeChaining::ForwordChaining() {
    
    CoInitialize(NULL);
    //数据库连接
    ConnectionReady;

    //实例化结果集对象
    cout << "... "; Rules_Recordset.CreateInstance(__uuidof(Recordset));  cout << "... " << endl;
    cout << "... "; Character_Recordset.CreateInstance(__uuidof(Recordset));  cout << "... " << endl;
    
    //取数据
    GetRuleData;

    //装填数据到内存
    LoadRules;
    LoadCharacters;
    LoadAnimal;

    //输入已知事实
    cout << "The name of X is ? : ";
    cin >> Name4X;
    cout << endl;
    while (ChInfo != "stop") {
        cout << "How about " << Name4X << " ? (input features or {stop} to stop)";
        cin >> ChInfo;
        if (ChInfo == "stop") break;
        auto Character_iter = Character.find(ChInfo);
        auto Animal_iter = Animal.find(ChInfo);
        bool IsInfoError = Character_iter == Character.end() xor Animal_iter == Animal.end();
        if (!IsInfoError) {
            cout << "No infomation found ！" << endl;
            continue;
        }
        if(Character_iter != Character.end())
            Facts.push_back(Character_iter->second);
        else if(Animal_iter == Animal.end())
            Facts.push_back(Animal_iter->second);
    }

    if (Facts.empty()) {
        cout << "No facts input! Quit!" << endl;
        return error;
    }
    auto IsFact = Facts.back();

    cout << "start inferencing!" << endl;

    //开始进行正向推理
    GetAnimalCode = GetAnimalCode >> (EleNum / 2);
    while (!Rules_Pre.empty()) {
        //顺序访问规则
        Rules_Visited.push_back(Rules_Pre.back());
        Rules_Pre.pop_back();
        //寄存器赋值当前规则
        CodeReg[0] = Rules_Visited.back();
        //遍历事实
        auto Facts_iter = Facts.size();
        while (Facts_iter--){
            CodeReg[1] = Facts[Facts_iter];
            CodeReg[2] = CodeReg[0] & CodeReg[1];   //相与得出相关性
            if (!CodeReg[2].none() and CodeReg[0].count() > Facts.back().count() and bit_Greater(CodeReg[0],Facts.back())){
                Facts.push_back(CodeReg[0]);
            }
        }

    }

    //得出结论
    auto Aniaml_iter = Animal.begin();
    while (Aniaml_iter != Animal.end()) {
        CodeReg[1] = Aniaml_iter->second & Facts.back();
        if (!CodeReg[1].none() and !bit_Greater(Facts.back(),Aniaml_iter->second)) {
            Result.push_back(Aniaml_iter->first);
        }
        if (Aniaml_iter == Animal.end()) break;
        Aniaml_iter++;
    }
    if (Result.empty()) {
        cout << "\t  Nil" << endl;
        return error;
    }
    else {
        cout << Name4X << " " << Result.back() << endl;
    }
    //返回成功
	return success;
}
