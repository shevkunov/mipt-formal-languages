#pragma once
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <queue>
namespace mfl {
    class Automat{
    public:
        Automat() : edges_(0) {
        }
        Automat(std::string s) : edges_(0) {
            fromString(s);
        }
        void addEdge(int from, char ch, int to) {
            Alfabet_.insert(ch);
            ++edges_;
            e_[from][ch].push_back(to);
            e_[to];
        }
        void makeFinalState(int vertex) {
            F_.insert(vertex);
        }
        std::string toString() {
            std::ostringstream ss;
            ss << edges_ << " " << F_.size() << "\n";
            for(auto ii = e_.begin(); ii != e_.end(); ++ii) {
                for(auto cIt = ii->second.begin(); cIt != ii->second.end(); ++cIt) {
                    for (auto vIt = cIt->second.begin(); vIt < cIt->second.end(); ++vIt) {
                        ss << ii->first << " " << cIt->first << " " << *vIt << "\n";
                    }
                }
            }
            for (auto fIt = F_.begin(); fIt != F_.end(); ++fIt) {
                ss << *fIt << " ";
            }
            ss << "\n";
            return ss.str();
        }
        void fromString(std::string s) {
            clear();
            std::istringstream ss(s);
            size_t fSize, eSize;
            ss >> eSize >> fSize;
            for (size_t i = 0; i < eSize; ++i) {
                int from, to;
                char ch;
                ss >> from >> ch >> to;
                addEdge(from, ch, to);
            }
            for (size_t i = 0; i < fSize; ++i) {
                int v;
                ss >> v;
                F_.insert(v);
            }
        }
        void clear() {
            F_.clear();
            e_.clear();
            Alfabet_.clear();
            edges_ = 0;
        }
        Automat determinize() {
            Automat ra;
            int id = 0;
            std::map<std::set<int>, int> vId; // new vertex's id
            std::queue<std::set<int>> workin;
            workin.push({0});
            vId[{0}] = id++;
            while (!workin.empty()) {
                std::set<int> v = workin.front();
                workin.pop();
                for (auto chi = Alfabet_.begin(); chi != Alfabet_.end(); ++chi) {
                    std::set<int> go;
                    for (auto vi = v.begin(); vi != v.end(); ++vi) {
                        go.insert(e_[*vi][*chi].begin(), e_[*vi][*chi].end());
                    }
                    if (go.size()) {
                        if (vId.find(go) == vId.end()) {
                            vId[go] = id++;
                            workin.push(go);
                        }
                        ra.addEdge(vId[v], *chi, vId[go]);
                    }
                }
            }

            for (auto vIdIt = vId.begin(); vIdIt != vId.end(); ++vIdIt) {
                for (auto FIt = F_.begin(); FIt != F_.end(); ++FIt) {
                    if (vIdIt->first.find(*FIt) != vIdIt->first.end()) {
                        ra.makeFinalState(vIdIt->second);
                        break;
                    }
                }
            }
            return ra;
        }

        Automat completion() {
            Automat ra(*this);
            bool added = false;
            int newVertex = 0;
            for (auto vIt = ra.e_.begin(); vIt != ra.e_.end(); ++vIt) {
                for (auto chIt = ra.Alfabet_.begin(); chIt != ra.Alfabet_.end(); ++chIt) {
                    if (vIt->second.find(*chIt) == vIt->second.end()) {
                        if (!added) {
                            newVertex = ra.e_.rbegin()->first + 1;
                            added = true;
                        }

                        ra.addEdge(vIt->first, *chIt, newVertex);
                    }
                }
            }
            return ra;
        }

        Automat minimize() {
            Automat a = determinize().completion(), ra;
            std::vector<std::vector<int>> g(a.e_.size(), std::vector<int>(a.Alfabet_.size()));
            std::vector<std::vector<int>> oldClass(a.e_.size(), std::vector<int>(1 + a.Alfabet_.size(), 0));
            std::vector<int> final(a.e_.size(), 0);
            std::vector<int> newClass(a.e_.size(), 0);

            int i = 0;
            for (auto vIt = a.e_.begin(); vIt != a.e_.end(); ++vIt, ++i) {
                int j = 0;
                for (auto chIt = a.Alfabet_.begin(); chIt != a.Alfabet_.end(); ++chIt, ++j) {
                    if (vIt->second[*chIt].size() != 1) {
                        throw std::runtime_error("Automat::minimize() : not minimal!");
                    }
                    g[i][j] = *(vIt->second[*chIt].begin());
                }
                final[i] = (a.F_.find(vIt->first) != a.F_.end());
            }

            for (auto fIt = a.F_.begin(); fIt != a.F_.end(); ++fIt) {
                oldClass[*fIt][0] = 1;
            }

            bool equals = true;
            do {
                for (size_t v = 0; v < oldClass.size(); ++v) {
                    for (size_t ch = 0; ch < g[v].size(); ++ch) {
                        oldClass[v][ch + 1] = oldClass[g[v][ch]][0];
                    }
                }

                int newClasses = 0;
                std::map<std::vector<int>, int> classes;
                for (size_t i = 0; i < oldClass.size(); ++i) {
                    if (classes.find(oldClass[i]) == classes.end()) {
                        classes[oldClass[i]] = newClasses;
                        newClass[i] = newClasses++;
                    } else {
                        newClass[i] = classes[oldClass[i]];
                    }
                }

                equals = true;
                for (size_t i = 0; i < oldClass.size(); ++i) {
                    equals &= (oldClass[i][0] == newClass[i]);
                    oldClass[i][0] = newClass[i];
                }
            } while (!equals);


            std::set <std::vector<int>> added;
            for (size_t v = 0; v < oldClass.size(); ++v) {
                if (added.find(oldClass[v]) == added.end()) {
                    added.insert(oldClass[v]);
                    auto chIt = a.Alfabet_.begin();
                    for (size_t ch = 1; ch < oldClass[v].size(); ++ch, ++chIt) {
                        ra.addEdge(oldClass[v][0], *chIt, oldClass[v][ch]);
                        if (final[v]) {
                            ra.makeFinalState(oldClass[v][0]);
                        }
                    }
                }
            }

            return ra;
        }

    private:
        std::set<char> Alfabet_;
        std::set<int> F_;
        std::map<int, std::map<char, std::vector<int>>> e_;
        size_t edges_;
    };
}
