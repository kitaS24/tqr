# Readme for the tqr by kitaS24

tqr is a terminal tool that converts text into QR codes.

# Example
```
tqr "Hello, World"
```

# Build

```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

At this point tool should build. (Tested on Linux)

# Manual

```
════════════════════════════════
           HELP/MAN
════════════════════════════════
Usage: tqr [OPTION...] [STRING...]

    -d2, --d2           display in half blocks (default). 2 pixels/char
    -d4, --d4           display in quad blocks. 4 pixels/char                                                      
    -d8, --d8           display with braille. 8 pixels/char                                                        
-d4 and -d8 produce more compact QR codes.                                                                         
Readers might have a problem with reading them due to a terminal font                                              
                                                                                                                   
    --err-low           QR error compression: LOW                                                                  
    --err-medium        QR error compression: MEDIUM (default)                                                     
    --err-quartile      QR error compression: QUARTILE                                                             
    --err-high          QR error compression: HIGH                                                                 
                                                                                                                   
    -h, --help          give this help list                                                                        
    -man, --man         give this help list                                                                        
                                                                                                                   
Everything else is a text that will be converted                                                                   
════════════════════════════════                                                                                   
```

---
# License
**This project is licensed under the MIT LICENSE**

## Third-Party Licenses
**This project uses third-party library that has its own license**
* [QR-Code-generator]()  **| https://github.com/nayuki/QR-Code-generator** (MIT)