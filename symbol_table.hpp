#pragma once

class SymbolTable
{
public:
    vector<SymbolTable *> children;
    SymbolTable *par;
    set<Var> symtable;
    bool exists(Var v)
    {
        if (symtable.find(v) == symtable.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    string to_string(){
        string s="";
        cout<<"Number of elements here are"<<symtable.size()<<endl;
        for(auto &var:symtable){
            cout<<var.name<<" ";
            s+=var.name;
        }
        cout<<"The final string is"<<s<<" "<<endl;
        return s;
    }
};
class TypeMap
{
   public:
    TypeMap* par;
    vector<TypeMap *> children;
    // Mapping from a variable name to its type 
    map<string, TypeExpr *>mapping;
};