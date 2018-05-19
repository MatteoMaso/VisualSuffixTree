//
// Created by root on 5/10/18.
//

#ifndef VISUALSUFFIXTREE_SVGUTILS_H
#define VISUALSUFFIXTREE_SVGUTILS_H

#include <fstream>
#include <string>
#include <iostream>
#include "SvgCreator.h"

using namespace std;

typedef struct RgbColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor {
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

class SvgUtils {

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

    static void printSvgNodeBlock(std::ofstream *svg_out, string text, double width, double x, double y, double height, RgbColor c, double opacity = 1) {

        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
        temp += text;
        temp += "</title><rect x=\"";
        temp += to_string(x);
        temp += "\" y=\"";
        temp += to_string(y);
        temp += "\" width=\"";
        temp += to_string(width);
//        temp += "\" ""height=\"" + to_string(height) + "\" fill=\"rgb("+to_string(c.r)+","+ to_string(c.g)+","+ to_string(c.b)+")\" rx=\"2\" ry=\"2\" />\n""</g>";
        temp += "\" ""height=\"" + to_string(height) + "\" fill=\"rgb(" + to_string(c.r) + "," + to_string(c.g) + "," +
                to_string(c.b) + ")\" opacity=\""+ to_string(opacity)+" \" rx=\"2\" ry=\"2\" />\n""</g>";

        char str[temp.length()];
        strcpy(str, temp.c_str());

        *svg_out << str;
    }

    static void printSvgNodeBlock2(std::ofstream *svg_out, string text, double width, double x, double y, double height, string color, double opacity = 1) {

        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
        temp += text;
        temp += "</title><rect x=\"";
        temp += to_string(x);
        temp += "\" y=\"";
        temp += to_string(y);
        temp += "\" width=\"";
        temp += to_string(width);
//        temp += "\" ""height=\"" + to_string(height) + "\" fill=\"rgb("+to_string(c.r)+","+ to_string(c.g)+","+ to_string(c.b)+")\" rx=\"2\" ry=\"2\" />\n""</g>";
        temp += "\" ""height=\"" + to_string(height) + "\" fill=\""+ color +"\" opacity=\""+ to_string(opacity)+" \" rx=\"2\" ry=\"2\" />\n""</g>";

        char str[temp.length()];
        strcpy(str, temp.c_str());

        *svg_out << str;
    }

    static string getHeader(string fileName) {

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


    static RgbColor getColor(string color, int red, int green, int blue) {

        RgbColor tmp;
        tmp.r = 255;
        tmp.g = 50;
        tmp.b = 50;

        if (color.compare("RGB") == 0) {
            //use rgb color
            tmp.r = red;
            tmp.g = green;
            tmp.b = blue;
        } else {
            //check standard color
            if (color.compare("RED") == 0) {
                tmp.r = 255;
                tmp.g = 50;
                tmp.b = 50;
            } else if (color.compare("GREEN") == 0) {
                tmp.r = 0;
                tmp.g = 255;
                tmp.b = 0;
            } else if (color.compare("BLUE") == 0) {
                tmp.r = 0;
                tmp.g = 0;
                tmp.b = 255;
            } else if (color.compare("BLACK") == 0) {
                tmp.r = 0;
                tmp.g = 0;
                tmp.b = 0;
            } else if (color.compare("YELLOW") == 0) {
                tmp.r = 255;
                tmp.g = 255;
                tmp.b = 0;
            } else {
                std::cout << "we are using the default color!!" << std::endl;
            }

        }

        return tmp;

    }


    static RgbColor HsvToRgb(HsvColor hsv) {
        RgbColor rgb;
        unsigned char region, remainder, p, q, t;

        if (hsv.s == 0) {
            rgb.r = hsv.v;
            rgb.g = hsv.v;
            rgb.b = hsv.v;
            return rgb;
        }

        region = hsv.h / 43;
        remainder = (hsv.h - (region * 43)) * 6;

        p = (hsv.v * (255 - hsv.s)) >> 8;
        q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
        t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

        switch (region) {
            case 0:
                rgb.r = hsv.v;
                rgb.g = t;
                rgb.b = p;
                break;
            case 1:
                rgb.r = q;
                rgb.g = hsv.v;
                rgb.b = p;
                break;
            case 2:
                rgb.r = p;
                rgb.g = hsv.v;
                rgb.b = t;
                break;
            case 3:
                rgb.r = p;
                rgb.g = q;
                rgb.b = hsv.v;
                break;
            case 4:
                rgb.r = t;
                rgb.g = p;
                rgb.b = hsv.v;
                break;
            default:
                rgb.r = hsv.v;
                rgb.g = p;
                rgb.b = q;
                break;
        }

        return rgb;
    }

    static HsvColor RgbToHsv(RgbColor rgb) {
        HsvColor hsv;
        unsigned char rgbMin, rgbMax;

        rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
        rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

        hsv.v = rgbMax;
        if (hsv.v == 0) {
            hsv.h = 0;
            hsv.s = 0;
            return hsv;
        }

        hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
        if (hsv.s == 0) {
            hsv.h = 0;
            return hsv;
        }

        if (rgbMax == rgb.r)
            hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
        else if (rgbMax == rgb.g)
            hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
        else
            hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

        return hsv;
    }

};


#endif //VISUALSUFFIXTREE_SVGUTILS_H
