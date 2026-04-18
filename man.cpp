//
// Created by kita on 18.4.26.
//


// man/help

std::string ColorCodeMan(unsigned char r, unsigned char g, unsigned char b) {
    std::string c = "\x1b[38;2;";
    //  \x1b[38;2;255;0;255m
    c+= std::to_string(r)+";";
    c+= std::to_string(g)+";";
    c+= std::to_string(b)+"m";
    return c;
}

void ManGray() {
    std::cout <<ColorCodeMan(255,200,200);
}
void ManDef() {
    std::cout <<ColorCodeMan(200,255,200);
}
void ManW() {
    std::cout <<ColorCodeMan(255,255,255);
}
void ManLink() {
    std::cout <<ColorCodeMan(128,128,255);
}

void TableDivider(int size) {
    for (int i=0; i<size; i++) {
        std::cout << "═";
    }
    std::cout <<"\n";
}

void man() {
    TableDivider(32);
    std::cout << "           HELP/MAN\n";
    TableDivider(32);
    std::cout << "Usage: tqr [OPTION...] [STRING...]\n\n";

    ManDef(); std::cout << "    -d2, --d2           display in half blocks (default). 2 pixels/char\n";
    ManGray();std::cout << "    -d4, --d4           display in quad blocks. 4 pixels/char\n";
    ManGray();std::cout << "    -d8, --d8           display with braille. 8 pixels/char\n";
    ManW();   std::cout << "-d4 and -d8 produce more compact QR codes.\n";
    ManW();   std::cout << "Readers might have a problem with reading them due to a terminal font\n";
    std::cout << "\n";
    ManGray();std::cout << "    --err-low           QR error compression: LOW\n";
    ManDef(); std::cout << "    --err-medium        QR error compression: MEDIUM (default)\n";
    ManGray();std::cout << "    --err-quartile      QR error compression: QUARTILE\n";
    ManGray();std::cout << "    --err-high          QR error compression: HIGH\n";
    std::cout << "\n";
    ManGray();std::cout << "    -h, --help          give this help list\n";
    ManGray();std::cout << "    -man, --man         give this help list\n";
    std::cout << "\n";

    ManW();std::cout << "Everything else is a text that will be converted\n";
    TableDivider(32);
    std::cout << "This project uses a library to convert text to QR:\n";
    ManLink();std::cout << "  https://github.com/nayuki/QR-Code-generator/tree/master\n";
    ManW();std::cout << "library is available in Java, TypeScript/JavaScript, Python, Rust, C++, C\n";
}