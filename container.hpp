#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <iostream>

#include "component.hpp"
#include "SFML/Graphics.hpp"


namespace GUI{
  class Container : public Component, ActionListener{
    public:
      Container():Component{}, components_{}{this->addActionListener(this);}

      /**
       * Rajoute un élément graphique au container
       */
      void addComponent(Component *c);

      // From component
      void render(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates::Default) override;

      // ActionListener
      void actionPerformed(sf::Event event) override {
        for(auto c : components_){
          for(auto l : c->getActionListeners()){
            l->actionPerformed(event);
          }
        }
        //std::cout<<"done transmitting event!\n";
      }

    private:
      std::vector<Component*> components_;
  };
}
#endif
