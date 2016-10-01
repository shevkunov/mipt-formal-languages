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
        }
        Automat determinize() {
            Automat ra;
            int id = 0;
            std::map<std::set<int>, int> vId; // new vertex's id
            std::queue<std::set<int>> workin;
            workin.push({0});
            vId[{0}] = id++;
            while (!workin.empty()) {

            }
            return ra;
        }

    private:
        std::set<int> F_;
        std::map<int, std::map<char, std::vector<int>>> e_;
        size_t edges_;
    };
}
