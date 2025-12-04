import processing.serial.*;

Serial myPort;

int graphWidth = 800;
int graphHeight = 400;

ArrayList<Integer> moistureData = new ArrayList<Integer>();
ArrayList<Integer> thresholdData = new ArrayList<Integer>();

int maxPoints = 200; // max number of points to display
int padding = 50;

boolean waterLow = false; // track water supply status

void setup() {
  size(800, 400);
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[3], 9600);
}

void draw() {
  background(255);
  
  // Draw axes
  stroke(0);
  line(padding, padding, padding, graphHeight-padding); // y-axis
  line(padding, graphHeight-padding, graphWidth-padding, graphHeight-padding); // x-axis
  
  // Draw grid
  stroke(200);
  for(int i=0; i<=5; i++){
    float y = map(i, 0, 5, graphHeight-padding, padding);
    line(padding, y, graphWidth-padding, y);
    fill(0);
    text(int(map(i,0,5,0,1023)), 5, y);
  }
  
  // Draw moisture level line
  stroke(0, 0, 255);
  noFill();
  beginShape();
  for(int i=0; i<moistureData.size(); i++){
    float x = map(i, 0, maxPoints-1, padding, graphWidth-padding);
    float y = map(moistureData.get(i), 0, 1023, graphHeight-padding, padding);
    vertex(x, y);
  }
  endShape();
  
  // Draw threshold line
  stroke(255, 0, 0);
  noFill();
  beginShape();
  for(int i=0; i<thresholdData.size(); i++){
    float x = map(i, 0, maxPoints-1, padding, graphWidth-padding);
    float y = map(thresholdData.get(i), 0, 1023, graphHeight-padding, padding);
    vertex(x, y);
  }
  endShape();
  
  // Legend
  fill(0,0,255); rect(graphWidth-150, padding, 10, 10);
  fill(0); text("Moisture", graphWidth-130, padding+10);
  fill(255,0,0); rect(graphWidth-150, padding+20, 10, 10);
  fill(0); text("Threshold", graphWidth-130, padding+30);
  
  // Display water warning if low
  if (waterLow) {
    fill(255, 0, 0);
    textSize(24);
    textAlign(CENTER, CENTER);
    text("⚠ Water supply too low! ⚠", graphWidth/2, graphHeight/2);
  }
}

void serialEvent(Serial p) {
  String line = p.readStringUntil('\n');
  if (line != null) {
    line = line.trim();
    String[] parts = line.split(",");
    if (parts.length >= 3) {
      try {
        int moistureLevel = int(parts[0]);
        int threshold = int(parts[1]);
        int waterStatus = int(parts[2]); // 0 = low, 1 = enough

        // Update waterLow flag
        waterLow = (waterStatus == 0);

        // Add data
        moistureData.add(moistureLevel);
        thresholdData.add(threshold);
        
        // Limit points
        if (moistureData.size() > maxPoints) {
          moistureData.remove(0);
          thresholdData.remove(0);
        }
      } 
      catch (Exception e) {
        println("Parsing error: " + line);
      }
    }
  }
}
