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

//�������һ���ַ���ƴ�Ӻ���
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

//��ձ����ƴ��
string DeleteString(string table) {
    string sub_del_1 = "DELETE * FROM ";
    string del = sub_del_1 + table + string(";");
    return del;
}

//��ѯ���ƴ��
string SelectString(string table, string kw) {
    string sub_slc_1 = "SELECT * FROM ";
    string slc = sub_slc_1 + table + string(" WHERE content='") + kw + string("';");
    return slc;
}

//���뷽��
int KnowledgeInput::Input() {
    
    //����Accessǰ��׼��
    CoInitialize(NULL);
    _ConnectionPtr m_pConnection;
    _RecordsetPtr m_pRecordset;
    _RecordsetPtr Character_Recordset;
    _RecordsetPtr Animal_Recordset;
    HRESULT hr;
    _variant_t RecordsSelect;
    cout << "������Ԫ������׼��" << endl;

    //�������ݿ⣨Microsoft Access��
    try
    {
        cout << "�����������ݿ� ... ";
        hr = m_pConnection.CreateInstance("ADODB.Connection");///����Connection����
        cout << "... ";
        if (SUCCEEDED(hr))
        {
            _bstr_t connString_planA = "Provider=Microsoft.ACE.OLEDB.12.0;Driver={Microsoft Access Driver(Animals4Homework.mdb)};DBQ=Animals4Homework.mdb";
            _bstr_t connString_planB = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb";
            hr = m_pConnection->Open(connString_planB, "", "", adModeUnknown);
            cout << "..." << endl;
            printf("�ɹ�����֪ʶ�⣡\n");
        }
    }
    catch (_com_error e) 
    {
        cout << "����֪ʶ��ʧ��!ԭ�����£�" << endl;
        cout << e.Description() << endl;
        return error;
    }

    //ʵ�������������
    cout << "... ";
    m_pRecordset.CreateInstance(__uuidof(Recordset)); 
    cout << "... " << endl;
    
    //ִ��SQL���
    try
    {
        //����ǰ����ձ���
        _bstr_t Clear_sql = DeleteString("Animal4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        Clear_sql = DeleteString("Character4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        Clear_sql = DeleteString("Rule4Homework").c_str();
        m_pRecordset = m_pConnection->Execute(Clear_sql, &RecordsSelect, adCmdText);
        cout << "��������գ�" << endl;

        //׼�������ǩ������,�����Ƶõ�������      
        bitset<EleNum> code4Animal;    code4Animal.set(0, 1);
        bitset<EleNum> code4Character;  code4Character.set(EleNum / 2, 1);
        bitset<EleNum> code4Rule;
        bitset<EleNum> tmp_reg;
        int count4Animal = EleNum/2;
        int count4Character = EleNum / 2;
        string ch = "";

        //�����ǩ
        cout << "���ڲ����ǩ���ݣ�" << endl;
        while (count4Animal--) {
            cout << "input {stop} to stop ,otherwise the sign insert:  ";
            cin >> ch;
            if (ch == "stop") break;
            _bstr_t Insert_sql = (InputString("Animal4Homework",ch,code4Animal.to_string())).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Animal = code4Animal << 1;
        }
        cout << "��ǩ�������!" << endl;
        
        //��������
        cout << "���ڲ����������ݣ�" << endl;
        while (count4Character--) {
            cout << "input {stop} to stop ,otherwise the character insert:  ";
            cin >> ch;
            if (ch == "stop") break;
            _bstr_t Insert_sql = (InputString("Character4Homework", ch, code4Character.to_string())).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Character = code4Character << 1;
        }
        cout << "�����������!" << endl;


        //�ر�����
        m_pConnection->Close();
        m_pConnection = NULL;


        //�����������ݿ⣨Microsoft Access��
        try
        {
            cout << "�����������ݿ� ... ";
            hr = m_pConnection.CreateInstance("ADODB.Connection");///����Connection����
            cout << "... ";
            if (SUCCEEDED(hr))
            {
                hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb", "", "", adModeUnknown);
                cout << "..." << endl;
                printf("�ɹ�����֪ʶ�⣡\n");
            }
        }
        catch (_com_error e)
        {
            cout << "����֪ʶ��ʧ��!ԭ�����£�" << endl;
            cout << e.Description() << endl;
            return error;
        }

        //�������
        cout << "���ڲ���������ݣ�" << endl;
        int count=0;
        while (1) {
            count++;
            cout << "input {stop} to stop ,otherwise the rule insert:  " << endl;
            while (1) {
                cout << "No." << count << ": IF x : ";
                cin >> ch;
                if (ch == "stop") break;
                //��������Ϸ���
                _bstr_t Charater_Select_sql = (SelectString("Character4Homework", ch)).c_str();
                Character_Recordset = m_pConnection->Execute(Charater_Select_sql, &RecordsSelect, adCmdText);
                CharacterExist = !Character_Recordset->BOF;
                _bstr_t Animal_Select_sql = (SelectString("Animal4Homework", ch)).c_str();                
                Animal_Recordset = m_pConnection->Execute(Animal_Select_sql, &RecordsSelect, adCmdText);
                SymbolExist = !Animal_Recordset->BOF;
                
                bool IsInfoExist = CharacterExist || SymbolExist;
                if (!IsInfoExist) {
                    cout << "�Ҳ��������Ϣ�����������룡" << endl;
                    continue;
                }
                //ȡ����ѯ���code�����������
                if (CharacterExist) {
                    Character_Recordset->MoveFirst();
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)Character_Recordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    //ȷ��Ҫ��Ҫ��������Ԫ��
                }
                else if (SymbolExist) {
                    Animal_Recordset->MoveFirst();
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)Animal_Recordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    //ȷ��Ҫ��Ҫ��������Ԫ��
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
                //��������Ϸ���
                _bstr_t Select_sql = (SelectString("Animal4Homework", ch)).c_str();
                m_pRecordset = m_pConnection->Execute(Select_sql, &RecordsSelect, adCmdText);
                if (!m_pRecordset->BOF) {
                    string code_found = _com_util::ConvertBSTRToString((_bstr_t)m_pRecordset->GetCollect("code"));
                    bitset<EleNum> bit_reg_1(code_found);
                    code4Rule = code4Rule | bit_reg_1;
                    break;
                }
                cout << "û�����������������룡" << endl;
            }
            if (ch == "stop") break;
            _bstr_t Insert_sql = (string("INSERT INTO Rule4Homework(code) values(")+ "'" + code4Rule.to_string() + "'" + string(");")).c_str();
            cout << Insert_sql << endl;
            m_pRecordset = m_pConnection->Execute(Insert_sql, &RecordsSelect, adCmdText);
            code4Rule.reset();
        }
        cout << "�����������!" << endl;
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

    //�ر�����
    m_pConnection->Close();
    m_pConnection = NULL;

    //���سɹ�
    return success;
}
