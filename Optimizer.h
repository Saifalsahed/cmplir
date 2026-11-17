#pragma once
#include <string>
#include <vector>
using namespace std;

class Optimizer {
public:
    static string optimize(const string &asmText);

private:
    static vector<string> splitLines(const string &text);
    static vector<string> peepholeOptimize(const vector<string> &input);
    static vector<string> mergeConsecutiveLabels(const vector<string> &input);
};