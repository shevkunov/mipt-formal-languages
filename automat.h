#pragma once
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <queue>

namespace mfl {
    class Automat{
    public:
        Automat() : edgesCount_(0) {
        }

        Automat(std::string s) : edgesCount_(0) {
            fromString(s);
        }

        void addEdge(int from, char chr, int to) {
            alfabet_.insert(chr);
            ++edgesCount_;
            edges_[from][chr].push_back(to);
            edges_[to];
        }

        void makeFinalState(int vertex) {
            final_.insert(vertex);
        }

        std::string toString() {
            std::ostringstream ss;
            ss << edgesCount_ << " " << final_.size() << "\n";
            for(auto verIt = edges_.begin(); verIt != edges_.end(); ++verIt) {
                for(auto charIt = verIt->second.begin(); charIt != verIt->second.end(); ++charIt) {
                    for (auto toIt = charIt->second.begin(); toIt < charIt->second.end(); ++toIt) {
                        ss << verIt->first << " " << charIt->first << " " << *toIt << "\n";
                    }
                }
            }
            for (auto fIt = final_.begin(); fIt != final_.end(); ++fIt) {
                ss << *fIt << " ";
            }
            ss << "\n";
            return ss.str();
        }

        void fromString(std::string s) {
            clear();
            std::istringstream ss(s);
            size_t edgesCount, finalCount;
            ss >> edgesCount >> finalCount;
            for (size_t i = 0; i < edgesCount; ++i) {
                int from, to;
                char ch;
                ss >> from >> ch >> to;
                addEdge(from, ch, to);
            }
            for (size_t i = 0; i < finalCount; ++i) {
                int v;
                ss >> v;
                final_.insert(v);
            }
        }

        void clear() {
            final_.clear();
            edges_.clear();
            alfabet_.clear();
            edgesCount_ = 0;
        }

        Automat determinize() {
            Automat retAuto;
            int id = 0;
            std::map<std::set<int>, int> vId; // new vertex's id
            std::queue<std::set<int>> workIn;
            workIn.push({0});
            vId[{0}] = id++;
            while (!workIn.empty()) {
                std::set<int> currentVertex = workIn.front();
                workIn.pop();
                for (auto charIt = alfabet_.begin(); charIt != alfabet_.end(); ++charIt) {
                    std::set<int> go;
                    for (auto verIt = currentVertex.begin(); verIt != currentVertex.end(); ++verIt) {
                        go.insert(edges_[*verIt][*charIt].begin(), edges_[*verIt][*charIt].end());
                    }
                    if (go.size()) {
                        if (vId.find(go) == vId.end()) {
                            vId[go] = id++;
                            workIn.push(go);
                        }
                        retAuto.addEdge(vId[currentVertex], *charIt, vId[go]);
                    }
                }
            }

            for (auto vIdIt = vId.begin(); vIdIt != vId.end(); ++vIdIt) {
                for (auto finIt = final_.begin(); finIt != final_.end(); ++finIt) {
                    if (vIdIt->first.find(*finIt) != vIdIt->first.end()) {
                        retAuto.makeFinalState(vIdIt->second);
                        break;
                    }
                }
            }
            return retAuto;
        }

        Automat completion() {
            Automat retAuto(*this);
            bool added = false;
            int newVertex = 0;
            for (auto verIt = retAuto.edges_.begin(); verIt != retAuto.edges_.end(); ++verIt) {
                for (auto charIt = retAuto.alfabet_.begin(); charIt != retAuto.alfabet_.end(); ++charIt) {
                    if (verIt->second.find(*charIt) == verIt->second.end()) {
                        if (!added) {
                            newVertex = retAuto.edges_.rbegin()->first + 1;
                            added = true;
                        }

                        retAuto.addEdge(verIt->first, *charIt, newVertex);
                    }
                }
            }
            return retAuto;
        }

        Automat minimize() {
            Automat a = determinize().completion(), retAuto;
            std::vector<std::vector<int>> graph(a.edges_.size(), std::vector<int>(a.alfabet_.size()));
            std::vector<std::vector<int>> oldClass(a.edges_.size(), std::vector<int>(1 + a.alfabet_.size(), 0));
            std::vector<int> final(a.edges_.size(), 0);
            std::vector<int> newClass(a.edges_.size(), 0);

            int i = 0;
            for (auto verIt = a.edges_.begin(); verIt != a.edges_.end(); ++verIt, ++i) {
                int j = 0;
                for (auto charIt = a.alfabet_.begin(); charIt != a.alfabet_.end(); ++charIt, ++j) {
                    if (verIt->second[*charIt].size() != 1) {
                        throw std::runtime_error("Automat::minimize() : not minimal!");
                    }
                    graph[i][j] = *(verIt->second[*charIt].begin());
                }
                final[i] = (a.final_.find(verIt->first) != a.final_.end());
            }

            for (auto fIt = a.final_.begin(); fIt != a.final_.end(); ++fIt) {
                oldClass[*fIt][0] = 1;
            }

            bool equals = true;
            do {
                for (size_t v = 0; v < oldClass.size(); ++v) {
                    for (size_t ch = 0; ch < graph[v].size(); ++ch) {
                        oldClass[v][ch + 1] = oldClass[graph[v][ch]][0];
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
                    auto charIt = a.alfabet_.begin();
                    for (size_t ch = 1; ch < oldClass[v].size(); ++ch, ++charIt) {
                        retAuto.addEdge(oldClass[v][0], *charIt, oldClass[v][ch]);
                        if (final[v]) {
                            retAuto.makeFinalState(oldClass[v][0]);
                        }
                    }
                }
            }

            return retAuto;
        }

    private:
        std::set<char> alfabet_;
        std::set<int> final_;
        std::map<int, std::map<char, std::vector<int>>> edges_;
        size_t edgesCount_;
    };
}
