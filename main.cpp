#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include "app.cpp"
#include "man.cpp"

std::string ColorCode(unsigned char r, unsigned char g, unsigned char b) {
    std::string c = "\x1b[38;2;";
    //  \x1b[38;2;255;0;255m
    c+= std::to_string(r)+";";
    c+= std::to_string(g)+";";
    c+= std::to_string(b)+"m";
    return c;
}

int main(int argc, char* argv[]) {

    QrDisplay DispType = QrDisplay::Half;
    QrEc EcType = QrEc::MEDIUM;
    bool RepBlank = false;
    bool FlagAuto = false;

    std::vector<std::string> Strs = {};

    for (int i=1;i<argc;i++) {

        std::string StrArg = std::string(argv[i]);

        bool IsFlag = false;

        if (StrArg == "-d2" || StrArg == "--d2") {
            DispType=QrDisplay::Half;
            IsFlag = true;
        }
        if (StrArg == "-d4" || StrArg == "--d4") {
            DispType=QrDisplay::Quad;
            IsFlag = true;
        }
        if (StrArg == "-d8" || StrArg == "--d8") {
            DispType=QrDisplay::Braille;
            IsFlag = true;
        }
        /*
        if (StrArg == "-d0.5" || StrArg == "--d0.5") {
            DispType=QrDisplay_Ascii;
            IsFlag = true;
        }*/

        if (StrArg == "-err-low" || StrArg == "--err-low") {
            EcType=QrEc::LOW;
            IsFlag = true;
        }
        if (StrArg == "-err-medium" || StrArg == "--err-medium") {
            EcType=QrEc::MEDIUM;
            IsFlag = true;
        }
        if (StrArg == "-err-quartile" || StrArg == "--err-quartile") {
            EcType=QrEc::QUARTILE;
            IsFlag = true;
        }
        if (StrArg == "-err-high" || StrArg == "--err-high") {
            EcType=QrEc::HIGH;
            IsFlag = true;
        }

        if (StrArg == "-rep-blank" || StrArg == "--rep-blank") {
            RepBlank = true;
            IsFlag = true;
        }
        if (StrArg == "-replace-blank" || StrArg == "--replace-blank") {
            RepBlank = true;
            IsFlag = true;
        }

        if (StrArg == "-auto" || StrArg == "--auto") {
            FlagAuto = true;
            IsFlag = true;
        }

        if (StrArg == "-h" || StrArg == "--h") {
            man();
            return 0;
        }
        if (StrArg == "-help" || StrArg == "--help") {
            man();
            return 0;
        }
        if (StrArg == "-man" || StrArg == "--man") {
            man();
            return 0;
        }

        if (!IsFlag) {
            Strs.push_back(StrArg);
        }
    }


    tqrCli qrg;
    for (int i=0;i<Strs.size();i++) {
        std::cout <<ColorCode(128,100,100)<<Strs[i]<<"\n";
        std::cout << ColorCode(255,255,255);
        if (FlagAuto) {
            qrg.QrAuto(Strs[i],RepBlank);
        }else {
            qrg.OutputQrToTerminal(Strs[i],DispType,EcType,RepBlank);
        }
        std::cout << "\n\n\n\n";
    }
}