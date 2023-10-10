#include "mbed.h"
#include "LM75B.h"
#include "C12832.h"

//Frequences des notes en gamme 3
#define DO  262.0
#define DOd 277.0
#define RE  294.0
#define REd 311.0
#define MI 330.0
#define FA  349.0
#define FAd 370.0
#define SOL  392.0
#define SOLd 415.0
#define LA  440.0
#define LAd 466.0
#define SI  494.0
float freq_notes[7]      = {DO, RE,  MI, FA, SOL, LA, SI};
float azerty2notes[26]      = {DO, 4*DO, 2*LAd, 2*DO, RE, 2*DOd, 2*RE, 2*REd, SOL, 2*MI, 2*FA, 2*FAd, 2*SOL, 4*DOd, SOLd, LA, LAd, REd, SI, MI, FAd, 2*SI, 2*SOLd, 2*LA, FA, DOd} ;

/*A : DO
Z : DOd
E : RE
R : REd
T : MI
Y : FA
U : FAd
I : SOL
O : SOLd
P LA
Q LAd
S SI
D 2*DO
F 2*DOd
G 2*RE
H 2*REd
J 2*MI
K 2*FA
L 2*FAd
M 2*SOL
W 2*SOLd
X 2*LA
C 2*LAd
V 2*SI
B 4*DO
N 4*DOd*/
PwmOut speaker(p26);
AnalogIn pot1(p19);
AnalogIn pot2(p20);
DigitalOut myled(LED1);

int AnalogIn2Index(float v, int n){return (v*n==n)? n-1 : v*n ;}

void PlayNote(float freq, float volume){
    speaker.period(1/freq);
    speaker = volume;
}


int main() {
  while(1) {
      PlayNote(DO, pot1.read());
      wait(1);
  }
}
