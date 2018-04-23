//
// Created by root on 4/16/18.
//

#ifndef VISUALSUFFIXTREE_SVGCREATOR_H
#define VISUALSUFFIXTREE_SVGCREATOR_H

using namespace std;

class SvgCreator {

public:
    SvgCreator(char *inputFileName);

    string partitioner(string s, int from, int to){

        string a = "";

        for (int i = from; i <= to; ++i) {
            a += s[i];
        }

        return a;
    }

    string getHeader(string fileName) {
        {

            string txt;
            string temp;
            ifstream file;
            file.open(fileName);

            if (file.is_open()) {
//        std::cout << "File open" << std::endl;

                while (!file.eof()) {
                    getline(file, temp);
                    txt += temp + "\n";
                }

            } else {
                std::cout << "I'm not able to read the header!" << std::endl;
            }

            file.close();
            return txt;
        }
    }

};


#endif //VISUALSUFFIXTREE_SVGCREATOR_H
