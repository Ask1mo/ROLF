//LINKPROGRAM_MK2
//LINKPROGRAM_MK3







/*---=== PUT YOUR SETTINGS HERE!!! ===---*/
#define PANELSETUP_VOICETUBE
#define PLATFORM_ESP32_WROOMDEVKIT
#define LEDTYPE_STANDARD
#define LINKPROGRAM_MK2

#define DEBUGLEVEL DEBUG_WARNINGS
/*---=== PUT YOUR SETTINGS HERE!!! ===---*/





/*---=== Pin Mapping ===---*/
#ifdef PLATFORM_ARDUINO
    #define PIN_BUTTON      2           //D2
    #define PIN_LEDS        12          //D12
    #define PIN_ERRORLED    13          //D13
    #define PIN_LIGHTSENSOR A0          //A0
    #define ENABLE_DIODECONTROL false   //Arduino is not powerful enough for per diode control. So it's disabled
    #define ENABLE_WEBCONTROL false     //Arduino has no website capabilities
    #define BAUDRATE 9600
    #define CUSTOMPALETTEAMOUNT 5
#endif
#ifdef PLATFORM_ESP32_FIREBEETLE2
    #define PIN_BUTTON      27          //On board button
    #define PIN_LEDS        4           //D?
    #define PIN_ERRORLED    2           //No pinout, only internal on board
    #define PIN_LIGHTSENSOR 35          //D?
    #define ENABLE_DIODECONTROL true    //Individual Diode Fx enabled
    #define ENABLE_WEBCONTROL true      //Controls via website enabled
    #define BAUDRATE 115200
    #define CUSTOMPALETTEAMOUNT 5
#endif
#ifdef PLATFORM_ESP32_FIREBEETLE2_DEBUG
    #define PIN_BUTTON      27          //On board button
    #define PIN_LEDS        5           //On boadr LED
    #define PIN_LIGHTSENSOR 35          //D?
    #define PIN_ERRORLED    2           //No pinout, only internal on board
    #define ENABLE_DIODECONTROL true    //Individual Diode Fx enabled
    #define ENABLE_WEBCONTROL true      //Controls via website enabled
    #define BAUDRATE 115200
    #define CUSTOMPALETTEAMOUNT 5
#endif
#ifdef PLATFORM_ESP32_WROOMDEVKIT
    #define PIN_BUTTON      27          //On board button
    #define PIN_LEDS        32           //On boadr LED
    #define PIN_LIGHTSENSOR 15          //D?
    #define PIN_ERRORLED    2           //NOT BOUND YET
    #define ENABLE_DIODECONTROL true    //Individual Diode Fx enabled
    #define ENABLE_WEBCONTROL false      //Controls via website enabled
    #define BAUDRATE 115200
    #define CUSTOMPALETTEAMOUNT 5
#endif
/*---=== Pin Mapping ===---*/

/* ---=== LED Setup ===---*/
#ifdef PANELSETUP_PROTO
    #define CANVASWIDTH     6
    #define CANVASHEIGHT    2
#endif
#ifdef PANELSETUP_ATOS
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_EVA
    #define CANVASWIDTH     3
    #define CANVASHEIGHT    3
#endif
#ifdef PANELSETUP_LIAM
    #define CANVASWIDTH     5
    #define CANVASHEIGHT    4
#endif
#ifdef PANELSETUP_PRIME
    #define CANVASWIDTH     7
    #define CANVASHEIGHT    7
#endif
#ifdef PANELSETUP_CHRISTMAS
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_WESTPOINT
    #define CANVASWIDTH     4
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_MINITOWER
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_TEST
    #define CANVASWIDTH     3
    #define CANVASHEIGHT    2
#endif
#ifdef PANELSETUP_HOUSECUBE
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_HOUSECUBEV2
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_POWERWIRE
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif
#ifdef PANELSETUP_VOICETUBE
    #define CANVASWIDTH     0
    #define CANVASHEIGHT    0
#endif

/* ---=== LED Setup ===---*/

/* ---=== Strip Setup ===---*/
#ifdef LEDTYPE_STANDARD
    #define LEDCOLORDER GRB
#endif
#ifdef LEDTYPE_CHRISTMASSTRING
    #define LEDCOLORDER RGB
#endif
/* ---=== Strip Setup ===---*/


/*---=== Debug Setup ===---*/
#define DEBUG_DISABLED              0
#define DEBUG_ERRORS                1
#define DEBUG_WARNINGS              2
#define DEBUG_OPERATIONS            3
#define DEBUG_DAYISRUINED           5
/*---=== Debug Setup ===---*/