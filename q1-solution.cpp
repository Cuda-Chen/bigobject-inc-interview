#include <vector>
#include <cstring>
#include <string>

std::vector<char *> vs = {
  "aa",
  "ccc",
  "eeeee",
  "gggggg",
  "zzzz",
};

// Q1
// 完成用 binary serach 搜尋vector vs, 回傳字串str在vs中的位置, 若未發現則回傳 -1
// 不限使用 C或C++的function 
// 但請注意c++ string 與 c char*轉換的消耗, 盡可能以最有效率的方式完成工作, 
int bin_search(const std::string &str) {
    int l = 0, r = vs.size();
    while(l < r) {
        int m = l + (r - l) / 2;
        int res = strcmp(vs[m], str.c_str());
            if(res == 0)
                return m;
            else if(res < 0)
                l++;
            else
                r--;
    }
    return -1;
}

// Q2
// 若要insert "111"到vs內, 應該放在哪最為合適.

// Ans. 
