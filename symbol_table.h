
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <string>
using namespace std;

class SymbolInfo
{
private:
    string name, type;
    string ret_type;
    string dataType;
    bool isArray;

    int param_count;
    string *param_types;
    bool *param_is_array;

    int field_count;
    string *field_types;
    string *field_names;
    SymbolInfo *next;

public:
    SymbolInfo(string n, string t, string dt="", bool arr=false)
    : name(n), type(t), dataType(dt), isArray(arr),
      ret_type(""), param_count(0), param_types(nullptr),param_is_array(nullptr),
      field_count(0), field_types(nullptr), field_names(nullptr),
      next(nullptr) {}

    // For FUNCTION 
    SymbolInfo(const string &n, const string &t,
               const string &ret, int pc, const string *pt, const bool *pa = nullptr)
        : name(n), type(t), ret_type(ret), param_count(pc),
          field_count(0), field_types(nullptr), field_names(nullptr),
          next(nullptr)
    {
        if (pc > 0) {
            param_types = new string[pc];
            param_is_array = new bool[pc];
            for (int i = 0; i < pc; i++) {
                param_types[i] = pt[i];
                param_is_array[i] = (pa != nullptr) ? pa[i] : false;
            }
        } else {
            param_types = nullptr;
            param_is_array = nullptr;
        }
    }


    // For STRUCT/UNION
    SymbolInfo(const string &n, const string &t,
               int fc, const string *ft, const string *fn)
        : name(n), type(t),
          ret_type(""), param_count(0), param_types(nullptr),param_is_array(nullptr),
          field_count(fc), next(nullptr)
    {
        if (fc > 0)
        {
            field_types = new string[fc];
            field_names = new string[fc];
            for (int i = 0; i < fc; i++)
            {
                field_types[i] = ft[i];
                field_names[i] = fn[i];
            }
        }
        else
        {
            field_types = field_names = nullptr;
        }
    }

    string getDisplayString() const
    {
        if (type == "FUNCTION")
        {
            // Format: <name,FUNCTION,rettype<==(p1,p2,...)>
            // string s = ret_type + "<==(";
            // for (int i = 0; i < param_count; i++)
            // {
            //     if (i > 0) 
            //     {
            //         s += ",";
            //     }
            //     s += param_types[i];
            // }
            // s += ")";
            return "< " + name + " , " + "ID" + " >";
        }
        else if (type == "STRUCT" || type == "UNION")
        {
            // Format: <name,STRUCT,{(type1,var1),(type2,var2),...}>
            string s = "{";
            for (int i = 0; i < field_count; i++)
            {
                if (i > 0)
                    s += ",";
                s += "(" + field_types[i] + "," + field_names[i] + ")";
            }
            s += "}";
            return "<" + name + "," + type + "," + s + "> ";
        }
        else
        {
            return "< " + name + " , " + type + " > ";
        }
    }

    void setName(string n)
    {
        name = n;
    }

    void setType(string t)
    {
        type = t;
    }

    string getName()
    {
        return name;
    }
    string getType()
    {
        return type;
    }
    SymbolInfo *getNext()
    {
        return next;
    }
    void setNext(SymbolInfo *n)
    {
        next = n;
    }

    string getReturnType()
    {
        return ret_type;
    }

    int getParamCount()
    {
        return param_count;
    }

    string getParamType(int i)
    {
        return param_types[i];
    }

    int getFieldCount()
    {
        return field_count;
    }

    string getFieldType(int i)
    {
        if (i >= 0 && i < field_count)
            return field_types[i];

        return "";
    }

    string getFieldName(int i)
    {
        if (i >= 0 && i < field_count)
            return field_names[i];

        return "";
    }
    string getDataType() { return dataType; }

    bool getParamIsArray(int i) const {
        if (i >= 0 && i < param_count && param_is_array != nullptr) {
            return param_is_array[i];
        }
        return false;
    }

    bool getIsArray()  { return isArray; }

    ~SymbolInfo()
    {
        delete[] param_types;
        delete[] field_types;
        delete[] field_names;
        delete[] param_is_array;
    }
};

class ScopeTable
{
private:
    SymbolInfo **buckets;
    int num_buckets=30;
    string id;  // Changed from int to string to hold IDs like "1.1.1"
    int child_count; //  Tracks number of child scopes created from this table
    ScopeTable *parent;

public:
    //  id parameter changed to string
    ScopeTable(int n, string scope_id) : num_buckets(n), id(scope_id), child_count(0), parent(nullptr)
    {
        buckets = new SymbolInfo *[num_buckets];
        for (int i = 0; i < num_buckets; i++)
        {
            buckets[i] = nullptr;
        }
    }

    ~ScopeTable()
    {
        for (int i = 0; i < num_buckets; i++)
        {
            SymbolInfo *c = buckets[i];
            while (c)
            {
                SymbolInfo *temp = c;
                c = c->getNext();
                delete temp;
            }
        }
        delete[] buckets;
    }

