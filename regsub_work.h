#pragma once
#include <vector>
#include <string>
#include <exception>
#include <algorithm>

namespace mfl{
    class DPVSubWordSubReg {
    public:
        DPVSubWordSubReg(const std::string& word, char ch) : DPVSubWordSubReg(word.length()) {
            if (word.length() > 0) {
                if (ch != '1') {
                    for (size_t i = 0; i < word.length(); ++i) {
                        if (word[i] == ch) {
                            l[i][i] = true;
                            r[i][i] = true;
                            c[i][i] = true;
                            mx = max(mx, {1, i});
                        }
                    }
                } else {
                    mayBeZero = true;
                }
            } else {
                throw std::runtime_error("why?");
            }
        }

        DPVSubWordSubReg(const DPVSubWordSubReg& L, const DPVSubWordSubReg& R, char operation) : DPVSubWordSubReg(L.c.size())  {
            switch (operation) {
            case '+':
                mx = max(L.mx, R.mx);
                mayBeZero = L.mayBeZero || R.mayBeZero;
                for (size_t i = 0; i < c.size(); ++i) {
                    for (size_t j = i; j < c[i].size(); ++j) {
                        c[i][j] = L.c[i][j] || R.c[i][j];
                        l[i][j] = L.l[i][j] || R.l[i][j] || c[i][j];
                        r[i][j] = L.r[i][j] || R.r[i][j] || c[i][j];

                        if (l[i][j] || r[i][j] || c[i][j]) {
                            mx = max(mx, {j - i + 1, i});
                        }

                    }
                }
                break;
            case '.':
                mx = max(L.mx, R.mx);
                mayBeZero = L.mayBeZero && R.mayBeZero;
                for (size_t i = 0; i < c.size(); ++i) {
                    for (size_t j = i; j < c.size(); ++j) {
                        for (size_t k = i; k + 1 <= j; ++k) {
                            c[i][j] = c[i][j] || (L.c[i][k] && R.c[k + 1][j]);
                            l[i][j] = l[i][j] || (L.c[i][k] && R.l[k + 1][j]);
                            r[i][j] = r[i][j] || (L.r[i][k] && R.c[k + 1][j]);

                            if (L.r[i][k] && R.l[k + 1][j]) {
                                mx = max(mx, {j - i + 1, i});
                            }
                        }

                        if (L.mayBeZero) {
                            l[i][j] = l[i][j] || R.l[i][j];
                            c[i][j] = c[i][j] || R.c[i][j];
                        }

                        if (R.mayBeZero) {
                            r[i][j] = r[i][j] || L.r[i][j];
                            c[i][i] = c[i][i] || L.c[i][j];
                        }

                        l[i][j] = l[i][j] || L.l[i][j] || c[i][j];
                        r[i][j] = r[i][j] || R.r[i][j] || c[i][j];

                        if (l[i][j] || r[i][j] || c[i][j]) {
                            mx = max(mx, {j - i + 1, i});
                        }

                    }
                }
                break;
            }
        }

