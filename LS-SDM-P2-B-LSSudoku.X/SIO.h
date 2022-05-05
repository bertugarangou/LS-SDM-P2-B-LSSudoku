#ifndef SIO_
#define SIO_
void SIONovaDireccio(char num);
void SIOStartGame(char usuari);
signed char SIOHaAcabatPuntuacions(void);
void SIONovaTecla(signed char tecla);
void motorSIO(void);
__bit SIOcheckKrebut(void);
void SIOendGame(void);
void initSIO(void);
#endif