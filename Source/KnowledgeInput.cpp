#include "KnowledgeInput.h"
#include "Connection.h"
#include<iostream>
#include<bitset>
using namespace std;
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace  rename ("EOF", "adoEOF")

int KnowledgeInput::success = 0;
int KnowledgeInput::error=-1;
bool CharacterExist;
bool SymbolExist;

//随便整的一个字符串拼接函数
string InputString(string table, string ch, string code){
    string insert_p1 = "INSERT into ";
    string insert_p2 = "(code, content) values(";
    string comma = ",";
    string insert_p3 = ");";
    string qwt = "'";
    string ret = insert_p1 + table + insert_p2 + qwt + code + qwt + comma + qwt + ch + qwt + insert_p3;
    //
    return ret;
}

//清空表语句拼接
string DeleteString(string table) {
    string sub_del_1 = "DELETE * FROM ";
    string del = sub_del_1 + table + string(";");
    return del;
}

//查询语句拼接
string SelectString(string table, string kw) {
    string sub_slc_1 = "SELECT * FROM ";
    string slc = sub_slc_1 + table + string(" WHERE content='") + kw + string("';");
    return slc;
}

//输入方法
int KnowledgeInput::Input() {
    
    //连接Access前的准备
    CoInitialize(NULL);
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
    _RecordsetPtr Character_Recordset;
    _RecordsetPtr Animal_Recordset;
    HRESULT hr;
    _variant_t RecordsSelect;
    cout << "以下是元素输入准备" << endl;

    //连接数据库（Microsoft Access）
    try
    {
        cout << "正在连接数据库 ... ";
        hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
        cout << "... ";
        if (SUCCEEDED(hr))
        {
            _bstr_t connString_planA = "Provider=Microsoft.ACE.OLEDB.12.0;Driver={Microsoft Access Driver(Animals4Homework.mdb)};DBQ=Animals4Homework.mdb";
            _bstr_t connString_planB = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb";
            hr = m_pConnection->Open(connString_planB, "", "", adModeUnknown);
            cout << "..." << endl;
            printf("成功连接知识库！\n");
        }
    }
    catch (_com_error e) 
    {
        cout << "连接知识库失败!原因如下：" << endl;
        cout << e.Description() << endl;
        return error;
    }

    //实例化结果集对象
    cout << "... ";
    m_pRecordset.CreateInstance(__uuidof(Recordset)); 
    cout << "... " << endl;
    
    //执行SQL语句
    try
    {
        //输入前会清空表项
        _bstr_t Clear_sql = DeleteString("Animal4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        Clear_sql = DeleteString("Character4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        Clear_sql = DeleteString("Rule4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        cout << "基表已清空！" << endl;

        //准备插入标签与特征,逐步左移得到特征号      
        bitset<EleNum> code4Animal;    code4Animal.set(0, 1);
        bitset<EleNum> code4Character;  code4Character.set(EleNum / 2, 1);
        bitset<EleNum> code4Rule;
        bitset<EleNum> tmp_reg;
        int count4Animal = EleNum/2;
        int count4Character = EleNum / 2;
        string ch = "";

        //插入标签
        cout << "正在插入标签数据：" << endl;
        while (count4Animal--) {
            cout << "input {stop} to stop ,otherwise the sign insert:  ";
            cin >> ch;
            if (ch == "stop") break;
            _bstr_t Insert_sql = (InputString("Animal4Homework",ch,code4Animal.to_string())).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Animal = code4Animal << 1;
        }
        cout << "标签输入完毕!" << endl;
        
        //插入特征
        cout << "正在插入特征数据：" << endl;
        while (count4Character--) {
            cout << "input {stop} to stop ,otherwise the character insert:  ";
            cin >> ch;
            if (ch == "stop") break;
            _bstr_t Insert_sql = (InputString("Character4Homework", ch, code4Character.to_string())).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Character = code4Character << 1;
        }
        cout << "特征输入完毕!" << endl;


        //关闭连接
        m_pConnection->Close();
        m_pConnection = NULL;


        //重新连接数据库（Microsoft Access）
        try
        {
            cout << "正在连接数据库 ... ";
            hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
            cout << "... ";
            if (SUCCEEDED(hr))
            {
                hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb", "", "", adModeUnknown);
                cout << "..." << endl;
                printf("成功连接知识库！\n");
            }
        }
        catch (_com_error e)
        {
            cout << "连接知识库失败!原因如下：" << endl;
            cout << e.Description() << endl;
            return error;
        }

        //插入规则
        cout << "正在插入规则数据：" << endl;
        int count=0;
        while (1) {
            count++;
            cout << "input {stop} to stop ,otherwise the rule insert:  " << endl;
            while (1) {
                cout << "No." << count << ": IF x : ";
                cin >> ch;
                if (ch == "stop") break;
                //核验输入合法性
                _bstr_t Charater_Select_sql = (SelectString("Character4Homework", ch)).c_str();
                Character_Recordset = m_pConnection->Execute(Charater_Select_sql, &RecordsSelect, adCmdText);
                CharacterExist = !Character_Recordset->BOF;
                _bstr_t Animal_Select_sql = (SelectString("Animal4Homework", ch)).c_str();                
                Animal_Recordset = m_pConnection->Execute(Animal_Select_sql, &RecordsSelect, adCmdText);
                SymbolExist = !Animal_Recordset->BOF;
                
                bool IsInfoExist = CharacterExist || SymbolExist;
                if (!IsInfoExist) {
                    cout << "找不到相关信息，请重新输入！" << endl;
                    continue;
                }
                //取出查询结果code，做或操作。
                if (CharacterExist) {
                    Character_Recordset->MoveFirst();
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)Character_Recordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    //确定要不要继续输入元素
                }
                else if (SymbolExist) {
                    Animal_Recordset->MoveFirst();
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)Animal_Recordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    //确定要不要继续输入元素
                }
                cout << "Continue ? Y(any) or stop ";
                cin >> ch;
                if (ch != "stop") continue;
                else break;
            }
            while (1) {
                cout << endl;
                cout << "No. " << count << "  THEN x  : ";
                cin >> ch;
                if (ch == "stop") break;
                //核验输入合法性
                _bstr_t Select_sql = (SelectString("Animal4Homework", ch)).c_str();
                m_pRecordset = m_pConnection->Execute(Select_sql, &RecordsSelect, adCmdText);
                if (!m_pRecordset->BOF) {
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)m_pRecordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    break;
                }
                cout << "没有这个动物，请重新输入！" << endl;
            }
            if (ch == "stop") break;
            _bstr_t Insert_sql = (string("INSERT INTO Rule4Homework(code) values(")+ "'" + code4Rule.to_string() + "'" + string(");")).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Rule.reset();
        }
        cout << "规则输入完毕!" << endl;
    }
    catch (_com_error* e)
    {
        cout << (e->ErrorMessage()) << endl;
        if (m_pConnection->State)
        {
            m_pConnection->Close();
            m_pConnection = NULL;
        }
        CoUninitialize();
        return error;
    }

    //关闭连接
    m_pConnection->Close();
    m_pConnection = NULL;

    //返回成功
    return success;
}
