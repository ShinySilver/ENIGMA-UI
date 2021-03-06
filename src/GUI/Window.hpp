#ifndef S5LOO_WINDOW_HPP
#define S5LOO_WINDOW_HPP 1

#include <vector>
#include <tuple>
#include <atomic>
#include <mutex>
#include <chrono>

#include "SFML/Graphics.hpp"

#include "Component.hpp"

namespace GUI {
  class Window {
    public :
      Window(std::string n="Window", double w=800.0, double h=480.0, double framerate = 0.5, sf::Color bgColor = sf::Color(0, 100, 100)):
                                                          win_{sf::VideoMode{(unsigned int)w, (unsigned int)h}, n.data()},
                                                          winMutex{},
                                                          contentPane_{},
                                                          contentPaneMutex{},
                                                          bgColor_{bgColor},
                                                          framerate_{framerate},
                                                          lastClick_{std::chrono::steady_clock::now()},
                                                          eventHandler_{&Window::handleEvent, this},
                                                          renderer_{&Window::renderLoop, this}
      {
        winMutex.lock();
        win_.requestFocus();
        winMutex.unlock();
      }

      Window(const Window&) = delete;
      Window(Window&&) = default;
      Window& operator=(const Window&) = delete;
      Window& operator=(Window&&) = default;
      virtual ~Window() = default;

      void setContent(Component *content);
      void join();
      void close();

      void render();

    private :
      void handleEvent();
      void renderLoop();

      sf::RenderWindow win_;
      std::mutex winMutex; // Protège le code SFML

      Component *contentPane_;
      std::mutex contentPaneMutex; // Permet de changer de contentpane de manière flexible

      sf::Color bgColor_;

      std::atomic<double> framerate_;
      std::chrono::steady_clock::time_point lastClick_;
      std::thread eventHandler_;
      std::thread renderer_;

  };
}

#endif
