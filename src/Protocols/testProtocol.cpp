#include "testProtocol.hpp"
#include "../Protocols/robot.h"
#include "../IA/ia.hpp"

#include "../Utils/asservUtil.hpp"
#include "../Utils/sensorUtil.hpp"
#include "../Utils/armUtil.hpp"
#include "../Utils/Settings.hpp"

void TestProtocol::update(){ //execute the next action of this protocol
  switch(state_){
    case 0:
    std::cout<<text_;
    break;
    case 1:
    Utils::SensorUtil::instance()->reset();
    //Utils::SensorUtil::instance()->enableSensor(FRONT_LEFT_SENSOR);
    //Utils::SensorUtil::instance()->enableSensor(FRONT_RIGHT_SENSOR);
    Utils::AsservUtil::instance()->forward(300);
    if(Utils::Settings::getFlag("isLeftSide")){
        Utils::ArmUtil::instance()->module->sendCommand("moveL;");
    }else{
        Utils::ArmUtil::instance()->module->sendCommand("moveR;");
    }
    break;
    case 2:
	/*Utils::SensorUtil::instance()->disableSensor(FRONT_LEFT_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(FRONT_RIGHT_SENSOR);
	Utils::SensorUtil::instance()->enableSensor(LEFT_FRONT_SENSOR);
    Utils::SensorUtil::instance()->enableSensor(RIGHT_FRONT_SENSOR);
   	Utils::SensorUtil::instance()->enableSensor(LEFT_BACK_SENSOR);
    Utils::SensorUtil::instance()->enableSensor(RIGHT_BACK_SENSOR);*/
	Utils::AsservUtil::instance()->rotate(Utils::Settings::getFlag("isLeftSide")?-PI/2:PI/2);
    break;
    case 3:
    Utils::SensorUtil::instance()->enableSensor(FRONT_LEFT_SENSOR);
    Utils::SensorUtil::instance()->enableSensor(FRONT_RIGHT_SENSOR);
	/*Utils::SensorUtil::instance()->disableSensor(LEFT_FRONT_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(RIGHT_FRONT_SENSOR);
   	Utils::SensorUtil::instance()->disableSensor(LEFT_BACK_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(RIGHT_BACK_SENSOR);*/
	Utils::AsservUtil::instance()->forward(1050);
    break;
    case 4:
    Utils::SensorUtil::instance()->disableSensor(FRONT_LEFT_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(FRONT_RIGHT_SENSOR);
    if(Utils::Settings::getFlag("isLeftSide")){
        //Utils::SensorUtil::instance()->enableSensor(LEFT_FRONT_SENSOR);
        Utils::SensorUtil::instance()->enableSensor(RIGHT_FRONT_SENSOR);
        //Utils::SensorUtil::instance()->enableSensor(LEFT_BACK_SENSOR);
        Utils::SensorUtil::instance()->enableSensor(RIGHT_BACK_SENSOR);

    }else{
    	Utils::SensorUtil::instance()->enableSensor(LEFT_FRONT_SENSOR);
        //Utils::SensorUtil::instance()->enableSensor(RIGHT_FRONT_SENSOR);
       	Utils::SensorUtil::instance()->enableSensor(LEFT_BACK_SENSOR);
        //Utils::SensorUtil::instance()->enableSensor(RIGHT_BACK_SENSOR);
    }
    Utils::AsservUtil::instance()->rotate(Utils::Settings::getFlag("isLeftSide")?PI/2:-PI/2);
    break;
    case 5:
    Utils::ArmUtil::instance()->module->sendCommand("moveM;");
    break;
    case 6:
	Utils::SensorUtil::instance()->enableSensor(FRONT_LEFT_SENSOR);
    Utils::SensorUtil::instance()->enableSensor(FRONT_RIGHT_SENSOR);
	Utils::SensorUtil::instance()->disableSensor(LEFT_FRONT_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(RIGHT_FRONT_SENSOR);
   	Utils::SensorUtil::instance()->disableSensor(LEFT_BACK_SENSOR);
    Utils::SensorUtil::instance()->disableSensor(RIGHT_BACK_SENSOR);
    Utils::AsservUtil::instance()->forward(1000);
    break;
    default:
    // Anomaly
    break;
  }
  state_++;
}

bool TestProtocol::isCompleted(){ //wether the last action of this protocol have already been executed or not
  return state_>5;
}

unsigned short int TestProtocol::getPriority(){
  return priority_;
}
