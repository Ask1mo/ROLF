#ifndef BASEPIPEPIECES_H
#define BASEPIPEPIECES_H

//Todo: Fix weakness in parsing base pieces under 100 as they are converted to strings, and the leading 0 is removed. Which corrupts the message when it is sent to the server.

// "|" or "--" means LED pipe connection
// "O" means controller/heart module
// "+" means LED pipe going up from a controller/heart module
// "-" means LED pipe going down from a controller/heart module
// "*" means LED pipe going up and down from a controller/heart module

//Bit 7: North
//Bit 6: East
//Bit 5: South
//Bit 4: West
//Bit 3: Up
//Bit 2: Down
//Bit 0b1: N/A
//Bit 0: N/A

#define BASE_PIPE_UNKNOWN 0
#

#define BASE_PIPE_FORWARDBACKWARD       251
#define BASE_PIPE_LEFTRIGHT             252
#define BASE_PIPE_UPDOWN                253
#define BASE_PIPE_ENDCAP                254
#define BASE_PIPE_FEMALE                255

#define BASE_HEART_UNKNOWN 0

#define BASE_HEART_X 0b11110000
/*
        |

    --  O  --

        |
*/
#define BASE_HEART_XUP 0b11111000
/*
        |

    --  +  --

        |
*/
#define BASE_HEART_XDOWN 0b11110100
/*
        |

    --  -  --

        |
*/
#define BASE_HEART_XUPDOWN 0b11111100
/*
        |

    --  *  --

        |
*/



#define BASE_HEART_I 0b10100000
/*
        |

        O

        |
*/
#define BASE_HEART_IUP 0b10101000
/*
        |

        +

        |
*/
#define BASE_HEART_IDOWN 0b10100100
/*
        |

        -

        |
*/
#define BASE_HEART_IUPDOWN 0b10101100
/*
        |

        *

        |
*/




#define BASE_HEART_L 0b11000000
/*
        |

        O  --
*/ 
#define BASE_HEART_LUP 0b11001000
/*
        |

        +  --
*/
#define BASE_HEART_LDOWN 0b11000100
/*
        |

        -  --
*/
#define BASE_HEART_LUPDOWN 0b11001100
/*
        |

        *  --
*/





#define BASE_HEART_T 0b01110000
/*
    --  O  --

        |
*/
#define BASE_HEART_TUP 0b01111000
/*
    --  +  --

        |
*/
#define BASE_HEART_TDOWN 0b01110100
/*
    --  -  --

        |
*/
#define BASE_HEART_TUPDOWN 0b01111100
/*
    --  *  --

        |
*/
#endif