void sendPacket(float readings[], DateTime fecha, int number_sensors)
{

  LoRa.beginPacket();
  LoRa.print("ID:");
  LoRa.print(ID);
  LoRa.print(", Readings:");
  for(int i=0; i<number_sensors; i++){
    LoRa.print(readings[i]);
    LoRa.print("/");
  }
  LoRa.print(", DateTime:");
  LoRa.print(fecha.day());
  LoRa.print("/");
  LoRa.print(fecha.month());
  LoRa.print("/");
  LoRa.print(fecha.year());
  LoRa.print(" ");
  LoRa.print(fecha.hour());
  LoRa.print("/");
  LoRa.print(fecha.minute());
  LoRa.print(",");
  LoRa.endPacket();

}