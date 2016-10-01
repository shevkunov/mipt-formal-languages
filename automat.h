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
        }
        void makeFinalState(int vertex) {
            F_.insert(vertex);
        }
        std::string toString() {
            std::ostringstream ss;
            ss << edges_ << " " << F_.size() << "\n";
            for(auto ii = e_.begin(); ii != e_.end(); ++ii) {
                for(auto ic = ii->second.begin(); ic != ii->second.end(); ++ic) {
                    for (auto iv = ic->second.begin(); iv < ic->second.end(); ++iv) {
                        ss << ii->first << " " << ic->first << " " << *iv << "\n";
                    }
                }
            }
            for (auto ii = F_.begin(); ii != F_.end(); ++ii) {
                ss << *ii << " ";
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

    private:
        std::set<char> Alfabet_;
        std::set<int> F_;
        std::map<int, std::map<char, std::vector<int>>> e_;
        size_t edges_;
    };
}
