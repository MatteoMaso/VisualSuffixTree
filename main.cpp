#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "usage: "<<argv[0]<< " file" << std::endl;
        return 1;
    }

    cst_t cst;                      //declare the suffix tree
    construct(cst, argv[1], 1);     //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end   = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    for (iterator it = begin; it != end; ++it) {
//        std::cout << " ID: " << cst.id(*it) << " DEPTH: " << cst.depth(*it) << " NODEDEPTH: " << cst.node_depth(*it) << "-[" << cst.lb(*it) << "," << cst.rb(*it) << "]" << std::endl;

        str_length = cst.depth(*it);

        string edge = "";

        if( (cst.node_depth(*it) == 0)||(cst.node_depth(*it) == 1 && str_length == 1 && (cst.lb(*it) == cst.rb(*it))) ){
            edge = "-";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {

                edge = "leaft:         ";

                for (int i = 1; i < str_length - 1; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "#";
                edge += "#";

            } else {

                edge = "internal_node: ";

                for (int i = 1; i <= str_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }




        }

        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << "," << cst.rb(*it) << "]" << edge << std::endl;
    }

}