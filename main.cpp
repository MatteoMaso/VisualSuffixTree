#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

/**
 *
 * @param inputFileName file that contains the strings that you want to analyze
 */
void treeParser(char *inputFileName, char *outputFileName);

/**
 * This method take an input file well formatted and create a flameGraph as output
 * @param inputFileName formatted file to convert in FlameGraph SVG
 */
void createSvg(char *inputFileName);

void printBinFile(string &s, std::ofstream &bin_out) ;

/**
 * Class for binary file manipulation
 */
template<std::size_t N>
class BitIo {
public:

    void push_back(const std::bitset<N> &bs) {
        std::vector<Byte> result((N + 7) >> 3);
        for (int j = 0; j < int(N); ++j) {
            result[j >> 3] |= (bs[j] << (j & 7));
        }
        for (const Byte &byte : result) {
            bytes.push_back(byte);
        }
        num_bytes += NUM_BYTES_PER_BITSET;
    }

    std::bitset<N> pop_front() {
        std::bitset<N> result;
        for (int j = 0; j < int(N); ++j) {
            result[j] = ((bytes[(j >> 3) + offset] >> (j & 7)) & 1);
        }
        offset += NUM_BYTES_PER_BITSET;
        num_bytes -= NUM_BYTES_PER_BITSET;
        return result;
    }

    bool empty() {
        return num_bytes < NUM_BYTES_PER_BITSET;
    }

    void clear() {
        bytes.clear();
        num_bytes = 0;
    }

    std::size_t size() {
        return num_bytes;
    }

private:

    using Byte = unsigned char;
    static constexpr std::size_t NUM_BYTES_PER_BITSET = N / 8;

    template<std::size_t T>
    friend std::ostream &operator<<(std::ostream &os, const BitIo<T> &bio);

    template<std::size_t T>
    friend std::istream &operator>>(std::istream &is, BitIo<T> &bio);

    std::istream &read_file(std::istream &is) {
        bytes.clear();

        std::streampos current_pos, file_size;
        current_pos = is.tellg();
        is.seekg(0, std::ios::end);
        file_size = is.tellg() - current_pos;
        is.seekg(current_pos, std::ios::beg);

        bytes.resize(file_size);
        is.read((char *) &bytes[0], file_size);

        num_bytes += file_size;

        return is;
    }

    std::vector<Byte> bytes;
    std::size_t offset = 0;
    std::size_t num_bytes = 0;
};

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, const BitIo<N> &bio) {
    for (const auto &byte : bio.bytes) {
        os << byte;
    }
    return os;
}

template<std::size_t N>
std::istream &operator>>(std::istream &is, BitIo<N> &bio) {
    if (!is) {
        is.setstate(std::ios::failbit);
    }
    bio.read_file(is);
    return is;
}

//Parametri per il parser e per il coso
static const int parameters[10] = {10, 10, 8, 8};

int main(int argc, char *argv[]) {



    if (argc < 2) {
        cout << "insert -data nameInput for starting treeParser or -svg to create the SVG file" << std::endl;
        return 1;
    }

    char firstProgram[6] = "-data";
    char secondProgram[5] = "-svg";

    if (strcmp(argv[1], firstProgram) == 0) {
        treeParser(argv[2], argv[3]);
    } else if (strcmp(argv[1], secondProgram) == 0) {
        createSvg(argv[2]);
    } else {
        printf("Bad arguments!");
    }
}

string partitioner(string s, int from, int to){

    string a = "";

    for (int i = from; i <= to; ++i) {
        a += s[i];
    }

    return a;
}


