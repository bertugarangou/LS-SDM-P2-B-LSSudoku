#ifndef HORA_
    #define HORA_
    extern __bit jugant;
    extern char hora[6];
    void initHora(void);
    void motorHora(void);
    void HActualitzaHora(char nova[]);
    
    void HJugant(void);
    void HnoJugant(void);
    char* HGetTime(void);
    __bit HNouSegon(void);
    void HClearNouSegon(void);
#endif
