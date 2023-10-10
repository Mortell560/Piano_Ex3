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
const float notes[12] = {DO, DOd, RE, REd, MI, FA, FAd, SOL, SOLd, LA, LAd, SI};
const char* notes_str[12] = {"DO", "DOd", "RE", "REd", "MI", "FA", "FAd", "SOL", "SOLd", "LA", "LAd", "SI"};
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

volatile char c = '\0';

PwmOut speaker(p26);
AnalogIn pot1(p19);
AnalogIn pot2(p20);
C12832 lcd(p5, p7, p6, p8, p11, "LCD");
PwmOut redled(p23);
PwmOut greenled(p24);
PwmOut blueled(p25);
Serial pc(USBTX, USBRX);

const char* msg[3] = {"Volume: %3.1f\n", "Octave: %3.1f\n", "Note: %s\n"};

int AnalogIn2Index(float v, int n){return (v*n==n)? n-1 : v*n ;}

float max(float i){return (i < 0)? 0.0 : i;}

float pow(float x, int n){
    float res = x;
    while (n != 0){
        if (x < 0){
            res /= x;
            n++;
        }
        else if (n > 0){
            res *= x;
            n--;
        }
    }
    return res;
}

void PlayNote(float freq, float volume){
    speaker.period(1/freq);
    speaker = volume;
}

void onCharReceived(){
    c = pc.getc();
}

int main() {
    pc.attach(&onCharReceived);
    const int ngammes = 10;
    float freq = 1;
    int old_char = 0;
    int note = 0;

    while(1) {
      float pot_val1 = pot1.read();
      float pot_val2 = pot2.read();
      float infos[2] = {pot_val1, pot_val2};
      int curr_gamme = AnalogIn2Index(infos[1], ngammes);
      //PlayNote(DO, infos[0]);
      if (c >= '0' && c <= '9'){
          note = c-'0';
      }
      else if (c == 'a'){
          note = 10;
      }
      else if (c == 'z'){
          note = 11;
      }
      freq = notes[note]*pow(2.0, curr_gamme-3);

      if (old_char != c){
          old_char = c;
          pc.printf(msg[0], infos[0]*100);
          pc.printf(msg[1], curr_gamme);
          pc.printf(msg[2], notes_str[note]);
      }

      lcd.locate(0, 4);
      lcd.printf("Volume: %3.1f", infos[0] * 100);
      lcd.locate(0, 12);
      lcd.printf("Gamme: %i", curr_gamme);

      PlayNote(freq, infos[0]);
      wait_us(1/freq);

  }
}