    int hashOf(const std::string& name) const
    {
        unsigned long sum = 0;
        for (char c : name) sum += (unsigned char)c;
        return (int)(sum % num_buckets);
    }

    bool Insert(SymbolInfo *symbol, ostream &out)
    {
        unsigned int idx = hashOf(symbol->getName());
        SymbolInfo *c = buckets[idx];
        while (c)
        {
            if (c->getName() == symbol->getName())
            {
                return false;
            }
            c = c->getNext();
        }
        if (buckets[idx] == nullptr)
        {
            buckets[idx] = symbol;
        }
        else
        {
            SymbolInfo *tail = buckets[idx];
            while (tail->getNext())
            {
                tail = tail->getNext();
            }
            tail->setNext(symbol);
        }
        symbol->setNext(nullptr);
        return true;
    }

    SymbolInfo *Lookup(string name)
    {
        unsigned int idx = hashOf(name);
        SymbolInfo *c = buckets[idx];
        while (c)
        {
            if (c->getName() == name)
                return c;
            c = c->getNext();
        }
        return nullptr;
    }


    bool Delete(string name)
{
    unsigned int idx = hashOf(name);
    SymbolInfo *c = buckets[idx];
    SymbolInfo *prev = nullptr;

    while (c)
    {
        if (c->getName() == name)
        {
            if (prev)
            {
                prev->setNext(c->getNext());
            }
            else
            {
                buckets[idx] = c->getNext();
            }
            c->setNext(nullptr);
            delete c;
            return true;
        }
        prev = c;
        c = c->getNext();
    }
    return false;
}

    // Modified Print() to match Lexer log format
    void Print(ostream &out) const
    {
        out << "ScopeTable # " << id << "\n";
        for (int i = 0; i < num_buckets; i++)
        {
            SymbolInfo *c = buckets[i];
            if (c != nullptr)
            {
                out <<" "<< i << " --> ";
                while (c)
                {
                    out << c->getDisplayString();
                    c = c->getNext();
                }
                out << "\n";
            }
        }
        //out << "\n";
    }

    unsigned int getBucketIndex(string name) { return hashOf(name); }

    int getChainPos(string name)
    {
        unsigned int idx = hashOf(name);
        int pos = 0;
        SymbolInfo *c = buckets[idx];
        while (c)
        {
            if (c->getName() == name) return pos;
            pos++;
            c = c->getNext();
        }
        return -1;
    }

    void setParent(ScopeTable *p) { parent = p; }
    ScopeTable *getParent() { return parent; }
    void setId(string i) { id = i; }
    string getId() { return id; }
    int getChildCount() const { return child_count; }
    void incrementChildCount() { child_count++; }
};

class SymbolTable
{
private:
    ScopeTable *currentScope;
    int num_buckets;
    ostream &out;

public:
    SymbolTable(int n, ostream &o) : num_buckets(n), out(o)
    {
        currentScope = new ScopeTable(num_buckets, "1");
    }

    ~SymbolTable()
    {
        while (currentScope != nullptr)
        {
            ScopeTable *temp = currentScope;
            currentScope = currentScope->getParent();
            delete temp;
        }
    }

    void EnterScope()
    {
        if (currentScope == nullptr)
        {
            currentScope = new ScopeTable(num_buckets, "1");
            return;
        }

        currentScope->incrementChildCount();
        string newId = currentScope->getId() + "." + to_string(currentScope->getChildCount());

        ScopeTable *newScope = new ScopeTable(num_buckets, newId);
        newScope->setParent(currentScope);
        currentScope = newScope;
    }

    bool ExitScope()
    {
        if (currentScope == nullptr || currentScope->getParent() == nullptr)
            return false;

        ScopeTable *temp = currentScope;
        currentScope = currentScope->getParent();
        delete temp;
        return true;
    }

    bool InsertSymbol(SymbolInfo *symbol)
    {
        if (currentScope == nullptr) return false;
        return currentScope->Insert(symbol, out);
    }

    bool Remove(string name)
    {
        if (currentScope == nullptr) return false;
        return currentScope->Delete(name);
    }

    SymbolInfo *Lookup(string name)
    {
        ScopeTable *scope = currentScope;
        while (scope != nullptr)
        {
            SymbolInfo *symbol = scope->Lookup(name);
            if (symbol != nullptr)
                return symbol;
            scope = scope->getParent();
        }
        return nullptr;
    }

    void PrintCurrentScope()
    {
        if (currentScope != nullptr)
            currentScope->Print(out);
    }

    void PrintAllScopes()
    {
        ScopeTable *scope = currentScope;
        while (scope != nullptr)
        {
            scope->Print(out);
            scope = scope->getParent();
        }
        out<<"\n";
    }

    bool isAtRoot() const
    {
        return currentScope != nullptr && currentScope->getParent() == nullptr;
    }

    ScopeTable *getCurrentScope() { return currentScope; }
};

#endif
