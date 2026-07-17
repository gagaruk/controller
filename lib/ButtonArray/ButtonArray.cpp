#include "ControllerConfig.h"
#include "ButtonArray.h"

c_ButtonArray::c_ButtonArray(uint8_t W_pin, uint8_t A_pin, uint8_t S_pin, uint8_t D_pin, uint8_t X_pin)
    : _W_pin(W_pin), _A_pin(A_pin), _S_pin(S_pin), _D_pin(D_pin), _X_pin(X_pin)
{
}
void c_ButtonArray::init()
{
    pinMode(_W_pin, INPUT_PULLDOWN);
    pinMode(_A_pin, INPUT_PULLDOWN);
    pinMode(_S_pin, INPUT_PULLDOWN);
    pinMode(_D_pin, INPUT_PULLDOWN);
    pinMode(_X_pin, INPUT_PULLDOWN);
}

void c_ButtonArray::update()
{
    if (millis() - _lastDebounceTime < ButtonArray::DEBOUNCE_DELAY)
        return;
    _lastDebounceTime = millis();

    _previousStates = _currentStates;
    _currentStates = 0;

    _currentStates |= (digitalRead(_W_pin) == ButtonArray::pressedStates::W) ? ButtonArray::masks::W : 0;
    _currentStates |= (digitalRead(_A_pin) == ButtonArray::pressedStates::A) ? ButtonArray::masks::A : 0;
    _currentStates |= (digitalRead(_S_pin) == ButtonArray::pressedStates::S) ? ButtonArray::masks::S : 0;
    _currentStates |= (digitalRead(_D_pin) == ButtonArray::pressedStates::D) ? ButtonArray::masks::D : 0;
    _currentStates |= (digitalRead(_X_pin) == ButtonArray::pressedStates::X) ? ButtonArray::masks::X : 0;
}

bool c_ButtonArray::isPressed(uint8_t buttonMask) const
{
    return (_currentStates & buttonMask) != 0;
}

bool c_ButtonArray::isJustPressed(uint8_t buttonMask) const
{

    return ((_currentStates & buttonMask) != 0) && ((_previousStates & buttonMask) == 0);
}

ButtonData c_ButtonArray::getPressedStruct() const
{
    ButtonData data;
    data.a = isPressed(ButtonArray::masks::W);
    data.w = isPressed(ButtonArray::masks::A);
    data.s = isPressed(ButtonArray::masks::S);
    data.d = isPressed(ButtonArray::masks::D);
    data.x = isPressed(ButtonArray::masks::X);

    return data;
}

ButtonData c_ButtonArray::getJustPressedStruct() const
{
    ButtonData data;
    data.a = isJustPressed(ButtonArray::masks::W);
    data.w = isJustPressed(ButtonArray::masks::A);
    data.s = isJustPressed(ButtonArray::masks::S);
    data.d = isJustPressed(ButtonArray::masks::D);
    data.x = isJustPressed(ButtonArray::masks::X);

    return data;
}