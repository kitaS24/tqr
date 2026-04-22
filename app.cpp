//
// Created by kita on 18.4.26
//


#define QrDisplay_Half 0
#define QrDisplay_Quad 1
#define QrDisplay_Braille 2
//#define QrDisplay_Ascii 3

#define QrEc_LOW 0
#define QrEc_MEDIUM 1
#define QrEc_QUARTILE 2
#define QrEc_HIGH 3

#include "thirdparty/qrcodegen.hpp"
#include "thirdparty/qrcodegen.cpp"
using namespace qrcodegen;

///
/// class tqrCli:
///
/// tqrCli class is a class for converting text to QR codes.
/// this class relies on QR-Code-generator library by nayuki.
///


class tqrCli {
private:

    //this function generates an ANSI color code for terminal
std::string CodeGen(unsigned char FG,unsigned char BG){

    if(FG >7){
        if(BG >7){
            return "\033[9" +std::to_string(int(FG-8))+";10"+std::to_string(int(BG-8))+ "m";
        }else{
            return "\033[9" +std::to_string(int(FG-8))+";4"+std::to_string(int(BG))+ "m";
        }
    }else{
        if(BG >7){
            return "\033[3" +std::to_string(int(FG))+";10"+std::to_string(int(BG-8))+ "m";
        }else{
            return "\033[3" +std::to_string(int(FG))+";4"+std::to_string(int(BG))+ "m";
        }
    }

}


    // function converts an integer to utf8 char. Used for braille output
std::string to_utf8(char32_t cp) {
    std::string result;

    if (cp <= 0x7F)
        result += static_cast<char>(cp);
    else if (cp <= 0x7FF) {
        result += static_cast<char>(0xC0 | (cp >> 6));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        result += static_cast<char>(0xE0 | (cp >> 12));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        result += static_cast<char>(0xF0 | (cp >> 18));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }

    return result;
}

    //function maps a pixel array to braille char offset
uint8_t MapToBraille(bool Px[8]) {
    return (Px[0]<<0)+(Px[1]<<3)
    +(Px[2]<<1)+(Px[3]<<4)
    +(Px[4]<<2)+(Px[5]<<5)
    +(Px[6]<<6)+(Px[7]<<7);
}
    //functions map array to char offset
uint8_t Map4Assemble(bool Px[4]) {
    return (Px[0]<<0)+(Px[1]<<1)
    +(Px[2]<<2)+(Px[3]<<3)
    ;
}
uint8_t Map2Assemble(bool Px[2]) {
    return (Px[0]<<0)+(Px[1]<<1)
    ;
}

    //Output functions:

    //outputs a buffer in terminal using braille. Expects a square buffer
void OutputBraille(std::vector<std::vector<bool>> &out) {

    static int CharResX = 2;
    static int CharResY = 4;


    //check if out isn't empty && buffer is a square
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    uint16_t size = out.size();

    //calculating chars needed to build the buffer. Each braille char can show 2x4 area
    uint8_t x=ceil(float(size)/float(CharResX));//find width, divide by chars resolution width
    uint8_t y=ceil(float(size)/float(CharResY));//find height, divide by chars resolution height

    bool Px[8] = {};

    //iterating over the buffer to display the content
    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            //fill array for mapping
            for (int k=0;k<8;k++) {
                if (j*CharResY+(int(k/CharResX)) >= size ||i*CharResX+(k%CharResX) >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*CharResY+(int(k/CharResX))][i*CharResX+(k%CharResX)];
                }
            }

            std::cout << to_utf8(0x2800+MapToBraille(Px));
        }
        std::cout << "\n";
    }
}


    //outputs a buffer in terminal using quad chars+space. Expects a square buffer
