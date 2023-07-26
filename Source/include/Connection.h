//该头文件意图将一些很他妈臭屁的代码给隐藏起来，好让功能代码看上去没那么臭屁
//我的编码风格就这样，凑合着看吧
//Author: CV
//Date: 16/12/2022

#pragma once

//码长 ,若元素很多的话，改动这里，便可提升元素上限
#define EleNum 64

//数据库连接前准备

//数据库连接
#define ConnectionReady \
                            try{ \
                                cout << "正在连接数据库 ... "; \
                                hr = m_pConnection.CreateInstance("ADODB.Connection"); \
                                cout << "... ";\
                                if (SUCCEEDED(hr)){\
                                    hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Animals4Homework.mdb", "", "", adModeUnknown);\
                                    cout << "..." << endl;\
                                    printf("成功连接知识库！\n");\
                                }\
                            }\
                            catch (_com_error e){\
                                cout << "连接知识库失败!原因如下：" << endl;\
                                cout << e.Description() << endl;\
                                return error;\
                            }
//取规则（还有特征）
#define GetRuleData \
                    try{\
                        _bstr_t Ch_sql = "SELECT * FROM Character4Homework;";\
                        _bstr_t Rule_sql = "SELECT * FROM Rule4Homework;";\
                        _bstr_t Animal_sql = "SELECT * FROM Animal4Homework;";\
                        Rules_Recordset = m_pConnection->Execute(Rule_sql, &RecordsSelect_Rules, adCmdText);\
                        Character_Recordset = m_pConnection->Execute(Ch_sql, &RecordsSelect_Character, adCmdText);\
                        Animal_Recordset = m_pConnection->Execute(Animal_sql, &RecordsSelect_Animal, adCmdText);\
                        if(Rules_Recordset->BOF){\
                            cout << "未能取出规则数据，退出！" << endl;\
                            return error;\
                        }\
                        if(Character_Recordset->BOF){\
                            cout << "未能取出特征数据，退出！" << endl;\
                            return error;\
                        }\
                        if(Animal_Recordset->BOF){\
                            cout << "未能取出标签数据，退出！" << endl;\
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

