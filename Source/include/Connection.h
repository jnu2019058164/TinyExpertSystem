//��ͷ�ļ���ͼ��һЩ�������ƨ�Ĵ�����������������ù��ܴ��뿴��ȥû��ô��ƨ
//�ҵı�������������պ��ſ���
//Author: CV
//Date: 16/12/2022

#pragma once

//�볤 ,��Ԫ�غܶ�Ļ����Ķ�����������Ԫ������
#define EleNum 64

//���ݿ�����ǰ׼��

//���ݿ�����
#define ConnectionReady \
                            try{ \
                                cout << "�����������ݿ� ... "; \
                                hr = m_pConnection.CreateInstance("ADODB.Connection"); \
                                cout << "... ";\
                                if (SUCCEEDED(hr)){\
                                    hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb", "", "", adModeUnknown);\
                                    cout << "..." << endl;\
                                    printf("�ɹ�����֪ʶ�⣡\n");\
                                }\
                            }\
                            catch (_com_error e){\
                                cout << "����֪ʶ��ʧ��!ԭ�����£�" << endl;\
                                cout << e.Description() << endl;\
                                return error;\
                            }
//ȡ���򣨻���������
#define GetRuleData \
                    try{\
                        _bstr_t Ch_sql = "SELECT * FROM Character4Homework;";\
                        _bstr_t Rule_sql = "SELECT * FROM Rule4Homework;";\
                        _bstr_t Animal_sql = "SELECT * FROM Animal4Homework;";\
                        Rules_Recordset = m_pConnection->Execute(Rule_sql, &RecordsSelect_Rules, adCmdText);\
                        Character_Recordset = m_pConnection->Execute(Ch_sql, &RecordsSelect_Character, adCmdText);\
                        Animal_Recordset = m_pConnection->Execute(Animal_sql, &RecordsSelect_Animal, adCmdText);\
                        if(Rules_Recordset->BOF){\
                            cout << "δ��ȡ���������ݣ��˳���" << endl;\
                            return error;\
                        }\
                        if(Character_Recordset->BOF){\
                            cout << "δ��ȡ���������ݣ��˳���" << endl;\
                            return error;\
                        }\
                        if(Animal_Recordset->BOF){\
                            cout << "δ��ȡ����ǩ���ݣ��˳���" << endl;\
                            return error;\
                        }\
                    }\
                    catch (_com_error* e) {\
                        cout << (e->ErrorMessage()) << endl;\
                        if (m_pConnection->State){\
                            m_pConnection->Close();\
                            m_pConnection = NULL;\
                        }\
                        CoUninitialize();\
                        return error;\
                    }

#define LoadRules \
                Rules_Recordset->MoveFirst();\
                while (!Rules_Recordset->adoEOF) {\
                    _variant_t Rule_code = Rules_Recordset->GetCollect("code");\
                    Rules_Pre.push_back(bitset<EleNum>(_com_util::ConvertBSTRToString((_bstr_t)Rule_code)));\
                    Rules_Recordset->MoveNext();\
                }

#define LoadCharacters \
                Character_Recordset->MoveFirst();\
                while (!Character_Recordset->adoEOF) {\
                    _variant_t Character_code = Character_Recordset->GetCollect("code");\
                    _variant_t Character_content = Character_Recordset->GetCollect("content");\
                    bitset<EleNum> Ch_code(_com_util::ConvertBSTRToString((_bstr_t)Character_code));\
                    string Ch_content = _com_util::ConvertBSTRToString((_bstr_t)Character_content);\
                    Character.insert(pair<string, bitset<EleNum>>(Ch_content, Ch_code));\
                    Character_Recordset->MoveNext();\
                }

#define LoadAnimal \
                Animal_Recordset->MoveFirst();\
                while (!Animal_Recordset->adoEOF) {\
                    _variant_t Animal_code = Animal_Recordset->GetCollect("code");\
                    _variant_t Animal_content = Animal_Recordset->GetCollect("content");\
                    bitset<EleNum> Al_code(_com_util::ConvertBSTRToString((_bstr_t)Animal_code));\
                    string Al_content = _com_util::ConvertBSTRToString((_bstr_t)Animal_content);\
                    Animal.insert(pair<string, bitset<EleNum>>(Al_content, Al_code));\
                    Animal_Recordset->MoveNext();\
                }