void createSvg(char *inputFileName) {
    //BEGIN SVG CREATOR

    const int bitNodeDepth = 10; //fino a 1024
    const int bitDepth = 10;
    const int bitLb = 8;
    const int bitRb = 8;
    const int bitCharRepresentation = 2;

    //Read input file
    //    std::ifstream bin_in(inputFileName, std::ios::binary); //usare questo per passaggio parametri
    std::ifstream bin_in(inputFileName , std::ios::binary);

    BitIo<16> bio2;
    bin_in >> bio2;

    int nodeInfoDim = 36;
    int nodeInfoLength = 0;
    if ((nodeInfoDim % 16)  == 0){
        nodeInfoLength = nodeInfoDim / 16;
    } else {
        nodeInfoLength = nodeInfoDim / 16 + 1;
    }



//    std::cout << nodeInfoLength << std::endl; // Prints the 4 16-bit bitsets in correct order.




    //COSE NUOVE DI ALESSIA

    string a, b, c, line;
    int x, y, i, j, z, x0, y0, H;

    x0 = 10;
    y0 = 577;
    H = 15;

//    ifstream readFile("F.txt");


    cout << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
            "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
            "<svg version=\"1.1\" width=\"1000\" height=\"626\" onload=\"init(evt)\" viewBox=\"0 0 1000 626\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
            "<defs >\n"
            "\t<linearGradient id=\"background\" y1=\"0\" y2=\"1\" x1=\"0\" x2=\"0\" >\n"
            "\t\t<stop stop-color=\"#eeeeee\" offset=\"5%\" />\n"
            "\t\t<stop stop-color=\"#eeeeb0\" offset=\"95%\" />\n"
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
            "\t\t\t// Is it an ancestor\n"
            "\t\t\tif (0 == 0) {\n"
            "\t\t\t\tvar upstack = parseFloat(a[\"y\"].value) > ymin;\n"
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
            "]]>\n"
            "</script>\n"
            "\n"
            "<rect x=\"0.0\" y=\"0\" width=\"1000.0\" height=\"626.0\" fill=\"url(#background)\"  />\n"
            "<text text-anchor=\"middle\" x=\"500\" y=\"24\" font-size=\"17\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\"  >ToyString</text>\n"
            "<text text-anchor=\"\" x=\"10.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"unzoom\" onclick=\"unzoom()\" style=\"opacity:0.0;cursor:pointer\" >Reset Zoom</text>\n"
            "<text text-anchor=\"\" x=\"890.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"search\" onmouseover=\"searchover()\" onmouseout=\"searchout()\" onclick=\"search_prompt()\" style=\"opacity:0.1;cursor:pointer\" >Search</text>\n"
            "<text text-anchor=\"\" x=\"1090.00\" y=\"1249\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"matched\" > </text>"
    << std::endl;

//    while(getline(readFile,line))   {
//        stringstream iss(line);
//        getline(iss, a, '[');
//        getline(iss, b, ',');
//        getline(iss, c, ']');
//
//        i =  int32.Parse()
//
//        stoi(c);
//        x = x0 + a;
//        y = y0 + (z*H);
//        cout << "<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n"
//                "<title>DEPTH IS: " << z << "</title><rect x=\""<< x <<"\" y=\"" << y <<"\" width=\"980\" height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n"
//                                                                                        "</g>";
//    }
//    readFile.close();


    //FINE COSE NUOVE DI ALESSIA



    while (!bio2.empty()) {

        string nodeInfo = "";

        for (int i = 0; i < nodeInfoLength; ++i) {
            nodeInfo += bio2.pop_front().to_string();
        }

        string str = partitioner(nodeInfo, 0, 9);
        int x = stoi(partitioner(nodeInfo, 0, 9), nullptr, 2);


//        std::cout << "NodeDepth: " << stoi(partitioner(nodeInfo, 0, 9), nullptr, 2) << " Depth: " << stoi(partitioner(nodeInfo, 10, 19), nullptr, 2) << "-[" << stoi(partitioner(nodeInfo, 20, 27), nullptr, 2) << ","
//                  << stoi(partitioner(nodeInfo, 28, 35), nullptr, 2) << "]" <<  std::endl;

//        std::cout << nodeInfo << std::endl; //

        stoi(c);
//            "<title>DEPTH IS: " << z << "</title><rect x=\""<< x <<"\" y=\"" << y <<"\" width=\"980\" height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n"

        a = stoi(partitioner(nodeInfo, 0, 9), nullptr, 2);
        b = stoi(partitioner(nodeInfo, 20, 27), nullptr, 2);
        c = stoi(partitioner(nodeInfo, 28, 35), nullptr, 2);


        x = x0 + a;
//        y = y0 + (z*H);
//        cout << "<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n"
//

    }

};

string charEncoding(char &c, vector <string> &a, string &inputLine){

    for (int i = 0; i < inputLine.size(); i++) {
        char temp = inputLine[i];
        if ( c == temp ){
            return a[i];
        }
    }


    std::cout << "Errore in charEncoding, carattere: "<< c << " non trovato!" << std::endl; //
    exit(1);

}

/**
 * FORMAT OUTPUT FILE
 */