void OutputQuad(std::vector<std::vector<bool>> &out,bool ReplaceBlank) {

    static int CharResX = 2;
    static int CharResY = 2;

    //check if out isn't empty && buffer is a square
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    // chars in the right order
    const char* quad[16] = {
        " ",  "▘", "▝", "▀",
        "▖", "▌", "▞", "▛",
        "▗", "▚", "▐", "▜",
        "▄", "▙", "▟", "█"
    };
    const char* quadRep[16] = {
        "░",  "▘", "▝", "▀",
        "▖", "▌", "▞", "▛",
        "▗", "▚", "▐", "▜",
        "▄", "▙", "▟", "█"
    };

    uint16_t size = out.size();
    //calculating chars needed to build the buffer. Each char can show area 2x2
    uint8_t x=ceil(float(size)/float(CharResX));//find width, divide by chars resolution width
    uint8_t y=ceil(float(size)/float(CharResY));//find height, divide by chars resolution height

    bool Px[4] = {};

    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            //fill array for mapping. This can be done another way.
            for (int k=0;k<4;k++) {
                if (j*CharResY+(int(k/CharResX)) >= size ||i*CharResX+(k%CharResX) >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*CharResY+(int(k/CharResX))][i*CharResX+(k%CharResX)];
                }
            }

            //pixel output
            if (ReplaceBlank) {
                std::cout << quadRep[Map4Assemble(Px)];
            }else {
                std::cout << quad[Map4Assemble(Px)];
            }
        }
        std::cout << "\n";
    }
}
    //outputs a buffer in terminal using half blocks. Expects a square buffer
    //Also has an option for replacing space with a 25% filled block
void OutputHalf(std::vector<std::vector<bool>> &out,bool ReplaceBlank) {


    static int CharResX = 1;
    static int CharResY = 2;

    //check if out isn't empty && buffer is a square
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    //chars for displaying.
    const char* half[4] = {
        " ",  "▀","▄","█"
    };
    //same, but with space being replaced to work with proportional fonts
    const char* halfRep[4] = {
        "░",  "▀","▄","█"
    };

    uint16_t size = out.size();
    //calculating chars needed to build the buffer. Each char can show area 1x2
    uint8_t x=ceil(float(size)/float(CharResX));//find width, divide by chars resolution width
    uint8_t y=ceil(float(size)/float(CharResY));//find height, divide by chars resolution height

    bool Px[2] = {};

    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            //fill array for mapping. This can be done another way.
            for (int k=0;k<2;k++) {
                if (j*CharResY+k >= size ||i >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*CharResY+k][i];
                }
            }

            //depending on the option that was selected by user, use array with a space, or with a 25% filled block
            if (ReplaceBlank) {
                std::cout << halfRep[Map2Assemble(Px)];
            }else {
                std::cout << half[Map2Assemble(Px)];
            }
        }
        std::cout << "\n";
    }
}

public:

    /// this function will be executed by user.
    /// Function generates a QR code with the QR-Code-generator library by nayuki.
    /// function outputs QR code to the terminal
    /// input:
    /// std::string text            - text to be encoded
    /// int         type            - display type
    /// int         ErrCorrection   - error correction
    /// bool        ReplaceBlank    - replaces spaces with ░ for use with proportional fonts.
    ///


    void OutputQrToTerminal(std::string text, int type,int ErrCorrection,bool ReplaceBlank) {

        QrCode::Ecc ec;

        //sets an error correction. Converts int to the value that library expects
        switch (ErrCorrection) {
            case QrEc_LOW:
                ec=QrCode::Ecc::LOW;
                break;
            case QrEc_MEDIUM:
                ec=QrCode::Ecc::MEDIUM;
                break;
            case QrEc_QUARTILE:
                ec=QrCode::Ecc::QUARTILE;
                break;
            case QrEc_HIGH:
                ec=QrCode::Ecc::HIGH;
                break;
        }

        //generating qrcode
        QrCode qr0 = QrCode::encodeText(text.c_str(), ec);

        //creating a buffer
        std::vector<std::vector<bool>> img;
        for (int y = 0; y < qr0.getSize(); y++) {
            img.push_back({});
            for (int x = 0; x < qr0.getSize(); x++) {
                img[y].push_back(qr0.getModule(x, y));
            }
        }

        //printing
        switch (type) {
            case QrDisplay_Braille:
                OutputBraille(img);
                break;
            case QrDisplay_Half:
                OutputHalf(img,ReplaceBlank);
                break;
            case QrDisplay_Quad:
                OutputQuad(img,ReplaceBlank);
                break;
                /*
            case QrDisplay_Ascii:
                OutputC(img);
                break;*/
        }
    }
};