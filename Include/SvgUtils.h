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

    static string createSvgHeader(int width, int heigth, int fromTop) {

        string temp = "<svg version=\"1.1\" width=\"" + to_string(width) + "\" height=\"" + to_string(heigth) +
                      "\" onload=\"init(evt)\" viewBox=\"0 0 ";
        temp += to_string(width) + " " + to_string(heigth) +
                "\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">";

//        temp += getHeader("headerSvg.txt"); //Insert the header SVG into the file

        temp += "<defs>\n"
                "\t<linearGradient id=\"background\" y1=\"0\" y2=\"1\" x1=\"0\" x2=\"0\">\n"
                "\t\t<stop stop-color=\"#ffffff\" offset=\"5%\"/>\n"
                "\t\t<stop stop-color=\"#ffffff\" offset=\"95%\"/>\n"
                "\t</linearGradient>\n"
                "</defs>\n"
                "<style type=\"text/css\">\n"
                "\t.func_g:hover { stroke:black; stroke-width:0.5; }\n"
                "</style>\n"
                "\n"
                "<script type=\"text/ecmascript\">\n"
                "<![CDATA[\n"
                "\tvar details, searchbtn, matchedtxt, svg;\n"
                "\tfunction init(evt) { \n"
                "\t\tdetails = document.getElementById(\"details\").firstChild; \n"
                "\t\tsearchbtn = document.getElementById(\"search\");\n"
                "\t\tmatchedtxt = document.getElementById(\"matched\");\n"
                "\t\tsvg = document.getElementsByTagName(\"svg\")[0];\n"
                "\t\tsearching = 0;\n"
                "\t}\n"
                "\n"
                "\t// mouse-over for info\n"
                "\tfunction s(node) {\t\t// show\n"
                "\t\tinfo = g_to_text(node);\n"
                "\t\tdetails.nodeValue = \"Function: \" + info;\n"
                "\t}\n"
                "\tfunction c() {\t\t\t// clear\n"
                "\t\tdetails.nodeValue = ' ';\n"
                "\t}\n"
                "\n"
                "\t// ctrl-F for search\n"
                "\twindow.addEventListener(\"keydown\",function (e) {\n"
                "\t\tif (e.keyCode === 114 || (e.ctrlKey && e.keyCode === 70)) {\n"
                "\t\t\te.preventDefault();\n"
                "\t\t\tsearch_prompt();\n"
                "\t\t}\n"
                "\t})\n"
                "\n"
                "\t// functions\n"
                "\tfunction find_child(parent, name, attr) {\n"
                "\t\tvar children = parent.childNodes;\n"
                "\t\tfor (var i=0; i<children.length;i++) {\n"
                "\t\t\tif (children[i].tagName == name)\n"
                "\t\t\t\treturn (attr != undefined) ? children[i].attributes[attr].value : children[i];\n"
                "\t\t}\n"
                "\t\treturn;\n"
                "\t}\n"
                "\tfunction orig_save(e, attr, val) {\n"
                "\t\tif (e.attributes[\"_orig_\"+attr] != undefined) return;\n"
                "\t\tif (e.attributes[attr] == undefined) return;\n"
                "\t\tif (val == undefined) val = e.attributes[attr].value;\n"
                "\t\te.setAttribute(\"_orig_\"+attr, val);\n"
                "\t}\n"
                "\tfunction orig_load(e, attr) {\n"
                "\t\tif (e.attributes[\"_orig_\"+attr] == undefined) return;\n"
                "\t\te.attributes[attr].value = e.attributes[\"_orig_\"+attr].value;\n"
                "\t\te.removeAttribute(\"_orig_\"+attr);\n"
                "\t}\n"
                "\tfunction g_to_text(e) {\n"
                "\t\tvar text = find_child(e, \"title\").firstChild.nodeValue;\n"
                "\t\treturn (text)\n"
                "\t}\n"
                "\tfunction g_to_func(e) {\n"
                "\t\tvar func = g_to_text(e);\n"
                "\t\tif (func != null)\n"
                "\t\t\tfunc = func.replace(/ .*/, \"\");\n"
                "\t\treturn (func);\n"
                "\t}\n"
                "\tfunction update_text(e) {\n"
                "\t\tvar r = find_child(e, \"rect\");\n"
                "\t\tvar t = find_child(e, \"text\");\n"
                "\t\tvar w = parseFloat(r.attributes[\"width\"].value) -3;\n"
                "\t\tvar txt = find_child(e, \"title\").textContent.replace(/\\([^(]*\\)/,\"\");\n"
                "\t\tt.attributes[\"x\"].value = parseFloat(r.attributes[\"x\"].value) +3;\n"
                "\t\t\n"
                "\t\t// Smaller than this size won't fit anything\n"
                "\t\tif (w < 2*12*0.59) {\n"
                "\t\t\tt.textContent = \"\";\n"
                "\t\t\treturn;\n"
                "\t\t}\n"
                "\t\t\n"
                "\t\tt.textContent = txt;\n"
                "\t\t// Fit in full text width\n"
                "\t\tif (/^ *$/.test(txt) || t.getSubStringLength(0, txt.length) < w)\n"
                "\t\t\treturn;\n"
                "\t\t\n"
                "\t\tfor (var x=txt.length-2; x>0; x--) {\n"
                "\t\t\tif (t.getSubStringLength(0, x+2) <= w) { \n"
                "\t\t\t\tt.textContent = txt.substring(0,x) + \"..\";\n"
                "\t\t\t\treturn;\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t\tt.textContent = \"\";\n"
                "\t}\n"
                "\n"
                "\t// zoom\n"
                "\tfunction zoom_reset(e) {\n"
                "\t\tif (e.attributes != undefined) {\n"
                "\t\t\torig_load(e, \"x\");\n"
                "\t\t\torig_load(e, \"width\");\n"
                "\t\t}\n"
                "\t\tif (e.childNodes == undefined) return;\n"
                "\t\tfor(var i=0, c=e.childNodes; i<c.length; i++) {\n"
                "\t\t\tzoom_reset(c[i]);\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction zoom_child(e, x, ratio) {\n"
                "\t\tif (e.attributes != undefined) {\n"
                "\t\t\tif (e.attributes[\"x\"] != undefined) {\n"
                "\t\t\t\torig_save(e, \"x\");\n"
                "\t\t\t\te.attributes[\"x\"].value = (parseFloat(e.attributes[\"x\"].value) - x - 10) * ratio + 10;\n"
                "\t\t\t\tif(e.tagName == \"text\") e.attributes[\"x\"].value = find_child(e.parentNode, \"rect\", \"x\") + 3;\n"
                "\t\t\t}\n"
                "\t\t\tif (e.attributes[\"width\"] != undefined) {\n"
                "\t\t\t\torig_save(e, \"width\");\n"
                "\t\t\t\te.attributes[\"width\"].value = parseFloat(e.attributes[\"width\"].value) * ratio;\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t\t\n"
                "\t\tif (e.childNodes == undefined) return;\n"
                "\t\tfor(var i=0, c=e.childNodes; i<c.length; i++) {\n"
                "\t\t\tzoom_child(c[i], x-10, ratio);\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction zoom_parent(e) {\n"
                "\t\tif (e.attributes) {\n"
                "\t\t\tif (e.attributes[\"x\"] != undefined) {\n"
                "\t\t\t\torig_save(e, \"x\");\n"
                "\t\t\t\te.attributes[\"x\"].value = 10;\n"
                "\t\t\t}\n"
                "\t\t\tif (e.attributes[\"width\"] != undefined) {\n"
                "\t\t\t\torig_save(e, \"width\");\n"
                "\t\t\t\te.attributes[\"width\"].value = parseInt(svg.width.baseVal.value) - (10*2);\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t\tif (e.childNodes == undefined) return;\n"
                "\t\tfor(var i=0, c=e.childNodes; i<c.length; i++) {\n"
                "\t\t\tzoom_parent(c[i]);\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction zoom(node) { \n"
                "\t\tvar attr = find_child(node, \"rect\").attributes;\n"
                "\t\tvar width = parseFloat(attr[\"width\"].value);\n"
                "\t\tvar xmin = parseFloat(attr[\"x\"].value);\n"
                "\t\tvar xmax = parseFloat(xmin + width);\n"
                "\t\tvar ymin = parseFloat(attr[\"y\"].value);\n"
                "\t\tvar ratio = (svg.width.baseVal.value - 2*10) / width;\n"
                "\t\t\n"
                "\t\t// XXX: Workaround for JavaScript float issues (fix me)\n"
                "\t\tvar fudge = 0.0001;\n"
                "\t\t\n"
                "\t\tvar unzoombtn = document.getElementById(\"unzoom\");\n"
                "\t\tunzoombtn.style[\"opacity\"] = \"1.0\";\n"
                "\t\t\n"
                "\t\tvar el = document.getElementsByTagName(\"g\");\n"
                "\t\tfor(var i=0;i<el.length;i++){\n"
                "\t\t\tvar e = el[i];\n"
                "\t\t\tvar a = find_child(e, \"rect\").attributes;\n"
                "\t\t\tvar ex = parseFloat(a[\"x\"].value);\n"
                "\t\t\tvar ew = parseFloat(a[\"width\"].value);\n"
                "\t\t\t// Is it an ancestor\n";
        temp += "\t\t\tif (0 == "+to_string(fromTop)+") {\n";

        temp += "\t\t\t\tvar upstack = parseFloat(a[\"y\"].value) > ymin;\n"
                "\t\t\t} else {\n"
                "\t\t\t\tvar upstack = parseFloat(a[\"y\"].value) < ymin;\n"
                "\t\t\t}\n"
                "\t\t\tif (upstack) {\n"
                "\t\t\t\t// Direct ancestor\n"
                "\t\t\t\tif (ex <= xmin && (ex+ew+fudge) >= xmax) {\n"
                "\t\t\t\t\te.style[\"opacity\"] = \"0.5\";\n"
                "\t\t\t\t\tzoom_parent(e);\n"
                "\t\t\t\t\te.onclick = function(e){unzoom(); zoom(this);};\n"
                "\t\t\t\t\tupdate_text(e);\n"
                "\t\t\t\t}\n"
                "\t\t\t\t// not in current path\n"
                "\t\t\t\telse\n"
                "\t\t\t\t\te.style[\"display\"] = \"none\";\n"
                "\t\t\t}\n"
                "\t\t\t// Children maybe\n"
                "\t\t\telse {\n"
                "\t\t\t\t// no common path\n"
                "\t\t\t\tif (ex < xmin || ex + fudge >= xmax) {\n"
                "\t\t\t\t\te.style[\"display\"] = \"none\";\n"
                "\t\t\t\t}\n"
                "\t\t\t\telse {\n"
                "\t\t\t\t\tzoom_child(e, xmin, ratio);\n"
                "\t\t\t\t\te.onclick = function(e){zoom(this);};\n"
                "\t\t\t\t\tupdate_text(e);\n"
                "\t\t\t\t}\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction unzoom() {\n"
                "\t\tvar unzoombtn = document.getElementById(\"unzoom\");\n"
                "\t\tunzoombtn.style[\"opacity\"] = \"0.0\";\n"
                "\t\t\n"
                "\t\tvar el = document.getElementsByTagName(\"g\");\n"
                "\t\tfor(i=0;i<el.length;i++) {\n"
                "\t\t\tel[i].style[\"display\"] = \"block\";\n"
                "\t\t\tel[i].style[\"opacity\"] = \"1\";\n"
                "\t\t\tzoom_reset(el[i]);\n"
                "\t\t\tupdate_text(el[i]);\n"
                "\t\t}\n"
                "\t}\t\n"
                "\n"
                "\t// search\n"
                "\tfunction reset_search() {\n"
                "\t\tvar el = document.getElementsByTagName(\"rect\");\n"
                "\t\tfor (var i=0; i < el.length; i++) {\n"
                "\t\t\torig_load(el[i], \"fill\")\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction search_prompt() {\n"
                "\t\tif (!searching) {\n"
                "\t\t\tvar term = prompt(\"Enter a search term (regexp \" +\n"
                "\t\t\t    \"allowed, eg: ^ext4_)\", \"\");\n"
                "\t\t\tif (term != null) {\n"
                "\t\t\t\tsearch(term)\n"
                "\t\t\t}\n"
                "\t\t} else {\n"
                "\t\t\treset_search();\n"
                "\t\t\tsearching = 0;\n"
                "\t\t\tsearchbtn.style[\"opacity\"] = \"0.1\";\n"
                "\t\t\tsearchbtn.firstChild.nodeValue = \"Search\"\n"
                "\t\t\tmatchedtxt.style[\"opacity\"] = \"0.0\";\n"
                "\t\t\tmatchedtxt.firstChild.nodeValue = \"\"\n"
                "\t\t}\n"
                "\t}\n"
                "\tfunction search(term) {\n"
                "\t\tvar re = new RegExp(term);\n"
                "\t\tvar el = document.getElementsByTagName(\"g\");\n"
                "\t\tvar matches = new Object();\n"
                "\t\tvar maxwidth = 0;\n"
                "\t\tfor (var i = 0; i < el.length; i++) {\n"
                "\t\t\tvar e = el[i];\n"
                "\t\t\tif (e.attributes[\"class\"].value != \"func_g\")\n"
                "\t\t\t\tcontinue;\n"
                "\t\t\tvar func = g_to_func(e);\n"
                "\t\t\tvar rect = find_child(e, \"rect\");\n"
                "\t\t\tif (rect == null) {\n"
                "\t\t\t\t// the rect might be wrapped in an anchor\n"
                "\t\t\t\t// if nameattr href is being used\n"
                "\t\t\t\tif (rect = find_child(e, \"a\")) {\n"
                "\t\t\t\t    rect = find_child(r, \"rect\");\n"
                "\t\t\t\t}\n"
                "\t\t\t}\n"
                "\t\t\tif (func == null || rect == null)\n"
                "\t\t\t\tcontinue;\n"
                "\n"
                "\t\t\t// Save max width. Only works as we have a root frame\n"
                "\t\t\tvar w = parseFloat(rect.attributes[\"width\"].value);\n"
                "\t\t\tif (w > maxwidth)\n"
                "\t\t\t\tmaxwidth = w;\n"
                "\n"
                "\t\t\tif (func.match(re)) {\n"
                "\t\t\t\t// highlight\n"
                "\t\t\t\tvar x = parseFloat(rect.attributes[\"x\"].value);\n"
                "\t\t\t\torig_save(rect, \"fill\");\n"
                "\t\t\t\trect.attributes[\"fill\"].value =\n"
                "\t\t\t\t    \"rgb(230,0,230)\";\n"
                "\n"
                "\t\t\t\t// remember matches\n"
                "\t\t\t\tif (matches[x] == undefined) {\n"
                "\t\t\t\t\tmatches[x] = w;\n"
                "\t\t\t\t} else {\n"
                "\t\t\t\t\tif (w > matches[x]) {\n"
                "\t\t\t\t\t\t// overwrite with parent\n"
                "\t\t\t\t\t\tmatches[x] = w;\n"
                "\t\t\t\t\t}\n"
                "\t\t\t\t}\n"
                "\t\t\t\tsearching = 1;\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t\tif (!searching)\n"
                "\t\t\treturn;\n"
                "\n"
                "\t\tsearchbtn.style[\"opacity\"] = \"1.0\";\n"
                "\t\tsearchbtn.firstChild.nodeValue = \"Reset Search\"\n"
                "\n"
                "\t\t// calculate percent matched, excluding vertical overlap\n"
                "\t\tvar count = 0;\n"
                "\t\tvar lastx = -1;\n"
                "\t\tvar lastw = 0;\n"
                "\t\tvar keys = Array();\n"
                "\t\tfor (k in matches) {\n"
                "\t\t\tif (matches.hasOwnProperty(k))\n"
                "\t\t\t\tkeys.push(k);\n"
                "\t\t}\n"
                "\t\t// sort the matched frames by their x location\n"
                "\t\t// ascending, then width descending\n"
                "\t\tkeys.sort(function(a, b){\n"
                "\t\t\t\treturn a - b;\n"
                "\t\t\tif (a < b || a > b)\n"
                "\t\t\t\treturn a - b;\n"
                "\t\t\treturn matches[b] - matches[a];\n"
                "\t\t});\n"
                "\t\t// Step through frames saving only the biggest bottom-up frames\n"
                "\t\t// thanks to the sort order. This relies on the tree property\n"
                "\t\t// where children are always smaller than their parents.\n"
                "\t\tfor (var k in keys) {\n"
                "\t\t\tvar x = parseFloat(keys[k]);\n"
                "\t\t\tvar w = matches[keys[k]];\n"
                "\t\t\tif (x >= lastx + lastw) {\n"
                "\t\t\t\tcount += w;\n"
                "\t\t\t\tlastx = x;\n"
                "\t\t\t\tlastw = w;\n"
                "\t\t\t}\n"
                "\t\t}\n"
                "\t\t// display matched percent\n"
                "\t\tmatchedtxt.style[\"opacity\"] = \"1.0\";\n"
                "\t\tpct = 100 * count / maxwidth;\n"
                "\t\tif (pct == 100)\n"
                "\t\t\tpct = \"100\"\n"
                "\t\telse\n"
                "\t\t\tpct = pct.toFixed(1)\n"
                "\t\tmatchedtxt.firstChild.nodeValue = \"Matched: \" + pct + \"%\";\n"
                "\t}\n"
                "\tfunction searchover(e) {\n"
                "\t\tsearchbtn.style[\"opacity\"] = \"1.0\";\n"
                "\t}\n"
                "\tfunction searchout(e) {\n"
                "\t\tif (searching) {\n"
                "\t\t\tsearchbtn.style[\"opacity\"] = \"1.0\";\n"
                "\t\t} else {\n"
                "\t\t\tsearchbtn.style[\"opacity\"] = \"0.1\";\n"
                "\t\t}\n"
                "\t}\n"
                "\t\n"
                "]]>\n"
                "</script>\n"
                "\n"
                "\n"
                "";

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
                to_string(c.b) + ")\" opacity=\""+ to_string(opacity)+" \" rx=\"2\" ry=\"2\" />\n";
        temp += "<text text-anchor=\"\" x=\""+to_string(x+3)+"\" y=\""+to_string(y+10)+"\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\"></text>";
        temp += "</g>";
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
        temp += "\" ""height=\"" + to_string(height) + "\" fill=\""+ color +"\" opacity=\""+ to_string(opacity)+" \" rx=\"2\" ry=\"2\" />\n";
        temp += "<text text-anchor=\"\" x=\""+to_string(x+3)+"\" y=\""+to_string(y+10)+"\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\"></text>";
        temp += "</g>";
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