void treeParser(char *inputFileName, char *outputFileName) {

    std::cout << "Insert the alphabet: ABC or ARFS ... etc" << std::endl; //
    string inputLine;
    getline(cin, inputLine);
    cout << inputLine.length() << endl;

    vector <string> charCoding; //Vuoto per ora

    //TODO RENDERLO PARAMETRICO
    if (inputLine.length() <= 2){
        //1 bit
        charCoding.push_back("0");
        charCoding.push_back("1");

    } else if ( inputLine.length() > 2 && inputLine.length() <= 4){
//        2 bit
        charCoding.push_back("00");
        charCoding.push_back("01");
        charCoding.push_back("10");
        charCoding.push_back("11");
    } else if ( inputLine.length() > 4 && inputLine.length() <= 8){
        //3 bit
        charCoding.push_back("000");
        charCoding.push_back("001");
        charCoding.push_back("010");
        charCoding.push_back("011");
        charCoding.push_back("100");
        charCoding.push_back("101");
        charCoding.push_back("110");
        charCoding.push_back("111");
    } else {
        std::cout << "To much alphabet character! Todo implement" << std::endl; //
        exit(1);
    }



    for (int j = 0; j < inputLine.length(); j++) {
        std::cout << "Char: " << inputLine[j] << " Encoded: " << charCoding[j] << std::endl; //
    }






    //Lunghezza dei parametri, poi dovrò settarli da interfaccia e dovranno essere comuni anche al file che crea l'svg per essere in grado di leggerlo
    const int bitNodeDepth = 10; //fino a 1024
    const int bitDepth = 10;
    const int bitLb = 8;
    const int bitRb = 8;

    const int bitCharacter = 2;

    //Devo aggiornarlo ogni volta che aggiungo un nodo
    int nodeCounter = 0; //Contatore del numero di nodi così so quanto spazio occupa il file e quanto spazio potrei salvare

    //Constuct 01: 8 bits initialized to zero
//    bitset<8> bitset1(string("11111100"));

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    string nodeInfo = "";

    std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);
    BitIo<16> bio;

    for (iterator it = begin; it != end; ++it) {

        //Resetto la stringa del nodo info
        nodeInfo = "";

        //Per ogni nodo stampo le sue proprità
        //Print Node_depth  [0-?]
        unsigned long i = cst.node_depth(*it);
        nodeInfo += std::bitset<bitNodeDepth>(cst.node_depth(*it)).to_string();


        nodeInfo += std::bitset<bitDepth>(cst.depth(*it)).to_string();

        nodeInfo += std::bitset<bitLb>(cst.lb(*it)).to_string();

        nodeInfo += std::bitset<bitRb>(cst.rb(*it)).to_string();


        str_length = cst.depth(*it);



        //TODO DA STAMPARE SOLO IL RAMO E NON TUTTA LA STRINGA
        string edge = "";

        if ((cst.node_depth(*it) == 0) ||
            (cst.node_depth(*it) == 1 && str_length == 1 && (cst.lb(*it) == cst.rb(*it)))) {
            edge = "-";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {
                //leaf
                edge = "";

                for (int i = 1; i < str_length - 1; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "#";
                edge += "#";

            } else {
                //internal node
                edge = "";

                for (int i = 1; i <= str_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }
        }

//        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << ","
//                  << cst.rb(*it) << "]" << edge << std::endl;
//

        //TODO DEVO STAMPARE PRIMA IL NUMERO DI CARATTERI DELL'EDGE COSI POI IN LETTURA SO QUANTI LEGGERNE
//        nodeInfo += std::bitset<bitRb>(edge.length()).to_string();

        //todo togliere gli hashtag o tenerne conto di default

        for (int k = 0; k < edge.length(); k++) {
            nodeInfo += std::bitset<bitRb>(charEncoding(edge[k], charCoding, inputLine)).to_string();
//            std::cout << charEncoding(edge[k], charCoding, inputLine) << std::endl; //
        }



        printBinFile(nodeInfo, bin_out);

    }

    bin_out.close();
};


void printBinFile(string &s, std::ofstream &bin_out) {

    BitIo<16> bio;

    for (int j = 0; j <= s.length() / 16; j++) {

        string temp = "";

        for (int k = 0 + j*16; k <= 15 + j*16; k++) {
            if (k >=  s.length()) {
                temp += "0";
            } else {
                temp += s[k];
            }
        }

        bio.push_back(std::bitset<16>(temp));
    }

    bin_out << bio;

}

