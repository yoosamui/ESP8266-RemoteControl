
void processXml( Target* t, WiFiClient client);
void processBlinkTest( Target* t,  WiFiClient client);
void processWallLamps( Target* t,  WiFiClient client);


Target* tlist[] =
{
  new Target("get xml output", "n/a", "/xml", "0", 0, 0, processXml),
  new Target("rf13 Workroom Lamp 1", "Juan workroom Lamp1", "/rf13", "2", 10000013, 13000000, NULL),
  new Target("rf14 Bedroom Red Lamp", "Sup Bedroom Lamp", "/rf14", "3", 10000014, 14000000, NULL),
  new Target("rf10 Table Lamp 4", "downstairs living room near sofa", "/rf10", "4", 10000010, 10000000, NULL),
  new Target("rf11 Table Lamp 5", "downstairs living room near WC", "/rf11", "5", 10000011, 11000000, NULL),
  new Target("rf12 Table Lamp 6", "Upstairs living room near WC", "/rf12", "6", 10000012, 12000000, NULL),
  new Target("Empty", "Empty",  "/rf15", "7", 10000015, 15000000, NULL),
  new Target("Wall lamps", "Wall", "/rf16", "8", 0, 0, processWallLamps),
  new Target("rf23 Dummy blink work room lamp", "Dummy test", "/rf23", "9", 0, 0, processBlinkTest), //Dummy blink
  new Target("Wall rf17", "Wall",  "/rf17", "10", 10000025, 12100000, NULL),
  new Target("Wall rf18", "Wall",  "/rf18", "11", 10000125, 12200000, NULL),
  new Target("Wall rf19", "Wall",  "/rf19", "12", 10000027, 12300000, NULL),
  new Target("Wall rf20", "Wall",  "/rf20", "13", 10000028, 12400000, NULL),
  new Target("Wall rf21", "Wall",  "/rf21", "14", 10000029, 12500000, NULL),
  new Target("Wall rf22", "Wall",  "/rf22", "15", 10000030, 12600000, NULL),
  new Target("Wall rf24", "Wall",  "/rf24", "16", 10000124, 14000000, NULL),
  new Target("Wall rf25", "Wall",  "/rf25", "17", 10000126, 14200000, NULL)
  
};

