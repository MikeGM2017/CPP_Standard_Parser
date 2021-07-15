/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CPP_Standard_Parcer_main.cpp
 * Author: mike
 *
 * Created on July 9, 2021, 10:31 AM
 */

#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <iosfwd>
#include <iomanip>

using namespace std;

const string Version = "0.13";

enum CT {
    CT_NO,

    CT_SPACE,
    CT_NEWLINE,

    CT_ALPHA,
    CT_NUM,

    CT_MINUS,

    CT_COLON,
    CT_SEMICOLON,

    CT_EQU,

    CT_FL,
    CT_FR,

    CT_APOSTROPHE,
    CT_COMMAS,

    CT_SLASH,
    CT_BACKSLASH,

    CT_EOF,

    CT_LAST,
    CT_UNDEFINED
};

string CT_To_Str(CT t) {
    switch (t) {
        case CT_NO: return "CT_NO";
        case CT_SPACE: return "CT_SPACE";
        case CT_NEWLINE: return "CT_NEWLINE";
        case CT_ALPHA: return "CT_ALPHA";
        case CT_NUM: return "CT_NUM";
        case CT_MINUS: return "CT_MINUS";
        case CT_EQU: return "CT_EQU";
        case CT_COLON: return "CT_COLON";
        case CT_SEMICOLON: return "CT_SEMICOLON";
        case CT_FL: return "CT_FL";
        case CT_FR: return "CT_FR";
        case CT_APOSTROPHE: return "CT_APOSTROPHE";
        case CT_COMMAS: return "CT_COMMAS";
        case CT_SLASH: return "CT_SLASH";
        case CT_BACKSLASH: return "CT_BACKSLASH";
        case CT_EOF: return "CT_EOF";
        case CT_LAST: return "CT_LAST";
        case CT_UNDEFINED: return "CT_UNDEFINED";
        default: return "CT_???";
    }
}

CT CT_Get(char c) {
    if (c == ' ' || c == '\t' || c == '\v' || c == '\f') return CT_SPACE;
    if (c == '\n' || c == '\r') return CT_NEWLINE;
    if (c >= 'a' && c <= 'z') return CT_ALPHA;
    if (c >= 'A' && c <= 'Z') return CT_ALPHA;
    if (c == '_') return CT_ALPHA;
    if (c >= '0' && c <= '9') return CT_NUM;
    if (c == '-') return CT_MINUS;
    if (c == ':') return CT_COLON;
    if (c == ';') return CT_SEMICOLON;
    if (c == '=') return CT_EQU;
    if (c == '{') return CT_FL;
    if (c == '}') return CT_FR;
    if (c == '\'') return CT_APOSTROPHE;
    if (c == '"') return CT_COMMAS;
    if (c == '/') return CT_SLASH;
    if (c == '\\') return CT_BACKSLASH;
    return CT_NO;
}

enum TT {
    TT_NO,

    TT_SPACE,
    TT_NEWLINE,

    TT_ALPHANUM,
    TT_DIGIT,

    TT_COLON,
    TT_SEMICOLON,

    TT_EQU,

    TT_FL,
    TT_FR,

    TT_C_STR,
    TT_S_STR,

    TT_REM_LINE,

    TT_EOF,

    TT_ERROR,

    TT_LAST,
    TT_UNDEFINED
};

string TT_To_Str(TT t) {
    switch (t) {
        case TT_NO: return "TT_NO";
        case TT_SPACE: return "TT_SPACE";
        case TT_NEWLINE: return "TT_NEWLINE";
        case TT_ALPHANUM: return "TT_ALPHANUM";
        case TT_DIGIT: return "TT_DIGIT";
        case TT_COLON: return "TT_COLON";
        case TT_SEMICOLON: return "TT_SEMICOLON";
        case TT_EQU: return "TT_EQU";
        case TT_FL: return "TT_FL";
        case TT_FR: return "TT_FR";
        case TT_C_STR: return "TT_C_STR";
        case TT_S_STR: return "TT_S_STR";
        case TT_REM_LINE: return "TT_REM_LINE";
        case TT_EOF: return "TT_EOF";
        case TT_ERROR: return "TT_ERROR";
        case TT_UNDEFINED: return "TT_UNDEFINED";
        default: return "TT_???";
    }
}

