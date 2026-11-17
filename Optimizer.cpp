#include "Optimizer.h"
#include <sstream>
#include <unordered_map>
#include <set>


static string trimmed(const string &s) {
    size_t a = s.find_first_not_of(" \t");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}

static bool isComment(const string &line) { string t = trimmed(line); return !t.empty() && t[0] == ';'; }
static bool isBlank(const string &line)   { return trimmed(line).empty(); }

static bool isLabelLine(const string &line, string &outName) {
    if (!line.empty() && (line[0] == ' ' || line[0] == '\t')) return false;
    string t = trimmed(line);
    if (t.empty() || t.back() != ':') return false;
    outName = t.substr(0, t.size() - 1);
    return true;
}

static bool parseTwoOperand(const string &line, string &mnem, string &op1, string &op2) {
    string t = trimmed(line);
    size_t sp = t.find(' ');
    if (sp == string::npos) return false;
    mnem = t.substr(0, sp);
    string rest = trimmed(t.substr(sp + 1));
    size_t comma = rest.find(',');
    if (comma == string::npos) return false;
    op1 = trimmed(rest.substr(0, comma));
    op2 = trimmed(rest.substr(comma + 1));
    return true;
}

static bool parseOneOperand(const string &line, string &mnem, string &op) {
    string t = trimmed(line);
    size_t sp = t.find(' ');
    if (sp == string::npos) { mnem = t; op = ""; return !t.empty(); }
    mnem = t.substr(0, sp);
    op = trimmed(t.substr(sp + 1));
    return true;
}

static size_t nextReal(const vector<string> &lines, size_t idx) {
    while (idx < lines.size() && (isComment(lines[idx]) || isBlank(lines[idx]))) idx++;
    return idx;
}

static bool redundantMovBack(const string &l1, const string &l2) {
    string m1, a1, b1, m2, a2, b2;
    if (!parseTwoOperand(l1, m1, a1, b1) || !parseTwoOperand(l2, m2, a2, b2)) return false;
    return m1 == "mov" && m2 == "mov" && a1 == b2 && b1 == a2;
}

static bool samePushPop(const string &l1, const string &l2) {
    string m1, o1, m2, o2;
    if (!parseOneOperand(l1, m1, o1) || !parseOneOperand(l2, m2, o2)) return false;
    return m1 == "push" && m2 == "pop" && o1 == o2;
}

static bool isIdentityArith(const string &line) {
    string m, a, b;
    if (!parseTwoOperand(line, m, a, b)) return false;
    if ((m == "add" || m == "sub") && b == "0") return true;
    if ((m == "imul" || m == "mul") && b == "1") return true;
    return false;
}

// Optimizer methods 

vector<string> Optimizer::splitLines(const string &text) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line)) lines.push_back(line);
    return lines;
}

vector<string> Optimizer::mergeConsecutiveLabels(const vector<string> &input) {
    vector<string> out;
    unordered_map<string, string> rename;

    for (size_t i = 0; i < input.size(); ) {
        string name;
        if (isLabelLine(input[i], name)) {
            string survivor = name;
            out.push_back(input[i]);
            size_t j = i + 1;
            while (true) {
                size_t k = nextReal(input, j);
                for (size_t c = j; c < k; c++) out.push_back(input[c]);
                string nextName;
                if (k < input.size() && isLabelLine(input[k], nextName)) {
                    rename[nextName] = survivor;
                    j = k + 1;
                    continue;
                }
                j = k;
                break;
            }
            i = j;
            continue;
        }
        out.push_back(input[i]);
        i++;
    }

    if (rename.empty()) return out;

    for (auto &kv : rename) {
        string t = kv.second;
        while (rename.count(t)) t = rename[t];
        kv.second = t;
    }

    static const set<string> jumpish = {"jmp","je","jne","jl","jle","jg","jge","call"};
    for (string &line : out) {
        string mnem, op;
        if (parseOneOperand(line, mnem, op) && jumpish.count(mnem) && rename.count(op))
            line = "    " + mnem + " " + rename[op];
    }
    return out;
}

vector<string> Optimizer::peepholeOptimize(const vector<string> &input) {
    vector<string> cur = input;
    bool changed = true;

    while (changed) {
        changed = false;
        vector<string> out;
        out.reserve(cur.size());

        for (size_t i = 0; i < cur.size(); ) {
            if (isBlank(cur[i])) { i++; continue; }

            if (!isComment(cur[i])) {
                size_t j = nextReal(cur, i + 1);
                if (j < cur.size()) {
                    if (redundantMovBack(cur[i], cur[j])) {
                        out.push_back(cur[i]);
                        for (size_t k = i + 1; k < j; k++) out.push_back(cur[k]);
                        i = j + 1; changed = true; continue;
                    }
                    if (samePushPop(cur[i], cur[j])) {
                        i = j + 1; changed = true; continue;
                    }
                }
            }

            if (isIdentityArith(cur[i])) { i++; changed = true; continue; }

            out.push_back(cur[i]);
            i++;
        }
        cur = out;
    }

    return mergeConsecutiveLabels(cur);
}

string Optimizer::optimize(const string &asmText) {
    vector<string> lines = peepholeOptimize(splitLines(asmText));
    string result;
    for (auto &l : lines) result += l + "\n";
    return result;
}