//
// Created by root on 5/10/18.
//

#ifndef VISUALSUFFIXTREE_SVGUTILS_H
#define VISUALSUFFIXTREE_SVGUTILS_H

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class SvgUtils{

public:

static string getWindowsConfigurations(int width, int heigth) {

        string temp = "<rect x=\"0.0\" y=\"0\" width=\"" + to_string(width) + "\" height=\"" + to_string(heigth) +
                      "\" fill=\"url(#background)\"/>";
        temp += "<text text-anchor=\"middle\" x=\"500\" y=\"24\" font-size=\"17\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\">Flame Graph</text>\n"
                "<text text-anchor=\"\" x=\"10.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"unzoom\" onclick=\"unzoom()\" style=\"opacity: 0; cursor: pointer;\">Reset Zoom</text>\n"
                "<text text-anchor=\"\" x=\"890.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"search\" onmouseover=\"searchover()\" onmouseout=\"searchout()\" onclick=\"search_prompt()\" style=\"opacity: 0.1; cursor: pointer;\">Search</text>\n"
                "<text text-anchor=\"\" x=\"1090.00\" y=\"1249\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"matched\"> </text>\n"
                "\n"
                "<text text-anchor=\"\" x=\"10\" y=\"609\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"details\"> </text>";

        return temp;


    }

static string createSvgHeader(int width, int heigth) {

        string temp = "<svg version=\"1.1\" width=\"" + to_string(width) + "\" height=\"" + to_string(heigth) +
                      "\" onload=\"init(evt)\" viewBox=\"0 0 ";
        temp += to_string(width) + " " + to_string(heigth) +
                "\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">";
        temp += getHeader("headerSvg.txt"); //Insert the header SVG into the file

        temp += getWindowsConfigurations(width, heigth);

        return temp;
    }

static void printSvgNodeBlock(std::ofstream *svg_out, string text, double width, double x, double y, double height) {

        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
        temp += text;
        temp += "</title><rect x=\"";
        temp += to_string(x);
        temp += "\" y=\"";
        temp += to_string(y);
        temp += "\" width=\"";
        temp += to_string(width);
        temp += "\" ""height=\"" + to_string(height) + "\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n""</g>";

        char str[temp.length()];
        strcpy(str, temp.c_str());

        *svg_out << str;
    }

static    string getHeader(string fileName) {

        string txt;
        string temp;
        ifstream file;
        file.open(fileName);

        if (file.is_open()) {
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

};


#endif //VISUALSUFFIXTREE_SVGUTILS_H
