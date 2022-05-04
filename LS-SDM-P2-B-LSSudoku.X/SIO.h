#ifndef SIO_
#define SIO_
void SIONovaDireccio(char num);
void SIOStartGame(char usuari);
signed char SIOUsuariActua(void);
void SIONovaTecla(signed char tecla);
void motorSIO(void);
__bit SIOcheckKrebut(void);
void SIOendGame(void);
#endif