class Token {
public:
    TT Type;
    string Text;

    Token(string text) : Text(text), Type(TT_NO) {
    }

    Token(TT tt, char c) : Type(tt) {
        Text.append(1, c);
    }

    Token(TT tt, string text) : Type(tt), Text(text) {
    }

    Token(TT tt) : Type(tt) {
    }

    void Print() {
        if (Type != TT_NEWLINE) {
            cout << "Token: Type=\"" << TT_To_Str(Type) << "\" Text = \"" << Text << "\"";
        } else {
            cout << "Token: Type=\"" << TT_To_Str(Type) << "\" Text = \"";
            for (int i = 0; i < Text.size(); i++) {
                if (Text[i] == '\n') {
                    cout << "\\n";
                } else if (Text[i] == '\r') {
                    cout << "\\r";
                } else {
                    cout << Text[i];
                }
            }
            cout << "\"";
        }
    }
};

void token_space_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (ct == CT_SPACE) {
                s.append(1, c1);
                c_count++;
            } else {
                f_in.putback(c1);
                tokens.push_back(Token(TT_SPACE, s));
                stop = true;
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

void token_newline_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);

    if (c == '\r') { // Windows newline: "\r\n"
        char c1 = f_in.get();
        if (c1 == '\n') {
            s.append(1, c1);
        } else {
            f_in.putback(c1);
        }
        tokens.push_back(Token(TT_NEWLINE, s));
        return;
    }

    // Linux newline: "\n"
    tokens.push_back(Token(TT_NEWLINE, s));

}

