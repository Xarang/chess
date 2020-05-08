#include <dlfcn.h>
#include <iostream>
#include <string>

#include "listener-manager.hh"

namespace listener {


    void ListenerManager::register_board(::board::Chessboard& board) {
        for (auto listener : listeners_) {
            listener->register_board(board::ChessboardInterfaceImpl(board));
        }
        std::cout << "[LOAD] registered board\n";
    }

    ListenerManager::ListenerManager(std::vector<std::string> plugins) {
        for (auto listener : plugins) {
            void *lib = dlopen(listener.c_str(), RTLD_LAZY);
            if (!lib) {
                std::cerr << "could not open listener lib " << listener << " : " << dlerror();
                throw new std::runtime_error("could not open listener lib " + listener + " : " + dlerror());
            }
            std::cout << "[LOAD] loaded listener lib: " << listener << "\n";
            void *listenerFunc = dlsym(lib, "listener_create");
            std::cout << "[LOAD] extracted 'listener_create' from lib: " << listenerFunc << "\n";
            listener::Listener* lst = reinterpret_cast<listener::Listener*(*)()>(listenerFunc)();
            listeners_.push_back(lst);
            plugins_.push_back(lib);
            std::cout << "[LOAD] plugin " << listener << " ready to go!" << "(" << plugins_.size() << "/" << plugins.size() << ")\n";
        }
    }

    void ListenerManager::close_listeners() {
        for (auto plugin : plugins_) {
            dlclose(plugin);
        }
    }

}