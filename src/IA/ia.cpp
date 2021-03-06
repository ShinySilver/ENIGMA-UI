#include "ia.hpp"
#include "iostream"

namespace AI{

  IA::IA(void (*onDisable)(),void (*onEnable)(), bool (*isBusy)()):
    protocols_{},
    protocolCount_{},
    protocolIdMutex_{},
    disabled_{false},
    enabled_{false},
    onEnable_{onEnable},
    onDisable_{onDisable},
    isBusy_{isBusy},
    mainLoop_{&IA::update, this} {

    }

  void IA::addProtocol(Protocol *protocol) {
    protocolIdMutex_.lock();
    protocolCount_+=1;
    protocols_.emplace_back(protocol);
    protocolIdMutex_.unlock();
  }

  void IA::autoselectProtocol() {
    selectedProtocolId_=-1;
    unsigned short int maxPriority = 0;
    for (unsigned short int selectedProtocolId = 0; selectedProtocolId < protocolCount_; ++selectedProtocolId) {
      if (!protocols_[selectedProtocolId]->isCompleted()) {
        if (protocols_[selectedProtocolId]->getPriority() > maxPriority
              && protocols_[selectedProtocolId]->getPriority()>PRIORITY_NULL) {
          maxPriority = protocols_[selectedProtocolId]->getPriority();
          selectedProtocolId_ = selectedProtocolId;
        }
      }
    }
  }

  void IA::update() {
    while (!disabled_) {
      /**
       * On attends le top départ
       */
      while(!enabled_){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }

      //std::cout<<"Et on repart!\n";

      /**
       * Si on atteint la fin du match, l'ia s'arrête
       */
      if((std::chrono::duration<double>)(std::chrono::system_clock::now()-matchStart_)>std::chrono::seconds(100)){
        break;
      }
      protocolIdMutex_.lock();
      if(isBusy_()){
          //std::cout<<"MB busy\n";
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }else{
          //std::cout<<"MB pas busy\n";
          if (selectedProtocolId_==-1||protocols_[selectedProtocolId_]->isCompleted()) {
            autoselectProtocol();
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Plus de stabilité
          }
          if(selectedProtocolId_!=-1) protocols_[selectedProtocolId_]->update();
          //std::cout<<"MB tjs pas busy\n";
      }
      protocolIdMutex_.unlock();
    }
    if(onDisable_) onDisable_(); // Un callback pour couper les actionneurs
  }

  void IA::enable(){
    enabled_=true;
    matchStart_=std::chrono::system_clock::now();
    if(onEnable_) onEnable_(); // Un callback pour activer les actionneurs
  }

  void IA::disable(){
    disabled_=true;
  }

  void IA::join(){
    if(mainLoop_.joinable()){
      std::cout<<"Waiting for the AI to finish...\n";
      mainLoop_.join();
      std::cout<<"AI thread joined.\n";
    }
  }

  int IA::getCurrentProtocolId(){
      return selectedProtocolId_;
  }

  int IA::getProtocolCount(){
      return protocolCount_;
  }

  void IA::forceCurrentProtocol(int protocolId){
      protocolIdMutex_.lock();
      if(protocolId>=protocolCount_ || protocolId <-1) return;
      selectedProtocolId_ = protocolId;
      if(protocolId!=-1) protocols_[selectedProtocolId_]->reset();
      forced_ = true;
      protocolIdMutex_.unlock();
  }

  void IA::reset(int protocolId){
      protocolIdMutex_.lock();
      protocols_[protocolId]->reset();
      protocolIdMutex_.unlock();
  }
}