void token_alphanum_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (ct == CT_ALPHA || ct == CT_NUM || ct == CT_MINUS) {
                s.append(1, c1);
                c_count++;
            } else {
                f_in.putback(c1);
                tokens.push_back(Token(TT_ALPHANUM, s));
                stop = true;
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

void token_num_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (ct == CT_NUM) {
                s.append(1, c1);
                c_count++;
            } else {
                f_in.putback(c1);
                tokens.push_back(Token(TT_DIGIT, s));
                stop = true;
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

void token_c_str_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    bool prev_is_backslash = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (ct == CT_APOSTROPHE && !prev_is_backslash) {
                s.append(1, c1);
                c_count++;
                tokens.push_back(Token(TT_C_STR, s));
                stop = true;
            } else if (ct == CT_NEWLINE) {
                f_in.putback(c1);
                tokens.push_back(Token(TT_ERROR, s));
                stop = true;
            } else {
                s.append(1, c1);
                c_count++;
                if (ct == CT_BACKSLASH && !prev_is_backslash) {
                    prev_is_backslash = true;
                } else {
                    prev_is_backslash = false;
                }
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

void token_s_str_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    bool prev_is_backslash = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (ct == CT_COMMAS && !prev_is_backslash) {
                s.append(1, c1);
                c_count++;
                tokens.push_back(Token(TT_S_STR, s));
                stop = true;
            } else if (ct == CT_NEWLINE) {
                f_in.putback(c1);
                tokens.push_back(Token(TT_ERROR, s));
                stop = true;
            } else {
                s.append(1, c1);
                c_count++;
                if (ct == CT_BACKSLASH && !prev_is_backslash) {
                    prev_is_backslash = true;
                } else {
                    prev_is_backslash = false;
                }
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

void token_rem_line_get(ifstream &f_in, char c, vector<Token> &tokens, int &c_count) {
    string s;
    s.append(1, c);
    bool stop = false;
    do {
        char c1 = f_in.get();
        CT ct;
        if (!f_in.eof()) {
            ct = CT_Get(c1);
            if (s.size() == 1 && ct != CT_SLASH) {
                f_in.putback(c1);
                tokens.push_back(Token(TT_ERROR, s));
                stop = true;
            } else if (ct == CT_NEWLINE) {
                f_in.putback(c1);
                tokens.push_back(Token(TT_REM_LINE, s));
                stop = true;
            } else {
                s.append(1, c1);
                c_count++;
            }
        }

        if (f_in.eof()) {
            stop = true;
        }

    } while (!stop);
}

bool Tokens_Get_From_File(string filename, vector<Token> &tokens, int &c_count, string &s_error) {
    bool tokens_res = true;

    ifstream f_in;
    f_in.open(filename.c_str());
    if (f_in.is_open()) {

        bool stop = false;
        do {
            char c = f_in.get();
            CT ct;
            if (!f_in.eof()) {
                c_count++;
                ct = CT_Get(c);
            } else {
                ct = CT_EOF;
                stop = true;
                //tokens.push_back(Token("<eof>"));
            }

            switch (ct) {
                case CT_SPACE:
                    token_space_get(f_in, c, tokens, c_count);
                    break;
                case CT_NEWLINE:
                    token_newline_get(f_in, c, tokens, c_count);
                    break;
                case CT_ALPHA:
                    token_alphanum_get(f_in, c, tokens, c_count);
                    break;
                case CT_NUM:
                    token_num_get(f_in, c, tokens, c_count);
                    break;
                case CT_COLON:
                    tokens.push_back(Token(TT_COLON, c));
                    break;
                case CT_SEMICOLON:
                    tokens.push_back(Token(TT_SEMICOLON, c));
                    break;
                case CT_EQU:
                    tokens.push_back(Token(TT_EQU, c));
                    break;
                case CT_FL:
                    tokens.push_back(Token(TT_FL, c));
                    break;
                case CT_FR:
                    tokens.push_back(Token(TT_FR, c));
                    break;
                case CT_APOSTROPHE:
                    token_c_str_get(f_in, c, tokens, c_count);
                    break;
                case CT_COMMAS:
                    token_s_str_get(f_in, c, tokens, c_count);
                    break;
                case CT_SLASH:
                    token_rem_line_get(f_in, c, tokens, c_count);
                    break;

                case CT_EOF:
                    tokens.push_back(Token(TT_EOF));
                    break;

                case CT_NO:
                case CT_LAST:
                case CT_UNDEFINED:
                default:
                    tokens.push_back(Token(TT_ERROR, c));
                    break;
            }

            //if (f_in.eof()) {
            //    stop = true;
            //}

        } while (!stop);

        f_in.close();

    } else {
        s_error = "Can not open file \"" + filename + "\"";
        tokens_res = false; // Error        
    }

    return tokens_res;
}

bool Try_Skip_Space_Newline_Remline(Token &t, bool &is_prev_space, bool &is_prev_newline, bool &is_prev_newline_space) {
    if (t.Type == TT_SPACE || t.Type == TT_REM_LINE) { // @Attention: REM_Line as Space
        is_prev_space = true;
        if (is_prev_newline) {
            is_prev_newline_space = true;
            is_prev_newline = false;
        } else {
            is_prev_newline_space = false;
            is_prev_newline = false;
        }
        return true; // Skip
    }
    if (t.Type == TT_NEWLINE) {
        is_prev_space = false;
        is_prev_newline_space = false;
        is_prev_newline = true;
        return true; // Skip
    }
    return false; // No Skip
}

enum ET {
    ET_NO,

    ET_DEF,

    ET_LAST,
    ET_UNDEFINED,
};

string ET_To_Str(ET t) {
    switch (t) {
        case ET_NO: return "ET_NO";
        case ET_DEF: return "ET_DEF";
        case ET_LAST: return "ET_LAST";
        case ET_UNDEFINED: return "ET_UNDEFINED";
        default: return "ET_???";
    }
}

class STD_Obj_Abstract {
protected:
    string ID;
    ET Type;
public:

    void ID_Set(string s) {
        ID = s;
    }

    string ID_Get() {
        return ID;
    }

    void Type_Set(ET type) {
        Type = type;
    }

    ET Type_Get() {
        return Type;
    }

    virtual void Print() = 0;

};

class STD_Obj_def_line {
protected:
    vector<string> Items;
public:

    STD_Obj_def_line() {
    }

    size_t Items_Size_Get() {
        return Items.size();
    }

    string Item_Get(size_t index) {
        if (index < Items.size()) {
            return Items[index];
        }
        return "";
    }

    void Item_Add(string s) {
        Items.push_back(s);
    }

};

class STD_Obj_def : public STD_Obj_Abstract {
protected:
    vector<STD_Obj_def_line> Lines;
public:

    STD_Obj_def() {
        Type_Set(ET_DEF);
    }

    void Line_Add() {
        STD_Obj_def_line line;
        Lines.push_back(line);
    }

    size_t Lines_Size_Get() {
        return Lines.size();
    }

    size_t Lines_Items_Size_Get(size_t line_index) {
        if (line_index < Lines.size()) {
            STD_Obj_def_line &line = Lines[line_index];
            return line.Items_Size_Get();
        }
        return 0;
    }

    string Line_Item_Get(size_t line_index, size_t item_index) {
        if (line_index < Lines.size()) {
            STD_Obj_def_line &line = Lines[line_index];
            return line.Item_Get(item_index);
        }
        return "";
    }

    void Line_Item_Add(size_t line_index, string s) {
        if (line_index < Lines.size()) {
            STD_Obj_def_line &line = Lines[line_index];
            line.Item_Add(s);
        }
    }

    void Item_Add(string s) {
        if (Lines.size()) {
            size_t line_last_index = Lines.size() - 1;
            Line_Item_Add(line_last_index, s);
        }
    }

    virtual void Print() {
        cout << "ID: \"" << ID << "\""
                << " Type: \"" << ET_To_Str(Type) << "\""
                << " Lines[" << Lines.size() << "]";
        for (int i = 0; i < Lines.size(); i++) {
            STD_Obj_def_line line = Lines[i];
            cout << endl << "   " << "[";
            cout << setw(2) << i;
            cout << "]";
            cout << "(" << line.Items_Size_Get() << ")";
            for (int j = 0; j < line.Items_Size_Get(); j++) {
                string item = line.Item_Get(j);
                cout << " " << item;
            }
        }
        cout << endl;
    }

};

enum STDEF {
    STDEF_NO,

    STDEF_ID,
    STDEF_COLON,
    STDEF_ITEM,

    STDEF_EOF,

    STDEF_ERROR,

    STDEF_LAST,
    STDEF_UNDEFINED,
};

string STDEF_To_Str(STDEF st) {
    switch (st) {
        case STDEF_NO: return "STDEF_NO";

        case STDEF_ID: return "STDEF_ID";
        case STDEF_COLON: return "STDEF_COLON";
        case STDEF_ITEM: return "STDEF_ITEM";

        case STDEF_EOF: return "STDEF_EOF";

        case STDEF_ERROR: return "STDEF_ERROR";

        case STDEF_LAST: return "STDEF_LAST";
        case STDEF_UNDEFINED: return "STDEF_UNDEFINED";
    }
    return "STDEF_???";
}

bool Try_Get_def(vector<Token> &tokens, int index, int &index_next, STD_Obj_Abstract **object_abstract, string &s_error,
        bool &is_prev_space, bool &is_prev_newline, bool &is_prev_newline_space) {
    bool res_try_get = true;
    STDEF st = STDEF_NO;
    STDEF st_next = STDEF_NO;
    STD_Obj_def *object_def = NULL;

    for (int i = index; i < tokens.size(); i++) {
        index_next = i;

        Token &t = tokens[i];

        bool res_skip = Try_Skip_Space_Newline_Remline(t, is_prev_space, is_prev_newline, is_prev_newline_space);
        if (res_skip) {
            continue; // Skip
        }

        st_next = STDEF_ERROR;

        if (st == STDEF_NO && t.Type == TT_ALPHANUM && !is_prev_space && !is_prev_newline_space) {
            object_def = new STD_Obj_def;
            object_def->ID_Set(t.Text);
            st_next = STDEF_ID;
        } else if (st == STDEF_ID && t.Type == TT_COLON) {
            st_next = STDEF_COLON;
        } else if (st == STDEF_COLON
                && (t.Type == TT_ALPHANUM || t.Type == TT_C_STR || t.Type == TT_S_STR)
                && is_prev_newline_space) {
            object_def->Line_Add();
            object_def->Item_Add(t.Text);
            st_next = STDEF_ITEM;
        } else if (st == STDEF_COLON && t.Type == TT_ALPHANUM && is_prev_space && !is_prev_newline_space) {
            object_def->Line_Add();
            object_def->Item_Add(t.Text);
            st_next = STDEF_ITEM;
        } else if (st == STDEF_ITEM
                && (t.Type == TT_ALPHANUM || t.Type == TT_C_STR || t.Type == TT_S_STR)
                && is_prev_newline_space) {
            object_def->Line_Add();
            object_def->Item_Add(t.Text);
            st_next = STDEF_ITEM;
        } else if (st == STDEF_ITEM
                && (t.Type == TT_ALPHANUM || t.Type == TT_C_STR || t.Type == TT_S_STR)
                && !is_prev_newline) {
            object_def->Item_Add(t.Text);
            st_next = STDEF_ITEM;

        } else if (st == STDEF_ITEM && t.Type == TT_ALPHANUM && is_prev_newline) {
            (*object_abstract) = object_def;
            object_def = NULL;
            index_next--;
            st_next = STDEF_NO;
            break; // Ok

        } else if (st == STDEF_ITEM && t.Type == TT_EOF) {
            (*object_abstract) = object_def;
            object_def = NULL;
            index_next--;
            st_next = STDEF_EOF;
            break; // Ok

        } else {
            st_next = STDEF_ERROR;
        }

        if (st_next == STDEF_ERROR) {
            s_error = STDEF_To_Str(st)
                    + " + token: \"" + t.Text + "\":" + TT_To_Str(t.Type)
                    + " -> " + STDEF_To_Str(st_next)
                    + " - failed";
            res_try_get = false;
        }

        if (!res_try_get) {
            break;
        }

        st = st_next;

    }
    return res_try_get;
}

bool STD_Objects_From_Tokens(vector<Token> &tokens, vector<STD_Obj_Abstract *> &STD_Objects, string &s_error) {
    bool ebnf_objects_res = true;
    bool is_prev_space = false;
    bool is_prev_newline = false;
    bool is_prev_newline_space = false;
    STD_Obj_Abstract *object_abstract = NULL;
    for (int i = 0; i < tokens.size(); i++) {
        Token &t = tokens[i];

        bool res_skip = Try_Skip_Space_Newline_Remline(t, is_prev_space, is_prev_newline, is_prev_newline_space);
        if (res_skip) {
            continue; // Skip
        }

        bool is_processed = false;

        if (!is_processed && t.Type == TT_ALPHANUM) {
            int index_next = i;
            bool res_try_get = Try_Get_def(tokens, i, index_next, &object_abstract, s_error,
                    is_prev_space, is_prev_newline, is_prev_newline_space);
            if (res_try_get && object_abstract) {
                i = index_next;
                STD_Objects.push_back(object_abstract);
                object_abstract = NULL;
                is_processed = true;
                ebnf_objects_res = true;
                continue;
            } else {
                ebnf_objects_res = false;
            }
        }

        if (!ebnf_objects_res) {
            break;
        }

    }
    return ebnf_objects_res;
}

class STD_Graph_Item {
protected:
public:
    string Text;
    int Text_Count;
    STD_Obj_Abstract *Object_Src;
    vector<STD_Obj_Abstract *> Objects_Dst;
    int Pos_Min;
    int Pos_Max;

    STD_Graph_Item() : Text_Count(0) {
    }

    STD_Graph_Item(string text, int count) : Text(text), Text_Count(count),
    Object_Src(NULL) {
    }

};

class STD_Graph {
protected:
    string Name;
    vector<STD_Graph_Item> Items;
public:

    STD_Graph(string name) : Name(name) {
    }

    virtual ~STD_Graph() {
    }

    string Name_Get() {
        return Name;
    }

    size_t Items_Size_Get() {
        return Items.size();
    }

    STD_Graph_Item Item_Get(int index) {
        return Items.at(index);
    }

    void Item_Add(STD_Graph_Item item) {
        Items.push_back(item);
    }

};

void STD_To_Graph(vector<STD_Obj_Abstract *> &STD_Objects, STD_Graph &g) {
    for (int i = 0; i < STD_Objects.size(); i++) {
        STD_Obj_def *obj_def = dynamic_cast<STD_Obj_def *> (STD_Objects[i]);
        if (obj_def) {
            string text = obj_def->ID_Get();
            STD_Graph_Item graph_item_new;
            graph_item_new.Text = text;
            graph_item_new.Text_Count = 0;
            graph_item_new.Object_Src = obj_def;
            graph_item_new.Pos_Min = STD_Objects.size();
            graph_item_new.Pos_Max = 0;
            for (int k = 0; k < STD_Objects.size(); k++) {
                STD_Obj_def *obj_def2 = dynamic_cast<STD_Obj_def *> (STD_Objects[k]);
                if (obj_def2) {
                    bool found = false;
                    for (int line = 0; line < obj_def2->Lines_Size_Get(); line++) {
                        for (int line_item = 0; line_item < obj_def2->Lines_Items_Size_Get(line); line_item++) {
                            string line_item_text = obj_def2->Line_Item_Get(line, line_item);
                            if (line_item_text == text) {
                                graph_item_new.Text_Count++;
                                found = true;
                            }
                        }
                    }
                    if (found) {
                        graph_item_new.Objects_Dst.push_back(obj_def2);
                        if (graph_item_new.Pos_Min > k) {
                            graph_item_new.Pos_Min = k;
                        }
                        if (graph_item_new.Pos_Max < k) {
                            graph_item_new.Pos_Max = k;
                        }
                    }
                }
            }

            g.Item_Add(graph_item_new);
        }
    }
}

void STD_Graph_Item_Print(STD_Graph_Item item) {
    cout << item.Text << "(" << item.Text_Count << ")";
    if (item.Objects_Dst.size()) {
        cout << ": " << item.Object_Src->ID_Get()
                << " -> (" << item.Objects_Dst.size() << ")"
                << "(" << item.Pos_Min << "-" << item.Pos_Max << ")";
        for (int i = 0; i < item.Objects_Dst.size(); i++) {
            STD_Obj_Abstract *obj = item.Objects_Dst[i];
            cout << " " << obj->ID_Get();
        }
    }
}

void STD_Graph_Item_Print_As_Dot(STD_Graph_Item item) {
    if (item.Objects_Dst.size()) {
        for (int i = 0; i < item.Objects_Dst.size(); i++) {
            STD_Obj_Abstract *obj = item.Objects_Dst[i];
            cout << "\"" << item.Object_Src->ID_Get() << "\" -> \"" << obj->ID_Get() << "\";" << endl;
        }
    }
}

void STD_Graph_Print(STD_Graph &g) {
    cout << "Graph " << g.Name_Get() << "[" << g.Items_Size_Get() << "]:" << endl;
    int count_0 = 0;
    for (int i = 0; i < g.Items_Size_Get(); i++) {
        cout << "[" << i << "] ";
        if (g.Item_Get(i).Objects_Dst.size() == 0) {
            count_0++;
        }
        STD_Graph_Item_Print(g.Item_Get(i));
        cout << endl;
    }

    if (count_0) {
        cout << endl;
        cout << "Graph Items without dst[" << count_0 << "]:" << endl;
        for (int i = 0; i < g.Items_Size_Get(); i++) {
            if (g.Item_Get(i).Objects_Dst.size() == 0) {
                cout << "[" << i << "] ";
                STD_Graph_Item_Print(g.Item_Get(i));
                cout << endl;
            }
        }
    }

}

void STD_Graph_Gen(STD_Graph &g) {
    cout << "digraph {" << endl;
    for (int i = 0; i < g.Items_Size_Get(); i++) {
        if (g.Item_Get(i).Objects_Dst.size()) {
            STD_Graph_Item_Print_As_Dot(g.Item_Get(i));
            cout << endl;
        }
    }
    cout << "}" << endl;
}

void Help_Print() {
    cout << "Program Test STD Generator V" << Version << endl;
    cout << "Use: test_ebnf_gen [switches]" << endl;
    cout << "switches:" << endl;
    cout << "-help - show this help;" << endl;
    cout << "-v - show version;" << endl;
    cout << "-f <filename> - set input file name;" << endl;
    cout << "-a - print arguments;" << endl;
    cout << "-c - print char count;" << endl;
    cout << "-t - print tokens;" << endl;
    cout << "-e - print ebnf objects;" << endl;
    cout << "-g - print graph;" << endl;
    cout << "-verbose - verbose output;" << endl;
}

void Version_Print() {
    cout << "Version: V" << Version << endl;
}

int main(int argc, char** argv) {

    bool Arg_Help = false;
    bool Arg_Version = false;
    string Arg_File_In;
    bool Arg_Args_Print = false;
    bool Arg_Char_Count_Print = false;
    bool Arg_Tokens_Print = false;
    bool Arg_STD_Objects_Print = false;
    bool Arg_Graph_Print = false;
    bool Arg_Graph_Gen = false;
    bool Arg_Verbose = false;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string s = argv[i];
            if (s == "-help" || s == "--help" || s == "/help"
                    || s == "-h" || s == "--help" || s == "/h"
                    || s == "-?" || s == "--?" || s == "/?") {
                Arg_Help = true;
            } else if (s == "-v" || s == "--v" || s == "/v"
                    || s == "-version" || s == "--version" || s == "/version") {
                Arg_Version = true;
            } else if (s == "-f") {
                if (i < argc - 1) {
                    i++;
                    string s2 = argv[i];
                    if (s2[0] != '-') {
                        Arg_File_In = s2;
                    } else {
                        cout << "ERROR: switch \"" << s << " without argument" << endl;
                        return 1; // Error
                    }
                } else {
                    cout << "ERROR: switch \"" << s << " without argument" << endl;
                    return 1; // Error
                }
            } else if (s == "-a") {
                Arg_Args_Print = true;
            } else if (s == "-c") {
                Arg_Char_Count_Print = true;
            } else if (s == "-t") {
                Arg_Tokens_Print = true;
            } else if (s == "-e") {
                Arg_STD_Objects_Print = true;
            } else if (s == "-g") {
                Arg_Graph_Print = true;
            } else if (s == "-gen") {
                Arg_Graph_Gen = true;
            } else if (s == "-verbose") {
                Arg_Verbose = true;
            }
        }
    } else {
        Arg_Help = true;
    }

    if (Arg_Help) {
        Help_Print();
        return 0;
    }

    if (Arg_Version) {
        Version_Print();
        return 0;
    }

    if (Arg_Args_Print) {
        cout << "Arg_File_In: \"" << Arg_File_In << "\"" << endl;
    }

    string s_error;

    vector<Token> tokens;
    int c_count = 0;
    bool tokens_res = Tokens_Get_From_File(Arg_File_In, tokens, c_count, s_error);
    if (!tokens_res) {
        if (!s_error.empty()) {
            cout << "ERROR: " << s_error << endl;
        } else {
            cout << "ERROR: process file \"" << Arg_File_In << "\" - undefined error" << endl;
        }
        return 1; // Error;
    }

    if (tokens_res) {

        if (Arg_Char_Count_Print) {
            cout << "Char Count=" << c_count << endl;
        }

        if (Arg_Tokens_Print) {
            cout << "tokens[" << tokens.size() << "]:" << endl;
            if (tokens.size()) {
                for (int i = 0; i < tokens.size(); i++) {
                    Token &token = tokens[i];
                    if (Arg_Verbose || (token.Type != TT_SPACE && token.Type != TT_NEWLINE && token.Type != TT_REM_LINE)) {
                        cout << "[" << i << "] ";
                        tokens[i].Print();
                        cout << endl;
                    }
                }
            }
        }

        vector<STD_Obj_Abstract *> STD_Objects;
        bool ebnf_objects_res = STD_Objects_From_Tokens(tokens, STD_Objects, s_error);
        if (!ebnf_objects_res) {
            if (!s_error.empty()) {
                cout << "ERROR: " << s_error << endl;
            } else {
                cout << "ERROR: process tokens to objects - undefined error" << endl;
            }
            return 1; // Error;
        }

        if (Arg_STD_Objects_Print) {
            cout << "STD_Objects[" << STD_Objects.size() << "]:" << endl;
            for (int i = 0; i < STD_Objects.size(); i++) {
                STD_Obj_Abstract *object_ptr = STD_Objects[i];
                cout << "[" << i << "] ";
                object_ptr->Print();
                //cout << endl;
            }
        }

        STD_Graph Graph("STD Graph");
        STD_To_Graph(STD_Objects, Graph);

        if (Arg_Graph_Print) {
            STD_Graph_Print(Graph);
        }

        if (Arg_Graph_Gen) {
            STD_Graph_Gen(Graph);
        }

    }

    return 0;
}
