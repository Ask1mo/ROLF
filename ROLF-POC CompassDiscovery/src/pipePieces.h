#ifndef BASEPIPEPIECES_H
#define BASEPIPEPIECES_H

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
//Bit 1: N/A
//Bit 0: N/A

#define BASE_PIPE_UNKNOWN 0
#define BASE_PIPE_FEMALE 255

#define BASE_HEART_UNKNOWN 0

#define BASE_HEART_X 11110000
/*
        |

    --  O  --

        |
*/
#define BASE_HEART_XUP 11111000
/*
        |

    --  +  --

        |
*/
#define BASE_HEART_XDOWN 11110100
/*
        |

    --  -  --

        |
*/
#define BASE_HEART_XUPDOWN 11111100
/*
        |

    --  *  --

        |
*/



#define BASE_HEART_I 10100000
/*
        |

        O

        |
*/
#define BASE_HEART_IUP 10101000
/*
        |

        +

        |
*/
#define BASE_HEART_IDOWN 10100100
/*
        |

        -

        |
*/
#define BASE_HEART_IUPDOWN 10101100
/*
        |

        *

        |
*/




#define BASE_HEART_L 11000000
/*
        |

        O  --
*/ 
#define BASE_HEART_LUP 11001000
/*
        |

        +  --
*/
#define BASE_HEART_LDOWN 11000100
/*
        |

        -  --
*/
#define BASE_HEART_LUPDOWN 11001100
/*
        |

        *  --
*/





#define BASE_HEART_T 01110000
/*
    --  O  --

        |
*/
#define BASE_HEART_TUP 01111000
/*
    --  +  --

        |
*/
#define BASE_HEART_TDOWN 01110100
/*
    --  -  --

        |
*/
#define BASE_HEART_TUPDOWN 01111100
/*
    --  *  --

        |
*/
#endif