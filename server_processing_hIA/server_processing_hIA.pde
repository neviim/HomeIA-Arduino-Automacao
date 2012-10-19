import java.io.FileWriter;
import java.io.IOException;
import org.json.*;
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

// Definicao dos dados de homeIA humor
int cuantos = 500;      // defaut 4000
Pelo[] lista ;

float[] z = new float[cuantos]; 
float[] phi = new float[cuantos]; 
float[] largos = new float[cuantos]; 
float radio;
float rx = 0;
float ry = 0;
// ----------
String myIP = "172.10.10.37";

//Inicializa o jsonarray
JSONArray myJsonLog = new JSONArray();

void setup() {
  // --- Modulo humor homeIA 
  size(640, 360, P3D);
  
  radio = height/3;
  lista = new Pelo[cuantos];
  
  for (int i=0; i<cuantos; i++) {
     lista[i] = new Pelo();
  }
  noiseDetail(3);
  // ------------
  
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);
  myRemoteLocation = new NetAddress(myIP,12000);
}


void draw() { 
  background(0);
  
  // Monta esfera de comunicacao da homeIA
  translate(width/2, height/2);

  float rxp = ((mouseX-(width/2))*0.005);
  float ryp = ((mouseY-(height/2))*0.005);
  rx = (rx*0.9)+(rxp*0.1);
  ry = (ry*0.9)+(ryp*0.1);
  rotateY(rx);
  rotateX(ry);
  fill(0);
  noStroke();
  sphere(radio);

  for (int i = 0;i < cuantos; i++) {
    lista[i].dibujar();
  }
  // -------------------------------
}

// Por aqui faz a entrada de dados
void mousePressed() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/ping");
  
  myMessage.add(123); /* adiciona um inteiro a message osc */
  myMessage.add(456);

  /* Envia a mensagem */
  oscP5.send(myMessage, myRemoteLocation); 
}

// processa as chamadas de dados recebida, oscEvent method.
void oscEvent(OscMessage theOscMessage) {
  
  /* DEBUG - Tipo dos parametros passados */
  //println();
  //println("Pattern: "+theOscMessage.addrPattern());
  //println("Tipotag: "+theOscMessage.typetag());
  
  // Recebe parametro do arduino sistema homeIA status da Luz.
  if(theOscMessage.addrPattern().equals("/hia/luz") && theOscMessage.typetag().equals("ii")) {
    // mostra os parametros recebido.
    println();
    println("-------------------------");
    println("Mensagem recebida do osc:");
    println(" - addrpattern: "+theOscMessage.addrPattern());
    println(" - typetag:     "+theOscMessage.typetag());
    println(" - ledPin:      "+theOscMessage.get(0).intValue());
    println(" - Status:      "+theOscMessage.get(1).intValue());
    
    //Variavel para log da operacao.
    String strlog = (mydate(0) +","+ str(hour())+":"+str(minute())+":"+str(second()) +","+
                     theOscMessage.addrPattern() +","+ 
                     theOscMessage.typetag() +","+
                     theOscMessage.get(0).intValue() +","+
                     theOscMessage.get(1).intValue());
                     
    //Cria o objeto e adiciona no array.
    JSONObject oscLog = new JSONObject();
    
    oscLog.put( "patterm", theOscMessage.addrPattern() );
    oscLog.put( "data",    mydate(0) );
    oscLog.put( "hora",    str(hour())+":"+str(minute())+":"+str(second()) );
    oscLog.put( "typetag", theOscMessage.typetag() );
    oscLog.put( "ledpin",  theOscMessage.get(0).intValue() );
    oscLog.put( "status",  theOscMessage.get(1).intValue() );
    myJsonLog.put( oscLog );
    
    //Grava em arquivo.
    try {
 
      FileWriter file = new FileWriter("homeIAlog.json");
      file.write(myJsonLog.toJSONString());
      file.flush();
      file.close();
 
    } catch (IOException e) {
      e.printStackTrace();
    }
 
    System.out.print(myJsonLog);
    
    //Imprime log console.                 
    println();
    println(" - Log: "+ strlog);
    println(" - Log: "+ myJsonLog);
                    
    //ler parametro recebido.
    int parametroA = theOscMessage.get(1).intValue();
    println();
    if (parametroA == 1) { println("Luz acesa."); }
    if (parametroA == 0) { println("Luz apagada."); }
    
    //Altera variavel humor...
    
  }
  
  // Filtra se a mensagen recebida é a que tenho de ser usada, (ii, representa dois parametros interiros)
  if(theOscMessage.addrPattern().equals("/ping") && theOscMessage.typetag().equals("ii")) {
     // Trata as mensagens recebidas por osc mensagem. 
     println();
     println("-------------------------");
     println("Mensagem recebida do osc:");
     println(" - addrpattern: "+theOscMessage.addrPattern());
     println(" - typetag:     "+theOscMessage.typetag());
     
     // Ler os parametros recebidos
     int parametroA = theOscMessage.get(0).intValue();
     int parametroB = theOscMessage.get(1).intValue();
     
     println();
     println("Mensagem A: "+parametroA);
     println("Mensagem B: "+parametroB);
     
     // Se os parametros estiver corretos retorna mensagem.
     if(parametroA == 123 && parametroB == 456) {
       // Retorna a resposta da mensagem recebida.
       OscMessage resposta = new OscMessage("/pong");
       oscP5.send(resposta, myRemoteLocation);
     }
     println();  
  }
  
  if(theOscMessage.addrPattern().equals("/pong")) {
    // resposta retornada de /ping
    println("-------------------------");
    print("Resposta: ");
    println(theOscMessage.addrPattern());
    
  }
  
}

// Data
String mydate(int offset)
{ 
  Date d = new Date();
  long timestamp = d.getTime() + (86400000 * offset); 
  String date = new java.text.SimpleDateFormat("yyyyMMdd").format(timestamp); 
  return date;
}

