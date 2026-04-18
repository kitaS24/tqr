//
// Created by kita on 18.4.26
//


#define QrDisplay_Half 0
#define QrDisplay_Quad 1
#define QrDisplay_Braille 2

#define QrEc_LOW 0
#define QrEc_MEDIUM 1
#define QrEc_QUARTILE 2
#define QrEc_HIGH 3

#include "thirdparty/qrcodegen.hpp"
#include "thirdparty/qrcodegen.cpp"
using namespace qrcodegen;

class App {
private:

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

uint8_t MapToBraille(bool Px[8]) {
    return (Px[0]<<0)+(Px[1]<<3)
    +(Px[2]<<1)+(Px[3]<<4)
    +(Px[4]<<2)+(Px[5]<<5)
    +(Px[6]<<6)+(Px[7]<<7);
}
uint8_t Map4Assemble(bool Px[4]) {
    return (Px[0]<<0)+(Px[1]<<1)
    +(Px[2]<<2)+(Px[3]<<3)
    ;
}
uint8_t Map2Assemble(bool Px[2]) {
    return (Px[0]<<0)+(Px[1]<<1)
    ;
}

void OutputBraille(std::vector<std::vector<bool>> &out) {
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    uint16_t size = out.size();

    uint8_t x=ceil(float(size)/2.0f);
    uint8_t y=ceil(float(size)/4.0f);

    bool Px[8] = {};

    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            for (int k=0;k<8;k++) {
                if (j*4+(int(k/2)) >= size ||i*2+(k&1) >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*4+(int(k/2))][i*2+(k&1)];
                }
            }

            std::cout << to_utf8(0x2800+MapToBraille(Px));
        }
        std::cout << "\n";
    }
}
void OutputB(std::vector<std::vector<bool>> &out) {
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    const char* quad[16] = {
        " ",  "▘", "▝", "▀",
        "▖", "▌", "▞", "▛",
        "▗", "▚", "▐", "▜",
        "▄", "▙", "▟", "█"
    };

    uint16_t size = out.size();

    uint8_t x=ceil(float(size)/2.0f);
    uint8_t y=ceil(float(size)/2.0f);

    bool Px[4] = {};

    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            for (int k=0;k<4;k++) {
                if (j*2+(int(k/2)) >= size ||i*2+(k&1) >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*2+(int(k/2))][i*2+(k&1)];
                }
            }

            std::cout << quad[Map4Assemble(Px)];
        }
        std::cout << "\n";
    }
}

void OutputA(std::vector<std::vector<bool>> &out) {
    if (out.size() == 0) {return;}

    for (int i=0;i<out.size();i++) {
        if (out[i].size() != out.size()) {
            return;
        }
    }

    const char* quad[4] = {
        " ",  "▀","▄","█"
    };

    uint16_t size = out.size();

    uint8_t x=ceil(float(size)/1.0f);
    uint8_t y=ceil(float(size)/2.0f);

    bool Px[2] = {};

    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {

            for (int k=0;k<2;k++) {
                if (j*2+k >= size ||i >=size) {
                    Px[k] = false;
                }else {
                    Px[k] = out[j*2+k][i];
                }
            }

            std::cout << quad[Map2Assemble(Px)];
        }
        std::cout << "\n";
    }
}

public:

    void OutputQrToTerminal(std::string text, int type,int ErrCorrection) {

        QrCode::Ecc ec;

        switch (ErrCorrection) {
            case 0:
                ec=QrCode::Ecc::LOW;
                break;
            case 1:
                ec=QrCode::Ecc::MEDIUM;
                break;
            case 2:
                ec=QrCode::Ecc::QUARTILE;
                break;
            case 3:
                ec=QrCode::Ecc::HIGH;
                break;
        }

        QrCode qr0 = QrCode::encodeText(text.c_str(), ec);

        std::vector<std::vector<bool>> img;
        for (int y = 0; y < qr0.getSize(); y++) {
            img.push_back({});
            for (int x = 0; x < qr0.getSize(); x++) {
                img[y].push_back(qr0.getModule(x, y));
            }
        }

        switch (type) {
            case QrDisplay_Braille:
                OutputBraille(img);
                break;
            case QrDisplay_Half:
                OutputA(img);
                break;
            case QrDisplay_Quad:
                OutputB(img);
                break;
        }
    }
};