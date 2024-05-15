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

#define BASEPIECE_UNKNOWN 0

#define BASEPIECE_X 11110000
/*
        |

    --  O  --

        |
*/
#define BASEPIECE_XUP 11111000
/*
        |

    --  +  --

        |
*/
#define BASEPIECE_XDOWN 11110100
/*
        |

    --  -  --

        |
*/
#define BASEPIECE_XUPDOWN 11111100
/*
        |

    --  *  --

        |
*/



#define BASEPIECE_I 10100000
/*
        |

        O

        |
*/
#define BASEPIECE_IUP 10101000
/*
        |

        +

        |
*/
#define BASEPIECE_IDOWN 10100100
/*
        |

        -

        |
*/
#define BASEPIECE_IUPDOWN 10101100
/*
        |

        *

        |
*/




#define BASEPIECE_L 11000000
/*
        |

        O  --
*/ 
#define BASEPIECE_LUP 11001000
/*
        |

        +  --
*/
#define BASEPIECE_LDOWN 11000100
/*
        |

        -  --
*/
#define BASEPIECE_LUPDOWN 11001100
/*
        |

        *  --
*/





#define BASEPIECE_T 01110000
/*
    --  O  --

        |
*/
#define BASEPIECE_TUP 01111000
/*
    --  +  --

        |
*/
#define BASEPIECE_TDOWN 01110100
/*
    --  -  --

        |
*/
#define BASEPIECE_TUPDOWN 01111100
/*
    --  *  --

        |
*/
