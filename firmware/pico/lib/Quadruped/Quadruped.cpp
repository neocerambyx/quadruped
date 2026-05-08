#include <Quadruped.h>


void Quadruped::home() {
    _isWalking = false;
    front_left.home();
    back_left.home();
    front_right.home();
    back_right.home();
}