        DPVSubWordSubReg(const DPVSubWordSubReg& V, char operation) : DPVSubWordSubReg(V.c.size()) {
            switch (operation) {
            case '*':
                mx = V.mx;
                mayBeZero = true;
                for (int i = 0; i < c.size(); ++i) {
                    for (int j = 0; j < c.size(); ++j) {
                        c[i][j] = V.c[i][j];
                        l[i][j] = V.l[i][j];
                        r[i][j] = V.r[i][j];
                    }
                }

                for (int i = 0; i < c.size(); ++i) {
                    for (int j = i; j < c.size(); ++j) {
                        for (int k = i; k + 1 <= j; ++k) {
                            if (c[i][k] && c[k + 1][j]) {
                                c[i][j] = true;
                                l[i][j] = true;
                                r[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }
                        }
                    }
                }

                for (int i = 0; i < c.size(); ++i) {
                    for (int j = i; j < c.size(); ++j) {
                        for (int k = i; k + 1 <= j; ++k) {
                            if (l[i][k] && c[k + 1][j]) {
                                l[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }

                            if (c[i][k] && r[k + 1][j]) {
                                r[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }
                        }
                    }
                }

                for (int i = 0; i < c.size(); ++i) {
                    for (int j = i; j < c.size(); ++j) {
                        for (int k = i; k + 1 <= j; ++k) {
                            if (r[i][k] && l[k + 1][j]) {
                                mx = max(mx, {j - i + 1, i});
                            }
                            if (c[i][k] && l[k + 1][j]) {
                                l[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }
                            if (r[i][k] && c[k + 1][j]) {
                                r[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }
                        }
                    }
                }

            }
        }

        std::pair<int, int> getMaximum() const {
            return mx;
        }

    private:
        std::vector<std::vector<char>> l;
        std::vector<std::vector<char>> r;
        std::vector<std::vector<char>> c; // we use [i, j] indexation
        std::pair<int, int> mx; // {length, first index}
        bool mayBeZero;

        DPVSubWordSubReg(size_t length) {
            l.resize(length, std::vector<char>(length, false));
            r.resize(length, std::vector<char>(length, false));
            c.resize(length, std::vector<char>(length, false));
            mx = {0, 0};
            mayBeZero = false;
        }

    };

    class DPVSubWord {
    public:
        DPVSubWord(const std::string& word, char ch) : DPVSubWord(word.length()) {
            if (word.length() > 0) {
                if (ch != '1') {
                    for (size_t i = 0; i < word.length(); ++i) {
                        if (word[i] == ch) {
                            c[i][i] = true;
                            mx = max(mx, {1, i});
                        }
                    }
                } else {
                    mayBeZero = true;
                }
            } else {
                throw std::runtime_error("why?");
            }
        }

        DPVSubWord(const DPVSubWord& L, const DPVSubWord& R, char operation) : DPVSubWord(L.c.size())  {
            switch (operation) {
            case '+':
                mx = max(L.mx, R.mx);
                mayBeZero = L.mayBeZero || R.mayBeZero;
                for (size_t i = 0; i < c.size(); ++i) {
                    for (size_t j = i; j < c[i].size(); ++j) {
                        c[i][j] = L.c[i][j] || R.c[i][j];

                        if (c[i][j]) {
                            mx = max(mx, {j - i + 1, i});
                        }

                    }
                }
                break;
            case '.':
                mayBeZero = L.mayBeZero && R.mayBeZero;
                for (size_t i = 0; i < c.size(); ++i) {
                    for (size_t j = i; j < c.size(); ++j) {
                        for (size_t k = i; k + 1 <= j; ++k) {
                            c[i][j] = c[i][j] || (L.c[i][k] && R.c[k + 1][j]);
                        }

                        if (L.mayBeZero) {
                            c[i][j] = c[i][j] || R.c[i][j];
                        }

                        if (R.mayBeZero) {
                            c[i][i] = c[i][i] || L.c[i][j];
                        }

                        if (c[i][j]) {
                            mx = max(mx, {j - i + 1, i});
                        }

                    }
                }
                break;
            }
        }

        DPVSubWord(const DPVSubWord& V, char operation) : DPVSubWord(V.c.size()) {
            switch (operation) {
            case '*':
                mx = V.mx;
                mayBeZero = true;
                for (int i = 0; i < c.size(); ++i) {
                    for (int j = 0; j < c.size(); ++j) {
                        c[i][j] = V.c[i][j];
                    }
                }

                for (int i = 0; i < c.size(); ++i) {
                    for (int j = i; j < c.size(); ++j) {
                        for (int k = i; k + 1 <= j; ++k) {
                            if (c[i][k] && c[k + 1][j]) {
                                c[i][j] = true;
                                mx = max(mx, {j - i + 1, i});
                            }
                        }
                    }
                }

            }
        }

        std::pair<int, int> getMaximum() const {
            return mx;
        }

    private:
        std::vector<std::vector<char>> c; // we use [i, j] indexation
        std::pair<int, int> mx; // {length, first index}
        bool mayBeZero;

        DPVSubWord(size_t length) {
            c.resize(length, std::vector<char>(length, false));
            mx = {0, 0};
            mayBeZero = false;
        }

    };

    template <class T>
    bool find(std::vector<T>& v, T value) {
        size_t i = 0;
        while ((i < v.size()) && (v[i] != value)) {
            ++i;
        }
        return i < v.size();
    }

    template<class DPV>
    std::pair<int, int> regSubDP(const std::string& reg, const std::string& word)  {
        std::vector<DPV> st; //std::vector faster than std::stack
        std::vector<char> gs = {'a', 'b', 'c'};
        std::vector<char> go = {'1', '.', '+', '*'};
        for (size_t i = 0; i < word.size(); ++i) {
            size_t j = 0;
            if (!find(gs, word[i])) {
                throw std::runtime_error("illegal string.");
            }
        }
        for (size_t i = 0; i < reg.size(); ++i) {
            if (!find(gs, reg[i]) && !find(go, reg[i])) {
                throw std::runtime_error("illegal reg.");
            }
        }
        for (size_t i = 0; i < reg.size(); ++i) {
            if (std::find(gs.begin(), gs.end(), reg[i]) != gs.end()) {
                st.push_back(DPV(word, reg[i]));
            } else {
                switch (reg[i]) {
                case '1':
                    st.push_back(DPV(word, reg[i]));
                    break;
                case '.':
                case '+':
                    if (st.size() < 2) {
                        throw std::runtime_error("incorrect reg.");
                    }
                    {
                        DPV b = st.back();
                        st.pop_back();
                        DPV a = st.back();
                        st.pop_back();
                        st.push_back(DPV(a, b, reg[i]));
                    }
                    break;
                case '*':
                    if (st.size() < 1) {
                        throw std::runtime_error("incorrect reg.");
                    }
                    {
                        DPV a = st.back();
                        st.pop_back();
                        st.push_back(DPV(a, '*'));
                    }
                    break;
                }
            }
        }

        if (st.size() != 1) {
            throw std::runtime_error("incorrect reg.");
        } else {
            return st.back().getMaximum();
        }
    }
}
