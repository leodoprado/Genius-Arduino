#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11

#define UM_SEGUNDO 500
#define MEIO_SEGUNDO 250

#define INDEFINIDO -1

#define TAMANHO_SEQUENCIA 10

#define BUZZER 13

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int leds_respondidos = 0;

enum Estados {
  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_COM_SUCESSO,
  JOGO_FINALIZADO_COM_FALHA
};

void setup(){
    Serial.begin(9600);
    iniciaPortas();
    iniciaJogo();
}

void tocaSom(int frequencia) {
   tone (BUZZER, frequencia, 500);
}

void iniciaJogo(){
  int jogo = analogRead(0);
  randomSeed(jogo);
  Serial.println(jogo);
  for (int i = 0; i< TAMANHO_SEQUENCIA; i++) {
      sequenciaLuzes[i] = sorteiaCor();
     // Serial.println (sequenciaLuzes[i]);
  }
}
  
int sorteiaCor(){  
  return random(LED_VERDE, LED_AZUL+1); //2-5 (6)
}

void iniciaPortas(){
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AMARELO, OUTPUT);
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);
  
  	pinMode(BOTAO_VERDE, INPUT_PULLUP);
  	pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  	pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  	pinMode(BOTAO_AZUL, INPUT_PULLUP);
}


void loop(){
  switch(estadoAtual()){ 
    case PRONTO_PARA_PROX_RODADA:
       Serial.println("Pronto para proxima rodada!");
       preparaNovaRodada();
       break;
    case USUARIO_RESPONDENDO:
       Serial.println("Usuario respondendo!");
       processaRespostaUsuario();
       break;
    case JOGO_FINALIZADO_COM_SUCESSO:
       Serial.println("Jogo finalizado com sucesso!");
       jogoFinalizadoSucesso();
       break;
    case JOGO_FINALIZADO_COM_FALHA:
       Serial.println("Jogo finalizado com falha");
       jogoFinalizadoFalha();
       break;
   }
    delay(500);
 }

void preparaNovaRodada() {
  rodada++;
  leds_respondidos=0;
  if (rodada <= TAMANHO_SEQUENCIA) {
      tocaLedsRodada();
  }  
}

void processaRespostaUsuario() {
   int resposta = checaRespostaJogador();
  
  
  if (resposta == INDEFINIDO) {
    return;
  }
  
  Serial.println(resposta);
  
  if (resposta == sequenciaLuzes[leds_respondidos]){
     leds_respondidos++;
  }else{
     Serial.println("Resposta Errada!");
     rodada = TAMANHO_SEQUENCIA + 2;
  }
}

int estadoAtual(){
    //computa o estado atual
  if (rodada <= TAMANHO_SEQUENCIA) {
    if (leds_respondidos == rodada) {
      return PRONTO_PARA_PROX_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_SEQUENCIA +1) {
      return JOGO_FINALIZADO_COM_SUCESSO;
  } else { // + 2
      return JOGO_FINALIZADO_COM_FALHA;
  }
}


void tocaLedsRodada(){
    for(int indice = 0; indice < rodada; indice++){
        piscaLed(sequenciaLuzes[indice]);
    }
}

int piscaLed(int portaLed){
    verificaSomDoLed(portaLed);
    digitalWrite(portaLed, HIGH);
    delay(UM_SEGUNDO);
    digitalWrite(portaLed, LOW);
    delay(MEIO_SEGUNDO); 
  	return portaLed;
}

void verificaSomDoLed(int portaLed){
 switch (portaLed) {
  case LED_VERDE:
     tocaSom(200);
     break;
  case LED_AMARELO:
     tocaSom(240);
     break;
  case LED_VERMELHO:
     tocaSom(280);
     break;
  case LED_AZUL:
     tocaSom(320);
     break;
  }
}

int checaRespostaJogador(){
  if (digitalRead(BOTAO_VERDE) == LOW) {
    return piscaLed(LED_VERDE);
  }
      
  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }
      
  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }
      
  if (digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }
  return INDEFINIDO;
}

void jogoFinalizadoSucesso() {
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL); 
  delay (MEIO_SEGUNDO);
}  

void jogoFinalizadoFalha() {
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL); 
  delay (MEIO_SEGUNDO);
}
