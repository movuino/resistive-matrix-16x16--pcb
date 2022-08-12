#if !defined(_BATTERY_MANAGER_)
#define _BATTERY_MANAGER_

// Battery statue
#define BATTERY_PIN 36       // Used to read the battery level
#define BATTERY_MIN_VAL 1900 // ~3.3v
#define BATTERY_MAX_VAL 2500 // ~4.2v

class BatteryManager
{
private:
    /* data */
public:
    BatteryManager(/* args */);
    ~BatteryManager();

    int getBatteryLevel();
};

BatteryManager::BatteryManager(/* args */)
{
    pinMode(BATTERY_PIN, INPUT);
}

BatteryManager::~BatteryManager()
{
}

int BatteryManager::getBatteryLevel()
{
  int sum_;
  int level_;

  sum_ = 0;
  for (uint8_t i = 0; i < 10; i++) // Do the average over 10 values
    sum_ += analogRead(BATTERY_PIN);
  
  if (sum_ < BATTERY_MIN_VAL * 10)
    sum_ = BATTERY_MIN_VAL * 10;

  if (sum_ > BATTERY_MAX_VAL * 10)
    sum_ = BATTERY_MAX_VAL * 10;
   
  level_ = ((float)((sum_ / 10) - BATTERY_MIN_VAL ) / (float)(BATTERY_MAX_VAL - BATTERY_MIN_VAL)) * 100.0;
  
  return level_;
}



#endif // _BATTERY_MANAGER_
